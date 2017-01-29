#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "board.h"
#include "winkey.h"
#include "settings.h"
#include "status.h"
#include "misc.h"
#include "sc16is7x0.h"

static volatile uint16_t ms_counter = 0;
static volatile uint8_t counter_last_rx_char = 0;

static volatile uint8_t winkey_state = 0;

uint8_t winkey_computer_rx_buffer[WINKEY_RX_BUF_SIZE];
uint8_t winkey_computer_tx_buffer[WINKEY_TX_BUF_SIZE];

void winkey_init(void) {
  PRINTF_WK("WK: INIT started\n");
  winkey_send_char(WINKEY_CMD_NULL);
  winkey_send_char(WINKEY_CMD_NULL);
  winkey_send_char(WINKEY_CMD_NULL);

  //Make sure that we have reset the RX FIFO
  //This might otherwise cause problems when the
  //SRI interface is restarted without cutting the power
  //since the winkey chip might have sent scrap characters
  //which mess up the ECHO command

  //Perform an echo test
/*  winkey_send_char(WINKEY_CMD_ADMIN);
  winkey_send_char(WINKEY_ADMIN_CMD_ECHO);
  winkey_send_char(0x55);
*/
  winkey_send_char(WINKEY_CMD_ADMIN);
  winkey_send_char(WINKEY_ADMIN_CMD_HOST_OPEN);

  winkey_state = winkey_init_sequence(WINKEY_STATE_REV_REPLY);
}

uint8_t winkey_get_initialized(void) {
  if (winkey_state == WINKEY_STATE_NORMAL)
    return(1);

  return(0);
}

void winkey_send_char(uint8_t data) {
  PRINTF_WK("UC >> WK: 0x%02X\n",data);

  sc16is7x0_write_char(data);
}

void winkey_send_data(uint8_t length, uint8_t *data) {
  PRINTF_WK("UC >> WK: ",data);

  for (uint8_t i=0;i<length;i++) {
    PRINTF_WK("0x%02X ",data[i]);

    sc16is7x0_write_char(data[i]);
  }

  PRINTF_WK("\n");
}

void winkey_reset(void) {
  winkey_send_char(WINKEY_CMD_NULL);
  winkey_send_char(WINKEY_CMD_NULL);
  winkey_send_char(WINKEY_CMD_NULL);

  winkey_send_char(WINKEY_CMD_ADMIN);
  winkey_send_char(WINKEY_ADMIN_CMD_RESET);
}

uint8_t winkey_init_sequence(uint8_t state) {
  PRINTF_WK("WK: INIT SEQUENCE: 0x%02X\n",state);

  if (state == WINKEY_STATE_ECHO_REQ) {
    winkey_send_char(WINKEY_CMD_ADMIN);
    winkey_send_char(WINKEY_ADMIN_CMD_HOST_OPEN);

    return(WINKEY_STATE_REV_REPLY);
  }
  else if (state == WINKEY_STATE_REV_REPLY) {
    //Setup the settings we wish to have the for Winkey in host mode
    //Set the sidetone parameters
    winkey_send_char(WINKEY_CMD_SIDETONE_FREQ);
    winkey_send_char(0x04);

    //Set the WEIGHT
    winkey_send_char(WINKEY_CMD_WEIGHT);
    winkey_send_char(settings_get_winkey_weight());

    //Set the LEAD/TAIL times, TODO: Think about how we should do this
    winkey_send_char(WINKEY_CMD_PTT_LEAD_TAIL);
    winkey_send_char(settings_get_ptt_radio_pre_delay()+settings_get_winkey_lead_time()/10);
    winkey_send_char(settings_get_winkey_tail_time()/10);

    //Setup the speed pot range
    winkey_send_char(WINKEY_CMD_SPEED_POT_SETUP);
    winkey_send_char(settings_get_winkey_speed_pot_min());
    winkey_send_char(settings_get_winkey_speed_pot_max()-settings_get_winkey_speed_pot_min());
    winkey_send_char(0);

    //Send the Winkey pin configuration
    winkey_send_char(WINKEY_CMD_PIN_CONF);
    winkey_send_char(settings_get_winkey_pincfg());

    //Set the correct keyer mode
    winkey_send_char(WINKEY_CMD_WINKEY2_MODE);
    winkey_send_char(settings_get_winkey_modereg());

    //Make sure we don't have anything keyed up
    winkey_send_char(WINKEY_CMD_KEY_IMMIDIATE);
    winkey_send_char(0x00);

    //Set the farnsworth configuration
    winkey_send_char(WINKEY_CMD_FARNSWORTH);
    winkey_send_char(settings_get_winkey_farnsworth());

    //Send first extension
    winkey_send_char(WINKEY_CMD_FIRST_EXTENSION);
    winkey_send_char(settings_get_winkey_first_extension());

    //Key compensation
    winkey_send_char(WINKEY_CMD_KEY_COMPENSATION);
    winkey_send_char(settings_get_winkey_key_compensation());

    //Paddle switchpoint
    winkey_send_char(WINKEY_CMD_PADDE_SWITCHPOINT);
    winkey_send_char(settings_get_winkey_paddle_memory_sw_point());

    //DIH DAH ratio
    winkey_send_char(WINKEY_CMD_DIH_DAH_RATIO);
    winkey_send_char(settings_get_winkey_dih_dah_ratio());

    winkey_send_char(WINKEY_CMD_GET_SPEED_POT);

    //Set default value of Winkey speed
    winkey_send_char(WINKEY_CMD_SPEED);
    winkey_send_char(settings_get_winkey_default_speed());

    status_set_winkey_pot_speed(settings_get_winkey_default_speed());

    winkey_state = WINKEY_STATE_NORMAL;

    return(WINKEY_STATE_NORMAL);
  }

  PRINTF_WK("WK: RETURN STATE FAILED\n");
  return(WINKEY_STATE_FAILED);
}

uint8_t winkey_init_failed(uint8_t state) {
  PRINTF_WK("WK: INIT FAILED 0x%02X\n",state);

  if (state == WINKEY_STATE_ECHO_REQ) {
    PRINTF_WK("WK: INIT FAILED, WRONG ECHO\n");
  }
  else if (state == WINKEY_STATE_REV_REPLY) {
    PRINTF_WK("WK: INIT FAILED, WRONG REVISION\n");
  }

  return(WINKEY_STATE_FAILED);
}

void winkey_execute(void) {
  if (winkey_state == WINKEY_STATE_ECHO_REQ) {
    int16_t winkey_rx_data = winkey_poll_char();
    if (winkey_rx_data != -1) {
      if (winkey_rx_data == 0x55)
        winkey_state = winkey_init_sequence(WINKEY_STATE_ECHO_REQ);
      else {
        PRINTF_WK("WK >> UC: INIT FAILED #1, WRONG ECHO 0x%02X\n",winkey_rx_data);
        winkey_state = winkey_init_failed(WINKEY_STATE_ECHO_REQ);
      }
    }
  }
  else if (winkey_state == WINKEY_STATE_REV_REPLY) {
    int16_t winkey_rx_data = winkey_poll_char();

    if (winkey_rx_data != -1) {
      PRINTF_WK("WK >> UC: REV -> %i\n",winkey_rx_data);

      if (winkey_rx_data == WINKEY_REV_NR)
        winkey_state = winkey_init_sequence(WINKEY_STATE_REV_REPLY);
      else
        winkey_state = winkey_init_failed(WINKEY_STATE_REV_REPLY);
    }
  }
  else if (winkey_state == WINKEY_STATE_NORMAL) {
    uint32_t computer_rx_len = winkey_receive_from_computer((uint8_t *)winkey_computer_rx_buffer,WINKEY_RX_BUF_SIZE);
    int16_t winkey_rx_data = winkey_poll_char();

    if (computer_rx_len != 0) {
      winkey_send_data(computer_rx_len, winkey_computer_rx_buffer);
    }

    if (winkey_rx_data != -1) {
      if ((winkey_rx_data & 0xc0) == 0xc0) {
        if (winkey_rx_data & (1<<0))
          PRINTF_WK("WK >> UC: STATUS -> XOFF\n");
        if (winkey_rx_data & (1<<1))
          PRINTF_WK("WK >> UC: STATUS -> BREAKIN\n");
        if (winkey_rx_data & (1<<2))
          PRINTF_WK("WK >> UC: STATUS -> BUSY TX CW\n");
        if (winkey_rx_data & (1<<3))
          PRINTF_WK("WK >> UC: STATUS -> KEYDOWN\n");
        if (winkey_rx_data == 0xC0)
          PRINTF_WK("WK >> UC: STATUS -> NO TX ACTIVE\n");
      }
      else if ((winkey_rx_data & 0xc0) == 0x80) {
        //Do nothing, we send the actual speed each WINKEY_POT_SPEED_INTERVAL
      }
      else {
        PRINTF_WK("WK >> UC: SENDING -> %c\n",winkey_rx_data);
        status_set_winkey_transmitted_character(winkey_rx_data);
      }

      winkey_send_to_computer(1,(uint8_t *)&winkey_rx_data);
    }
  }
}

uint32_t winkey_receive_from_computer(uint8_t *buffer, uint16_t length) {
  return(UART_Receive(LPC_UART3, buffer, length, NONE_BLOCKING));
}

void winkey_send_to_computer(uint8_t length, uint8_t *data) {
  UART_Send(LPC_UART3, data, length, BLOCKING);
}

int16_t winkey_poll_char(void) {
  int16_t rx_data = sc16is7x0_poll_char();

  if (rx_data != -1) {
    return(rx_data);
  }

  return(-1);
}

void winkey_setting_changed(uint8_t cmd) {
  PRINTF_WK("WK: SETTING CHANGED -> 0x%02X\n",cmd);

  if (cmd == WINKEY_CMD_SIDETONE_FREQ) {
    //Setup the settings we wish to have the for Winkey in host mode
    //Set the sidetone parameters
    winkey_send_char(WINKEY_CMD_SIDETONE_FREQ);
    winkey_send_char(0x04);
  }
  else if (cmd == WINKEY_CMD_WEIGHT) {
    //Set the WEIGHT
    winkey_send_char(WINKEY_CMD_WEIGHT);
    winkey_send_char(settings_get_winkey_weight());
  }
  else if (cmd == WINKEY_CMD_PTT_LEAD_TAIL) {
    //Set the LEAD/TAIL times, TODO: Think about how we should do this
    winkey_send_char(WINKEY_CMD_PTT_LEAD_TAIL);
    winkey_send_char(settings_get_ptt_radio_pre_delay()+settings_get_winkey_lead_time()/10);
    winkey_send_char(settings_get_winkey_tail_time()/10);
  }
  else if (cmd == WINKEY_CMD_SPEED_POT_SETUP) {
    //Setup the speed pot range
    winkey_send_char(WINKEY_CMD_SPEED_POT_SETUP);
    winkey_send_char(settings_get_winkey_speed_pot_min());
    winkey_send_char(settings_get_winkey_speed_pot_max()-settings_get_winkey_speed_pot_min());
    winkey_send_char(0);
  }
  else if (cmd == WINKEY_CMD_PIN_CONF) {
    //Send the Winkey pin configuration
    winkey_send_char(WINKEY_CMD_PIN_CONF);
    winkey_send_char(settings_get_winkey_pincfg());
  }
  else if (cmd == WINKEY_CMD_WINKEY2_MODE) {
    //Set the correct keyer mode
    winkey_send_char(WINKEY_CMD_WINKEY2_MODE);
    winkey_send_char(settings_get_winkey_modereg());
  }
  else if (cmd == WINKEY_CMD_KEY_IMMIDIATE) {
    //Make sure we don't have anything keyed up
    winkey_send_char(WINKEY_CMD_KEY_IMMIDIATE);
    winkey_send_char(0x00);
  }
  else if (cmd == WINKEY_CMD_FARNSWORTH) {
    //Set the farnsworth configuration
    winkey_send_char(WINKEY_CMD_FARNSWORTH);
    winkey_send_char(settings_get_winkey_farnsworth());
  }
  else if (cmd == WINKEY_CMD_FIRST_EXTENSION) {
    //Send first extension
    winkey_send_char(WINKEY_CMD_FIRST_EXTENSION);
    winkey_send_char(settings_get_winkey_first_extension());
  }
  else if (cmd == WINKEY_CMD_KEY_COMPENSATION) {
    //Key compensation
    winkey_send_char(WINKEY_CMD_KEY_COMPENSATION);
    winkey_send_char(settings_get_winkey_key_compensation());
  }
  else if (cmd == WINKEY_CMD_PADDE_SWITCHPOINT) {
    //Paddle switchpoint
    winkey_send_char(WINKEY_CMD_PADDE_SWITCHPOINT);
    winkey_send_char(settings_get_winkey_paddle_memory_sw_point());
  }
  else if (cmd == WINKEY_CMD_DIH_DAH_RATIO) {
    //DIH DAH ratio
    winkey_send_char(WINKEY_CMD_DIH_DAH_RATIO);
    winkey_send_char(settings_get_winkey_dih_dah_ratio());
  }
  else {
    PRINTF_WK("WINKEY: SETTING UNKNOWN\n");
  }
}

void winkey_set_speed(uint8_t speed) {
  if (winkey_get_initialized()) {
    winkey_send_char(WINKEY_CMD_SPEED);
    winkey_send_char(speed);

    status_set_winkey_pot_speed(speed);
  }
}

void winkey_dec_speed(void) {
  if (winkey_get_initialized()) {
    uint8_t new_speed = status_get_winkey_pot_speed() - 1;

    if (new_speed >= settings_get_winkey_speed_pot_min()) {
      PRINTF_WK("WK: SPEED DEC TO: %i\n", new_speed);

      winkey_send_char(WINKEY_CMD_SPEED);
      winkey_send_char(new_speed);

      status_set_winkey_pot_speed(new_speed);
    }
    else {
      PRINTF_WK("WK: SPEED AT MIN\n");
    }
  }
}

void winkey_inc_speed(void) {
  if (winkey_get_initialized()) {
    uint8_t new_speed = status_get_winkey_pot_speed() + 1;

    if (new_speed <= settings_get_winkey_speed_pot_max()) {
      PRINTF_WK("WK: SPEED INC TO: %i\n", new_speed);

      winkey_send_char(WINKEY_CMD_SPEED);
      winkey_send_char(new_speed);

      status_set_winkey_pot_speed(new_speed);
    }
    else {
      PRINTF_WK("WK: SPEED AT MAX\n");
    }
  }
}

void winkey_reset_speed(void) {
  if (winkey_get_initialized()) {
    winkey_send_char(WINKEY_CMD_SPEED);
    winkey_send_char(0x00);
  }
}

void winkey_clear_buffer(void) {
  if (winkey_get_initialized()) {
    winkey_send_char(WINKEY_CMD_CLEAR_BUFFER);
  }
}

void winkey_send_message(uint8_t length, uint8_t *message) {
  if (winkey_get_initialized()) {
    if (length <= 50)
      winkey_send_data(length, message);
    else
      winkey_send_data(50, message);
  }
}

void winkey_1ms_tick(void) {
  counter_last_rx_char++;
  ms_counter++;
}
