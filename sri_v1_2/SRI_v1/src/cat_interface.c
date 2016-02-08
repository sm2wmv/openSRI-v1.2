#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_nvic.h"
#include "lpc17xx_pinsel.h"
#include "board.h"
#include "cat_interface.h"
#include "setting_structs.h"
#include "settings.h"
#include "misc.h"
#include "status.h"
#include "main.h"

//#define CAT_FREQ_DEBUG
//#define CAT_DATA_DEBUG
//#define CAT_COMM_DEBUG

struct_cat_interface cat_interface;

uint16_t counter_cat_interface_tick = 0;
uint16_t counter_cat_interface_poll_tick = 0;

uint16_t cat_radio_rx_buffer_timeout = 0;

//! The radio RX buffer
uint8_t radio_rx_buffer[CAT_INTERFACE_RX_BUFFER_LENGTH];
//! The radio TX buffer
uint8_t radio_tx_buffer[CAT_INTERFACE_TX_BUFFER_LENGTH];
//! The computer RX buffer
uint8_t computer_rx_buffer[CAT_INTERFACE_RX_BUFFER_LENGTH];
//! The radio TX buffer
uint8_t computer_tx_buffer[CAT_INTERFACE_TX_BUFFER_LENGTH];

uint8_t radio_rx_buffer_pos=0;

void cat_interface_init_radio_uart(uint32_t baudrate, uint8_t stopbits, uint8_t parity, uint8_t flow_control, uint8_t jumper_cts_rts) {
  UART_DeInit(LPC_UART1);

  // UART Configuration structure variable
  UART_CFG_Type UARTConfigStruct;
  // UART FIFO configuration Struct variable
  UART_FIFO_CFG_Type UARTFIFOConfigStruct;
  // Pin configuration for UART1
  PINSEL_CFG_Type PinCfg;

  //******** Init UART towards the radio ********
  PinCfg.Funcnum = 1;
  PinCfg.OpenDrain = 0;
  PinCfg.Pinmode = 0;
  PinCfg.Pinnum = 15;
  PinCfg.Portnum = 0;
  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 16;
  PINSEL_ConfigPin(&PinCfg);

  UART_ConfigStructInit(&UARTConfigStruct);

  UARTConfigStruct.Baud_rate = baudrate;

  if (stopbits == SETTING_CAT_STOPBITS_TWO)
    UARTConfigStruct.Stopbits = UART_STOPBIT_2;
  else
    UARTConfigStruct.Stopbits = UART_STOPBIT_1;

  if (parity == SETTING_CAT_PARITY_EVEN)
    UARTConfigStruct.Parity = UART_PARITY_EVEN;
  else if (parity == SETTING_CAT_PARITY_ODD)
    UARTConfigStruct.Parity = UART_PARITY_ODD;
  else
    UARTConfigStruct.Parity = UART_PARITY_NONE;

  cat_interface.jumper_cts_rts = jumper_cts_rts;

  // Initialize UART1 peripheral with given to corresponding parameter
  UART_Init(LPC_UART1, &UARTConfigStruct);

  /* Initialize FIFOConfigStruct to default state:
   *        - FIFO_DMAMode = DISABLE
   *        - FIFO_Level = UART_FIFO_TRGLEV0
   *        - FIFO_ResetRxBuf = ENABLE
   *        - FIFO_ResetTxBuf = ENABLE
   *        - FIFO_State = ENABLE
   */
  UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

  // Initialize FIFO for UART1 peripheral
  UART_FIFOConfig(LPC_UART1, &UARTFIFOConfigStruct);


  // Enable UART Transmit
  UART_TxCmd(LPC_UART1, ENABLE);
}

void cat_interface_init_computer_uart(uint32_t baudrate, uint8_t stopbits, uint8_t parity, uint8_t flow_control, uint8_t jumper_cts_rts) {
  UART_DeInit(LPC_UART2);

  // UART Configuration structure variable
  UART_CFG_Type UARTConfigStruct;
  // UART FIFO configuration Struct variable
  UART_FIFO_CFG_Type UARTFIFOConfigStruct;
  // Pin configuration for UART2
  PINSEL_CFG_Type PinCfg;

  // DeInit NVIC and SCBNVIC
  //NVIC_DeInit();
  //NVIC_SCBDeInit();

  PinCfg.Funcnum = 1;
  PinCfg.OpenDrain = 0;
  PinCfg.Pinmode = 0;
  PinCfg.Pinnum = 10;
  PinCfg.Portnum = 0;
  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 11;
  PINSEL_ConfigPin(&PinCfg);

  UART_ConfigStructInit(&UARTConfigStruct);

  UARTConfigStruct.Baud_rate = baudrate;

  if (stopbits == SETTING_CAT_STOPBITS_TWO)
    UARTConfigStruct.Stopbits = UART_STOPBIT_2;
  else
    UARTConfigStruct.Stopbits = UART_STOPBIT_1;

  if (parity == SETTING_CAT_PARITY_EVEN)
    UARTConfigStruct.Parity = UART_PARITY_EVEN;
  else if (parity == SETTING_CAT_PARITY_ODD)
    UARTConfigStruct.Parity = UART_PARITY_ODD;
  else
    UARTConfigStruct.Parity = UART_PARITY_NONE;

  cat_interface.jumper_cts_rts = jumper_cts_rts;

  // Initialize UART2 peripheral with given to corresponding parameter
  UART_Init(LPC_UART2, &UARTConfigStruct);

  /* Initialize FIFOConfigStruct to default state:
   *        - FIFO_DMAMode = DISABLE
   *        - FIFO_Level = UART_FIFO_TRGLEV0
   *        - FIFO_ResetRxBuf = ENABLE
   *        - FIFO_ResetTxBuf = ENABLE
   *        - FIFO_State = ENABLE
   */
  UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

  // Initialize FIFO for UART3 peripheral
  UART_FIFOConfig(LPC_UART2, &UARTFIFOConfigStruct);


  // Enable UART Transmit
  UART_TxCmd(LPC_UART2, ENABLE);
}

void cat_interface_init(uint32_t baudrate, uint8_t stopbits, uint8_t parity, uint8_t flow_control, uint8_t jumper_cts_rts) {
  if (settings_get_cat_interface_enabled()) {
    cat_interface_init_computer_uart(baudrate, stopbits, parity, flow_control, jumper_cts_rts);
    cat_interface_init_radio_uart(baudrate, stopbits, parity, flow_control, jumper_cts_rts);

    cat_interface.poll_interval = settings_get_cat_poll_rate();
    cat_interface.retrieve_mode = settings_get_cat_retrieve_type();
    cat_interface.radio_model = settings_get_radio_model();
    cat_interface.flags = 0;


    PRINTF_CAT("CAT: Initialized - baud=%i, stopbits=%i, parity=%i, flow=%i, jump=%i\n",baudrate,stopbits,parity,flow_control,jumper_cts_rts);
    PRINTF_CAT("CAT: Poll interval=%i ms - retrieve=%i, model=%i\n",cat_interface.poll_interval, cat_interface.retrieve_mode, cat_interface.radio_model);

    counter_cat_interface_poll_tick = 0;
    radio_rx_buffer_pos = 0;
  }
}

void cat_interface_send_radio_request(void) {
  if (cat_interface.radio_model == RADIO_MODEL_YAESU_FT950) {
    cat_interface_uart_radio_tx_buf(12,(uint8_t *)"IF;OI;FT;FR;");

    radio_rx_buffer_pos = 0;
  }
}

uint8_t cat_interface_parse_cmd(uint8_t *buffer, uint8_t length) {
  if (settings_get_cat_interface_enabled()) {
    #ifdef CAT_DATA_DEBUG
      PRINTF("CAT DATA: PARSE CMD, length: %i\n",length);
    #endif

    if (cat_interface.radio_model == RADIO_MODEL_YAESU_FT950) {
      if (length == 26) {
        if (strncmp((char *)buffer,(char *)"IF",2) == 0) {
          uint32_t freq_vfoA = 0;

          freq_vfoA =  (buffer[5]-48)  * 10000000;
          freq_vfoA += (buffer[6]-48)  * 1000000;
          freq_vfoA += (buffer[7]-48)  * 100000;
          freq_vfoA += (buffer[8]-48)  * 10000;
          freq_vfoA += (buffer[9]-48)  * 1000;
          freq_vfoA += (buffer[10]-48) * 100;
          freq_vfoA += (buffer[11]-48) * 10;
          freq_vfoA += (buffer[12]-48);

          PRINTF_CAT("CAT: FREQ VFO A=%i\n",freq_vfoA);
          PRINTF_CAT("CAT: FREQ VFO A=%i\n",buffer[20]);

          status_set_vfoA_freq(freq_vfoA);
          //Convert from ASCII
          status_set_vfoA_mode(buffer[20]-48);

          return(1);
        }
        else if (strncmp((char *)buffer,(char *)"OI",2) == 0) {
          uint32_t freq_vfoB = 0;

          freq_vfoB =  (buffer[5]-48)  * 10000000;
          freq_vfoB += (buffer[6]-48)  * 1000000;
          freq_vfoB += (buffer[7]-48)  * 100000;
          freq_vfoB += (buffer[8]-48)  * 10000;
          freq_vfoB += (buffer[9]-48)  * 1000;
          freq_vfoB += (buffer[10]-48) * 100;
          freq_vfoB += (buffer[11]-48) * 10;
          freq_vfoB += (buffer[12]-48);

          PRINTF_CAT("CAT: FREQ VFO B=%i\n",freq_vfoB);
          PRINTF_CAT("CAT: MODE VFO B=%c\n",buffer[20]);

          status_set_vfoB_freq(freq_vfoB);
          //Convert from ASCII
          status_set_vfoB_mode(buffer[20]-48);
          return(1);
        }
      }
      else if (length == 3) {
        if (strncmp((char *)buffer,(char *)"FR",2) == 0) {
          uint8_t state = status_get_vfoAB_txrx_state();

          //Remove the state of the RX bits
          state &= 0x03;

          if (buffer[2] == '0')
            state |= (1<<STATUS_VFO_TXRX_VFOA_RX);
          else if (buffer[2] == '1')
            state |= (1<<STATUS_VFO_TXRX_VFOA_MUTE);
          else if (buffer[2] == '4')
            state |= (1<<STATUS_VFO_TXRX_VFOB_RX);
          else if (buffer[2] == '5')
            state |= (1<<STATUS_VFO_TXRX_VFOB_MUTE);

          status_set_vfoAB_txrx_state(state);
        }
        else if (strncmp((char *)buffer,(char *)"FT",2) == 0) {
          uint8_t state = status_get_vfoAB_txrx_state();

          //Remove the state of the TX bits
          state &= 0xFC;

          if (buffer[2] == '0')
            state |= (1<<STATUS_VFO_TXRX_VFOA_TX);
          else if (buffer[2] == '1')
            state |= (1<<STATUS_VFO_TXRX_VFOB_TX);

          status_set_vfoAB_txrx_state(state);
        }
      }
    }
  }

  return(0);
}

uint8_t cat_interface_parse_buffer(uint8_t *buffer, uint8_t length) {
  if (settings_get_cat_interface_enabled()) {
    if (cat_interface.radio_model == RADIO_MODEL_YAESU_FT950) {
      //Check for a ; character
      if (buffer[length-1] == ';') {
        cat_interface_parse_cmd(buffer,length-1);

        #ifdef CAT_DATA_DEBUG
          PRINTF("CAT DATA: BUFFER >> ");
          for (uint8_t i=0;i<length;i++) {
            PRINTF("%c",buffer[i]);
          }
          PRINTF("\n");
        #endif

        radio_rx_buffer_pos = 0;
        return(1);
      }
    }
  }

  return(0);
}

void cat_interface_execute(void) {
  if (settings_get_cat_interface_enabled()) {
    uint8_t radio_rx_byte = 0;
    uint8_t computer_rx_byte = 0;

    if (cat_interface.flags & (1<<CAT_INTERFACE_FLAG_POLL_RADIO)) {
      cat_interface_send_radio_request();

      cat_interface.flags &= ~(1<<CAT_INTERFACE_FLAG_POLL_RADIO);
    }

    if (cat_interface_uart_poll_computer_rx(&computer_rx_byte)) {
      cat_interface_uart_radio_tx(computer_rx_byte);
    }

    if (cat_interface_uart_poll_radio_rx(&radio_rx_byte)) {
      radio_rx_buffer[radio_rx_buffer_pos++] = radio_rx_byte;

      cat_interface_uart_computer_tx(radio_rx_byte);

      cat_interface_parse_buffer((uint8_t *)radio_rx_buffer,radio_rx_buffer_pos);

      if(radio_rx_buffer_pos >= CAT_INTERFACE_RX_BUFFER_LENGTH) {
        radio_rx_buffer_pos = 0;
      }
    }
  }
}

void cat_interface_uart_radio_tx_buf(uint8_t length, uint8_t *data) {
  if (settings_get_cat_interface_enabled()) {
    PRINTF_CAT("UC >> CAT: %s\n",data);
    UART_Send(LPC_UART1, data, length, BLOCKING);
  }
}

uint8_t cat_interface_uart_poll_radio_rx(uint8_t *data) {
  if (settings_get_cat_interface_enabled()) {
    if (!(LPC_UART1->LSR & UART_LSR_RDR)) {
      return(0);
    }
    else {
      cat_radio_rx_buffer_timeout = 0;
      data[0] = UART_ReceiveByte(LPC_UART1);
    }
  }

  return(1);
}

uint8_t cat_interface_uart_poll_computer_rx(uint8_t *data) {
  if (settings_get_cat_interface_enabled()) {
    if (!(LPC_UART2->LSR & UART_LSR_RDR)) {
      return(0);
    }
    else {
      data[0] = UART_ReceiveByte(LPC_UART2);
    }
  }

  return(1);
}

void cat_interface_uart_radio_tx(uint8_t data) {
  if (settings_get_cat_interface_enabled()) {
    UART_SendByte(LPC_UART1,data);
  }
}

void cat_interface_uart_computer_tx_buf(uint8_t length, uint8_t *data) {
  if (settings_get_cat_interface_enabled()) {
    UART_Send(LPC_UART2, data, length, BLOCKING);
  }
}

void cat_interface_uart_computer_tx(uint8_t data) {
  if (settings_get_cat_interface_enabled()) {
    UART_SendByte(LPC_UART2,data);
  }
}

void cat_interface_1ms_tick(void) {
  if (settings_get_cat_interface_enabled()) {
    if (cat_interface.retrieve_mode == SETTINGS_CAT_RETRIEVE_POLLING) {
      if (counter_cat_interface_poll_tick > (cat_interface.poll_interval*10)) {
        cat_interface.flags |= (1<<CAT_INTERFACE_FLAG_POLL_RADIO);

        counter_cat_interface_poll_tick = 0;
      }

      counter_cat_interface_poll_tick++;
    }

    if (cat_radio_rx_buffer_timeout > CAT_INTERFACE_RADIO_RX_CHAR_TIMEOUT) {
      radio_rx_buffer_pos = 0;
      cat_radio_rx_buffer_timeout = 0;
    }

    cat_radio_rx_buffer_timeout++;
    counter_cat_interface_tick++;
  }
}
