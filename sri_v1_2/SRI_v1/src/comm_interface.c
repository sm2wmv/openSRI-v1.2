/*! \file comm_interface.c
 *  \brief General communication interface
 *  \ingroup comm_interface_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-12-09
 *  \code #include "comm_interface.c" \endcode
 */
//    Copyright (C) 2011  Mikael Larsmark, SM2WMV
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comm_interface.h"
#include "computer_interface.h"
#include "lcd.h"
#include "queue.h"
#include "misc.h"

CREATE_QUEUE(COMM_INTERFACE_RX_QUEUE_SIZE,struct_comm_interface_msg,comm_rx);
CREATE_QUEUE_WRAPPERS(struct_comm_interface_msg,comm_rx);

CREATE_QUEUE(COMM_INTERFACE_TX_QUEUE_SIZE,struct_comm_interface_msg,comm_tx);
CREATE_QUEUE_WRAPPERS(struct_comm_interface_msg,comm_tx);

#define PREAMBLE_FOUND	0
//! Was the last TX message acked
#define TX_MSG_ACKED	1
//! A resend should be executed
#define RESEND_FLAG		2

static uint8_t *rx_buffer;
static uint8_t *rx_buffer_start;

//! Communication protocol status
static uint8_t comm_status = 0;

//! The previous received character from UDR0
static uint8_t prev_data = 0;

//! Keep track of received number of characters
static uint16_t rx_char_count = 0;

static uint8_t resend_count = 0;

static uint8_t add_rx_message = 0;

//! Computer comm preamble
#define COMPUTER_PREAMBLE	  0xFE
//! Computer comm postamble
#define COMPUTER_POSTAMBLE	0xFD
//! Computer MSG Acknowledge
#define COMPUTER_ACK				0xFA
//! Computer MSG NOT Acknowledge
#define COMPUTER_NACK				0xFB

#define RESET_TX_MSG_ACKED comm_status &= ~(1<<TX_MSG_ACKED)
#define SET_TX_MSG_ACKED comm_status |= (1<<TX_MSG_ACKED)

static struct_comm_interface_msg	rx_message;

/*! \brief Function to be called when a message is recieved and should be parsed/executed */
static void (*f_ptr_rx)(struct_comm_interface_msg);
/*! \brief Function to be called to retrieve UART data characters */
static unsigned int (*f_ptr_poll_rx)(uint8_t*);
/*! \brief Function to be called when we wish to send a message */
static void (*f_ptr_tx)(uint8_t);

static void (*f_ptr_resend)(void);

//! Counter with 1 ms ticks
static unsigned int counter_1ms = 0;

//! Timeout for the reception of data
static uint8_t rx_data_timeout = 0;

//! Timeout for the transmission of data
static uint8_t tx_data_timeout = 0;

//! RX message checksum
static uint8_t rx_checksum = 0;

void comm_interface_send_message(struct_comm_interface_msg message);
void comm_interface_send_ack(void);
void comm_interface_send_nack(void);

void enable_comm_interface_interrupt(void) {
  //COMM_ENABLE_INT;
}

void disable_comm_interface_interrupt(void) {
  //COMM_DISABLE_INT;
}

__inline__ static void  reset_rx_status(void) {
	rx_data_timeout = 0;
	rx_char_count = 0;
	rx_checksum = 0;
	prev_data = 0;
	
	comm_status &= ~(1<<PREAMBLE_FOUND);
}

__inline__ static void  reset_tx_status(void) {
	tx_data_timeout = 0;
}

void comm_interface_reset_all(void) {
  reset_rx_status();
  reset_tx_status();
  queue_drop_all_comm_tx();
  queue_drop_all_comm_rx();

  SET_TX_MSG_ACKED;
  comm_status &= ~(1<<RESEND_FLAG);
}

void comm_interface_poll_tx_queue(void) {
	//Check that the last message sent was acked, if so we transmit the next one in the queue
	if ((comm_status & (1<<TX_MSG_ACKED)) && (!queue_is_empty_comm_tx())) {
	  tx_data_timeout = 0;
		struct_comm_interface_msg msg;
    queue_get_first_comm_tx(&msg);

    comm_interface_send_message(msg);

		RESET_TX_MSG_ACKED;
	}
	else if (comm_status & (1<<RESEND_FLAG)) {
		if (resend_count < COMM_INTERFACE_RESEND_COUNT) {
		  tx_data_timeout = 0;

		  comm_status &= ~(1<<RESEND_FLAG);

		  PRINTF("RESEND\n");

		  struct_comm_interface_msg msg;
      queue_get_first_comm_tx(&msg);
      
      comm_interface_send_message(msg);
			resend_count++;
		}
		else {
      tx_data_timeout = 0;

		  queue_drop_comm_tx();
			comm_status &= ~(1<<RESEND_FLAG);
			SET_TX_MSG_ACKED;
			resend_count = 0;
			//f_ptr_resend();
		}
		
		comm_status &= ~(1<<RESEND_FLAG);
	}
}

/*! \brief Add a message to be sent
 *  \param cmd The command
 *  \param length The number of byte data
 *  \param data The data to be sent
 *  \return 0 if everything went OK, 1 if the TX queue was full */
uint8_t comm_interface_add_tx_message(uint8_t cmd, uint8_t length, uint8_t *data) {
	struct_comm_interface_msg message;
	message.cmd = cmd;
	message.checksum = cmd;
	message.length = length;
	message.checksum += length;
	
	for (uint8_t i=0;i<length;i++) {
		message.checksum += data[i];
		message.data[i] = data[i];
	}

	return(queue_enqueue_comm_tx(&message));
}

/*! \brief Add a struct message to be sent
 *  \param cmd The command
 *  \param length The number of byte data
 *  \param offset The offset of the data to be sent
 *  \param data The data to be sent
 *  \return 0 if everything went OK, 1 if the TX queue was full, 2 if the message was too large */
uint8_t comm_interface_add_tx_struct_message(uint8_t cmd, uint8_t length, uint16_t offset, uint8_t *data) {
  //PRINTF("FIRST\n-----\n");

  if ((length+2) > COMM_INTERFACE_DATA_LENGTH)
    return(2);

  struct_comm_interface_msg message;

  message.cmd = cmd;
  message.checksum = cmd;
  message.length = length+2;
  message.checksum += message.length;

  //PRINTF("CMD: 0x%02X\n",message.cmd);
  //PRINTF("LEN: %i\n",message.length);

  message.data[0] = offset>>8;
  message.checksum += message.data[0];
  message.data[1] = offset & 0x00FF;
  message.checksum += message.data[1];

  //PRINTF("OFFSET1: %i\n",message.data[0]);
  //PRINTF("OFFSET2: %i\n",message.data[1]);

  //PRINTF("DATA: ");
  for (uint8_t i=0;i<length;i++) {
    message.checksum += data[i];
    message.data[i+2] = data[i];
    //PRINTF("0x%02X ",message.data[i+2]);
  }

  //PRINTF("\nCHECKSUM: %i\n",message.checksum);

  return(queue_enqueue_comm_tx(&message));
}

void comm_interface_poll_rx_queue(void) {
  if (add_rx_message) {
    if (rx_message.cmd == COMPUTER_ACK) {
      queue_drop_comm_tx();
      
      SET_TX_MSG_ACKED;
      comm_status &= ~(1<<RESEND_FLAG);
      resend_count = 0;
    }
    else if (rx_message.cmd == COMPUTER_NACK) {
      comm_status |= (1<<RESEND_FLAG);
    }
    else if (!queue_enqueue_comm_rx(&rx_message)) {
      comm_interface_send_ack();  //Only send ACK if the message has been added to the queue
    }

    add_rx_message = 0;
  }
  
  if (!queue_is_empty_comm_rx()) {
    struct_comm_interface_msg msg;
    queue_dequeue_comm_rx(&msg);
		
    f_ptr_rx(msg);

		reset_rx_status();
	}
}

void comm_interface_send_message(struct_comm_interface_msg message) {
  //PRINTF("SEND ROUTINE\n------------\n");
  //PRINTF("CMD: 0x%02X\n",message.cmd);
  //PRINTF("LEN: %i\n",message.length);
  //PRINTF("DATA: ",message.cmd);
  //PRINTF("CHECKSUM: %i\n",message.checksum);

  tx_data_timeout = 0;

  f_ptr_tx(COMPUTER_PREAMBLE);
	f_ptr_tx(COMPUTER_PREAMBLE);
	f_ptr_tx(message.checksum);
	f_ptr_tx(message.cmd);
	f_ptr_tx(message.length);


	for (uint8_t i=0;i<message.length;i++) {
	  UART_SendByte(LPC_UART0, message.data[i]);
	}
		
	f_ptr_tx(COMPUTER_POSTAMBLE);
}

void  comm_interface_send_ack(void) {
	f_ptr_tx(COMPUTER_PREAMBLE);
	f_ptr_tx(COMPUTER_PREAMBLE);
	f_ptr_tx(COMPUTER_ACK);	//Checksum
	f_ptr_tx(COMPUTER_ACK);	//Cmd
	f_ptr_tx(0);							//Data length
	f_ptr_tx(COMPUTER_POSTAMBLE);
}

void comm_interface_send_nack(void) {
	f_ptr_tx(COMPUTER_PREAMBLE);
	f_ptr_tx(COMPUTER_PREAMBLE);
	f_ptr_tx(COMPUTER_NACK);	//Checksum
	f_ptr_tx(COMPUTER_NACK);	//Cmd
	f_ptr_tx(0);							//Data length
	f_ptr_tx(COMPUTER_POSTAMBLE);	
}

void comm_interface_init(void (*func_ptr_rx)(struct_comm_interface_msg), void (*func_ptr_tx)(uint8_t), unsigned int (*func_ptr_poll_rx)(uint8_t*)) {
	reset_rx_status();
	reset_tx_status();
	
	f_ptr_poll_rx = func_ptr_poll_rx;
	f_ptr_tx = func_ptr_tx;
	f_ptr_rx = func_ptr_rx;
	
	rx_buffer = (uint8_t *)malloc(COMM_INTERFACE_RX_BUF_LEN);
	rx_buffer_start = rx_buffer;

	SET_TX_MSG_ACKED;
}

void comm_interface_set_resend_ptr(void (*func_ptr)(void)) {
  f_ptr_resend = func_ptr;
}

void comm_interface_parse_rx_buffer(void) {
  uint16_t char_len = f_ptr_poll_rx((uint8_t *)(rx_buffer_start+rx_char_count));

  if (char_len != 0) {
    rx_data_timeout = 0;
    rx_char_count += char_len;

    if (rx_char_count > 5) {
      uint16_t preamble_pos = 0;

      #ifdef DEBUG_COMM_INTERFACE
        PRINTF("\nRX_CHAR: %i\n",rx_char_count);
        PRINTF("\nDATA\n----\n");
      #endif

      for (uint16_t char_pos=1;char_pos<rx_char_count;char_pos++) {
        #ifdef DEBUG_COMM_INTERFACE
          PRINTF("0x%02X ",rx_buffer_start[char_pos-1]);
        #endif

        if ((rx_buffer_start[char_pos] == COMPUTER_PREAMBLE) && ((rx_buffer_start[char_pos-1] == COMPUTER_PREAMBLE))) {
          preamble_pos = char_pos-1;

          for (uint16_t post_pos=preamble_pos+4; post_pos<rx_char_count; post_pos++) {
            if (rx_buffer[post_pos] == COMPUTER_POSTAMBLE) {
              //Check if the length of the message is correct
              if (rx_buffer_start[preamble_pos+4] == (post_pos - preamble_pos - 5)) {
                //Extract the data out of the message
                uint8_t calc_checksum = 0;

                uint16_t data_pos = 0;
                for (uint16_t count=preamble_pos+2;count<post_pos;count++) {
                  if ((preamble_pos+2) == count) {
                    rx_message.checksum = rx_buffer_start[count];
                  }
                  else if ((preamble_pos+3) == count) {
                    rx_message.cmd = rx_buffer_start[count];
                    calc_checksum += rx_buffer_start[count];
                  }
                  else if ((preamble_pos+4) == count) {
                    rx_message.length = rx_buffer_start[count];
                    calc_checksum += rx_buffer_start[count];
                  }
                  else {
                    rx_message.data[data_pos++] = rx_buffer_start[count];
                    calc_checksum += rx_buffer_start[count];
                  }
                }

                if (calc_checksum == rx_message.checksum) {
                  add_rx_message = 1;

                  reset_rx_status();
                }
                else {
                  reset_rx_status();
                  comm_interface_send_nack();
                }
              }
            }
          }
        }
      }
    }
  }
}

void comm_interface_1ms_tick(void) {
  counter_1ms++;
	
  if ((counter_1ms % 10) == 0) {
    rx_data_timeout++;
    tx_data_timeout++;
  }
	
	//Reset the RX status if the rx_data_timeout does time out
	if (rx_data_timeout > RX_DATA_TIMEOUT) {
		reset_rx_status();
	}
	
	if (tx_data_timeout > TX_DATA_TIMEOUT) {
		if ((comm_status & (1<<TX_MSG_ACKED)) == 0)
			comm_status |= (1<<RESEND_FLAG);
		
		tx_data_timeout = 0;
	}
}
