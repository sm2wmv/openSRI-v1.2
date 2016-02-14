#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "computer_interface.h"
#include "comm_interface.h"
#include "commands.h"
#include "version.h"
#include "misc.h"
#include "settings.h"
#include "eeprom_m24.h"
#include "status.h"
#include "main.h"
#include "ctrl.h"
#include "cat_interface.h"
#include "winkey.h"
#include "event_handler.h"

static uint8_t computer_inteface_connection_active = 0;
uint16_t counter_last_pong = 0;
uint16_t counter_last_ping = 0;

uint8_t flag_send_ping = 0;

void computer_interface_uart_rx(struct_comm_interface_msg message) {
  if (message.cmd == SRI_CMD_CONNECT) {
    computer_interface_connected_event();

    //When getting a SRI_CMD_CONNECT we know that a connection has been initialized
    //We send the firmware version in return
    comm_interface_add_tx_message(SRI_CMD_CONNECT,5,(uint8_t *)FIRMWARE_VER);

    SETTINGS_UPDATE_TO_COMPUTER(settings.audio);
    SETTINGS_UPDATE_TO_COMPUTER(settings.cw);
    SETTINGS_UPDATE_TO_COMPUTER(settings.misc);
    SETTINGS_UPDATE_TO_COMPUTER(settings.winkey);
    SETTINGS_UPDATE_TO_COMPUTER(settings.ptt);
    SETTINGS_UPDATE_TO_COMPUTER(settings.radio);
    SETTINGS_UPDATE_TO_COMPUTER(settings.display_cw);
    SETTINGS_UPDATE_TO_COMPUTER(settings.display_phone);
    SETTINGS_UPDATE_TO_COMPUTER(settings.display_digital);
    SETTINGS_UPDATE_TO_COMPUTER(settings.digital);

    settings_send_cw_messages();

    comm_interface_add_tx_message(SRI_CMD_UPDATE_UI,0,0);

    //Send status update for the ptt input etc
    status_execute_update();
  }
  else if (message.cmd == SRI_CMD_DISCONNECT) {
    computer_interface_disconnected_event();
  }
  else if (message.cmd == SRI_CMD_GET_FIRMWARE_REV) {
    comm_interface_add_tx_message(SRI_CMD_GET_FIRMWARE_REV,5,(uint8_t *)FIRMWARE_VER);
  }
  else if (message.cmd == SRI_CMD_TEST_COMMAND) {

  }
  else if (message.cmd == SRI_CMD_SEND_SETTINGS) {
    settings_update_from_computer((uint8_t *)&(message.data)+2, (uint16_t)(((uint16_t)(message.data[0])<<8)+message.data[1]), message.length-2);
  }
  else if (message.cmd == SRI_CMD_SEND_STATUS_UPDATE) {
    //Send status updates
    status_execute_update();
  }
  else if (message.cmd == SRI_CMD_CAT_SETTINGS_CHANGED) {
    cat_interface_init(settings_get_cat_baudrate(), settings_get_cat_stopbits(), settings_get_cat_parity(), settings_get_cat_flow_control(), settings_get_cat_jumper_cts_rts());
    status_execute_update();
  }
  else if (message.cmd == SRI_CMD_EEPROM_SAVE) {
    //Save all settings to the EEPROM
    settings_save_to_eeprom();
    settings_save_cw_messages_to_eeprom();
  }
  else if (message.cmd == SRI_CMD_SEND_PING) {
    comm_interface_add_tx_message(SRI_CMD_SEND_PONG, 0, NULL);
  }
  else if (message.cmd == SRI_CMD_SEND_PONG) {
    computer_interface_pong_event();
  }
  else if (message.cmd == SRI_CMD_DEBUG_TEXT) {
    misc_set_debug_state((message.data[0] & (1<<0)) >> 0);
    misc_set_system_debug_state((message.data[0] & (1<<1)) >> 1);
    misc_set_cat_debug_state((message.data[0] & (1<<2)) >> 2);
    misc_set_winkey_debug_state((message.data[0] & (1<<3)) >> 3);
    misc_set_sequencer_debug_state((message.data[0] & (1<<4)) >> 4);
  }
  else if (message.cmd == SRI_CMD_DISPLAY_RGB_CHANGED) {
    PRINTF("RED: %i - GREEN: %i - BLUE %i\n",settings_get_backlight_rgb_red(),settings_get_backlight_rgb_green(),settings_get_backlight_rgb_blue());
    ctrl_backlight_set_rgb(settings_get_backlight_rgb_red(),settings_get_backlight_rgb_green(),settings_get_backlight_rgb_blue());
  }
  else if (message.cmd == SRI_CMD_AUDIO_SETTINGS_CHANGED) {
    event_handler_execute_audio_change(event_handler_get_ptt_status());
  }
  else if (message.cmd == SRI_CMD_WINKEY) {
    if (message.data[0] == SRI_CMD_SUB_WINKEY_SETTINGS_CHANGED) {
      winkey_setting_changed(message.data[1]);
    }
  }
  else if (message.cmd == SRI_CMD_CW_MESSAGE) {
    if (message.data[0] == SRI_SUB_CMD_SET_CW_MESSAGE) {
      settings_set_cw_message(message.data[1], message.length-2,(uint8_t *)(message.data+2));
    }
    else if (message.data[0] == SRI_SUB_CMD_PLAY_CW_MESSAGE) {
      if (status_get_vfo_mode_type() == STATUS_RADIO_MODE_TYPE_CW) {
        PRINTF("WINKEY >> SENDING CW MSG %i\n",message.data[0]);

        winkey_send_message(strlen((char *)(settings_get_cw_message(message.data[1]))),settings_get_cw_message(message.data[1]));
      }
    }
    else if (message.data[0] == SRI_SUB_CMD_STOP_CW_MESSAGE) {
      winkey_clear_buffer();
    }
  }
}

void computer_interface_uart_tx(uint8_t data) {
  UART_SendByte(LPC_UART0,data);
}

uint32_t computer_interface_uart_poll_rx(uint8_t *buffer) {
  return(UART_Receive(LPC_UART0, buffer, COMM_INTERFACE_RX_BUF_LEN/2, NONE_BLOCKING));
}

uint8_t computer_interface_connection_status(void) {
  return(computer_inteface_connection_active);
}

void computer_interface_execute() {
  if (flag_send_ping) {
    //PRINTF("SENDING PING\n");
    if (computer_interface_connection_status())
      comm_interface_add_tx_message(SRI_CMD_SEND_PING,0,NULL);

    flag_send_ping = 0;
  }

}

void computer_interface_disconnected_event(void) {
  PRINTF("DISCONNECTED\n");

  ctrl_led_aux_off();
  computer_inteface_connection_active = 0;
  comm_interface_reset_all();
}

void computer_interface_pong_event(void) {
//  PRINTF("PONG EVENT\n");

  counter_last_pong = 0;
}

void computer_interface_connected_event(void) {
  PRINTF("CONNECTED\n");
  ctrl_led_aux_on();

  computer_inteface_connection_active = 1;
  counter_last_pong = 0;
  counter_last_ping = 0;
}

void computer_interface_1ms_tick(void) {
  if (computer_interface_connection_status()) {
    if (counter_last_pong > SRI_PING_TIMEOUT_LIMIT) {
      computer_interface_disconnected_event();
    }

    if (counter_last_ping >= SRI_PING_INTERVAL) {
      flag_send_ping = 1;

      counter_last_ping = 0;
    }

    counter_last_pong++;
    counter_last_ping++;
  }
}
