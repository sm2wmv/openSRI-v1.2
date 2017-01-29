#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lpc17xx_exti.h"

#include "sc16is7x0.h"
#include "event_handler.h"
#include "main.h"
#include "misc.h"
#include "settings.h"
#include "sequencer.h"
#include "display.h"
#include "status.h"
#include "audio.h"
#include "ctrl.h"
#include "board.h"
#include "winkey.h"

static volatile uint16_t counter_event_10us = 0;
static volatile uint16_t counter_event_1ms = 0;

volatile struct_event_status event_status;

struct_radio_status prev_radio_status;
static uint8_t prev_radio_mode_type = 255;

uint8_t event_handler_get_ptt_status(void) {
  return(event_status.curr_ptt_input);
}

void event_handler_ptt_radio_activate(void) {
  PRINTF("EVENT_HANDLER: RADIO PTT ACTIVE\n");

  if (status_get_vfo_mode_type() == STATUS_RADIO_MODE_TYPE_CW) {
    if ((settings_get_radio_cw_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1) || (settings_get_radio_cw_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1_2)) {
      ctrl_radio_ptt_1_set();
    }

    if ((settings_get_radio_cw_ptt() == SETTINGS_PTT_RADIO_OUTPUT_2) || (settings_get_radio_cw_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1_2)) {
      ctrl_radio_ptt_2_set();
    }

    if (settings_get_radio_cw_ptt() == SETTINGS_PTT_RADIO_OUTPUT_DISABLED) {
      ctrl_radio_ptt_1_clr(); //Make sure the output is off
      ctrl_radio_ptt_2_clr(); //Make sure the output is off
    }
  }
  else if (status_get_vfo_mode_type() == STATUS_RADIO_MODE_TYPE_PHONE) {
    if ((settings_get_radio_phone_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1) || (settings_get_radio_phone_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1_2)) {
      ctrl_radio_ptt_1_set();
    }

    if ((settings_get_radio_phone_ptt() == SETTINGS_PTT_RADIO_OUTPUT_2) || (settings_get_radio_phone_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1_2)) {
      ctrl_radio_ptt_2_set();
    }

    if (settings_get_radio_phone_ptt() == SETTINGS_PTT_RADIO_OUTPUT_DISABLED) {
      ctrl_radio_ptt_1_clr(); //Make sure the output is off
      ctrl_radio_ptt_2_clr(); //Make sure the output is off
    }
  }
  else if (status_get_vfo_mode_type() == STATUS_RADIO_MODE_TYPE_DIGITAL) {
    if ((settings_get_radio_digital_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1) || (settings_get_radio_digital_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1_2)) {
      ctrl_radio_ptt_1_set();
    }

    if ((settings_get_radio_digital_ptt() == SETTINGS_PTT_RADIO_OUTPUT_2) || (settings_get_radio_digital_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1_2)) {
      ctrl_radio_ptt_2_set();
    }

    if (settings_get_radio_digital_ptt() == SETTINGS_PTT_RADIO_OUTPUT_DISABLED) {
      ctrl_radio_ptt_1_clr(); //Make sure the output is off
      ctrl_radio_ptt_2_clr(); //Make sure the output is off
    }
  }
}

void event_handler_ptt_radio_deactivate(void) {
  PRINTF("EVENT_HANDLER: RADIO PTT DEACTIVE\n");

/*  if (status_get_vfo_mode_type() == STATUS_RADIO_MODE_TYPE_CW) {
    if ((settings_get_radio_cw_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1) || (settings_get_radio_cw_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1_2)) {
      ctrl_radio_ptt_1_clr();
    }

    if ((settings_get_radio_cw_ptt() == SETTINGS_PTT_RADIO_OUTPUT_2) || (settings_get_radio_cw_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1_2)) {
      ctrl_radio_ptt_2_clr();
    }

    if (settings_get_radio_cw_ptt() == SETTINGS_PTT_RADIO_OUTPUT_DISABLED) {
      ctrl_radio_ptt_1_clr(); //Make sure the output is off
      ctrl_radio_ptt_2_clr(); //Make sure the output is off
    }
  }
  else if (status_get_vfo_mode_type() == STATUS_RADIO_MODE_TYPE_PHONE) {
    if ((settings_get_radio_phone_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1) || (settings_get_radio_phone_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1_2)) {
      ctrl_radio_ptt_1_clr();
    }

    if ((settings_get_radio_phone_ptt() == SETTINGS_PTT_RADIO_OUTPUT_2) || (settings_get_radio_phone_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1_2)) {
      ctrl_radio_ptt_2_clr();
    }

    if (settings_get_radio_phone_ptt() == SETTINGS_PTT_RADIO_OUTPUT_DISABLED) {
      ctrl_radio_ptt_1_clr(); //Make sure the output is off
      ctrl_radio_ptt_2_clr(); //Make sure the output is off
    }
  }
  else if (status_get_vfo_mode_type() == STATUS_RADIO_MODE_TYPE_DIGITAL) {
    if ((settings_get_radio_digital_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1) || (settings_get_radio_digital_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1_2)) {
      ctrl_radio_ptt_1_clr();
    }

    if ((settings_get_radio_digital_ptt() == SETTINGS_PTT_RADIO_OUTPUT_2) || (settings_get_radio_digital_ptt() == SETTINGS_PTT_RADIO_OUTPUT_1_2)) {
      ctrl_radio_ptt_2_clr();
    }

    if (settings_get_radio_digital_ptt() == SETTINGS_PTT_RADIO_OUTPUT_DISABLED) {
      ctrl_radio_ptt_1_clr(); //Make sure the output is off
      ctrl_radio_ptt_2_clr(); //Make sure the output is off
    }
  }*/

  //I don't see where we wish to have one PTT on during a PTT deactive
  ctrl_radio_ptt_1_clr(); //Make sure the output is off
  ctrl_radio_ptt_2_clr(); //Make sure the output is off
}

void event_handler_ptt_amp_activate(void) {
  PRINTF("EVENT_HANDLER: AMP PTT ACTIVE\n");
  if (settings_get_amplifier_enabled()) {
    ctrl_ptt_amp_set();
  }
}

void event_handler_ptt_amp_deactivate(void) {
  PRINTF("EVENT_HANDLER: AMP PTT DEACTIVE\n");

  ctrl_ptt_amp_clr();
}

void event_handler_ptt_inhibit_activate(void) {
  if (settings_get_inhibit_enabled()) {
    PRINTF("EVENT_HANDLER: INHIBIT PTT ACTIVE\n");
    if (settings_get_inhibit_inverted())
      ctrl_radio_inhibit_clr();
    else
      ctrl_radio_inhibit_set();
  }
  else
    ctrl_radio_inhibit_clr(); //CLR is the default state
}

void event_handler_ptt_inhibit_deactivate(void) {
  if (settings_get_inhibit_enabled()) {
    PRINTF("EVENT_HANDLER: INHIBIT PTT DEACTIVE\n");

    if (settings_get_inhibit_inverted())
      ctrl_radio_inhibit_set();
    else
      ctrl_radio_inhibit_clr();
  }
  else
    ctrl_radio_inhibit_clr(); //CLR is the default state
}

void event_handler_ptt_ant_activate(void) {
  PRINTF("EVENT_HANDLER: ANT PTT ACTIVE\n");
  ctrl_ant_set();
}

void event_handler_ptt_ant_deactivate(void) {
  PRINTF("EVENT_HANDLER: ANT PTT DEACTIVE\n");
  ctrl_ant_clr();
}

void event_handler_radio_status_changed(struct_radio_status *curr_status) {
  if (curr_status->vfoA_mode != prev_radio_status.vfoA_mode) {
    event_handler_execute_audio_change(event_status.curr_ptt_input);
    display_update();
  }

  if (curr_status->vfoB_mode != prev_radio_status.vfoB_mode) {
    event_handler_execute_audio_change(event_status.curr_ptt_input);
    display_update();
  }

  if (curr_status->vfoAB_tx_rx_state != prev_radio_status.vfoAB_tx_rx_state) {
    event_handler_execute_audio_change(event_status.curr_ptt_input);
    display_update();
  }

  if (curr_status->vfoA_freq != prev_radio_status.vfoA_freq) {
    display_update();
  }

  if (curr_status->vfoB_freq != prev_radio_status.vfoB_freq) {
    display_update();
  }

  uint8_t curr_mode_type = status_get_vfo_mode_type();
  if (prev_radio_mode_type != curr_mode_type) {
    //Disable the Interrupts for FSK and CW
    NVIC_DisableIRQ(EINT1_IRQn);
    NVIC_DisableIRQ(EINT2_IRQn);
    NVIC_DisableIRQ(EINT3_IRQn);

    if (curr_mode_type == STATUS_RADIO_MODE_TYPE_CW) {

      PRINTF("Mode change: CW, init SC16IS740 for CW\n");
      sc16is7x0_init_winkey();
      //Route the SC16IS740 serial data to the winkey chip
      ctrl_fsk_tx_enable_clr();

      //Initialize the winkey chip again
      winkey_init();

      if ((settings_get_cw_input_source() & (1<<SETTING_CW_INPUT_SOURCE_USB_DTR)) != 0) {
        LPC_GPIOINT->IO2IntClr=(1<<11);
        NVIC_EnableIRQ(EINT1_IRQn);
      }

      if ((settings_get_cw_input_source() & (1<<SETTING_CW_INPUT_SOURCE_WINKEY)) != 0) {
        LPC_GPIOINT->IO2IntClr=(1<<13);
        NVIC_EnableIRQ(EINT3_IRQn);
      }

      ctrl_radio_cw_clr();
      ctrl_led_cw_off();
    }
    else if (curr_mode_type == STATUS_RADIO_MODE_TYPE_DIGITAL) {
      PRINTF("Mode change: Digital, init SC16IS740 for FSK\n");
      //Route the SC16IS740 serial data for FSK output
      ctrl_fsk_tx_enable_set();
      sc16is7x0_init_fsk(settings_get_digital_baudrate_divisor(), settings_get_digital_parity(), settings_get_digital_stopbits(), settings_get_digital_bitlength());
    }

    prev_radio_mode_type = curr_mode_type;
  }

  memcpy(&prev_radio_status, curr_status,sizeof(curr_status));
}

void event_handler_execute_audio_change(uint8_t ptt_state) {
  if (ptt_state != 0) {
    //Checks the PTT inputs and sets the proper audio route, this always determine the
    //priority of the different states
    if (ptt_state & (1<<STATUS_PTT_INPUT_FOOTSWITCH))
      audio_set_state(status_get_vfo_mode_type(),SETTING_AUDIO_ROUTE_FOOTSWITCH);
    else if (ptt_state & (1<<STATUS_PTT_INPUT_WK))
      audio_set_state(status_get_vfo_mode_type(),SETTING_AUDIO_ROUTE_WINKEY);
    else if (ptt_state & (1<<STATUS_PTT_INPUT_VOX_SENSE))
      audio_set_state(status_get_vfo_mode_type(),SETTING_AUDIO_ROUTE_VOX);
    else if (ptt_state & (1<<STATUS_PTT_INPUT_WK_DTR))
      audio_set_state(status_get_vfo_mode_type(),SETTING_AUDIO_ROUTE_COMPUTER);
    else if (ptt_state & (1<<STATUS_PTT_INPUT_WK_RTS))
      audio_set_state(status_get_vfo_mode_type(),SETTING_AUDIO_ROUTE_COMPUTER);
    else if (ptt_state & (1<<STATUS_PTT_INPUT_FSKCW_RTS))
      audio_set_state(status_get_vfo_mode_type(),SETTING_AUDIO_ROUTE_COMPUTER);
    else
      audio_set_state(status_get_vfo_mode_type(),SETTING_AUDIO_ROUTE_RX);
  }
  else {
    audio_set_state(status_get_vfo_mode_type(),SETTING_AUDIO_ROUTE_RX);
  }
}

void event_handler_execute_ptt_change(uint8_t prev_state, uint8_t curr_state) {
  if ((prev_state == 0) && (curr_state != 0)) { //Activate the sequencer
    PRINTF("EVENT_HANDLER_PTT_CHANGE: ACTIVATE\n");
    ctrl_led_ptt_on();
    sequencer_activate();

    display_ptt_active();
    event_handler_execute_audio_change(curr_state);
  }
  else if ((prev_state != 0) && (curr_state == 0)) { //Deactivate sequencer
    PRINTF("EVENT_HANDLER_PTT_CHANGE: DEACTIVATE\n");
    ctrl_led_ptt_off();
    sequencer_deactivate();

    display_ptt_deactive();
    event_handler_execute_audio_change(curr_state);
  }
  else {
    //Do nothing
    PRINTF("EVENT_HANDLER_PTT_CHANGE: DO NOTHING\n");
  }
}

void event_handler_execute(void) {
  if (event_status.flags & (1<<EVENT_HANDLER_CHECK_PTT_INPUTS)) {
    if (settings_get_ptt_input_enabled() & (1<<SETTING_PTT_INPUT_FOOTSWITCH)) {
      event_status.temp_ptt_input = ((PTT_IN_PORT->FIOPIN & (1<<PTT_IN)) >> PTT_IN);

      if (event_status.temp_ptt_input)
        event_status.curr_ptt_input &= ~(1<<STATUS_PTT_INPUT_FOOTSWITCH);
      else
        event_status.curr_ptt_input |= (1<<STATUS_PTT_INPUT_FOOTSWITCH);
    }

    if (settings_get_ptt_input_enabled() & (1<<SETTING_PTT_INPUT_WK)) {
      event_status.temp_ptt_input = ((WINKEY_PTT_PORT->FIOPIN & (1<<WINKEY_PTT)) >> WINKEY_PTT);

      if (event_status.temp_ptt_input) {
        event_status.curr_ptt_input |= (1<<STATUS_PTT_INPUT_WK);
      }
      else
        event_status.curr_ptt_input &= ~(1<<STATUS_PTT_INPUT_WK);
    }

    if (settings_get_ptt_input_enabled() & (1<<SETTING_PTT_INPUT_FSKCW_RTS)) {
      event_status.temp_ptt_input = ((FT4232_FSKCW_PTT_PORT->FIOPIN & (1<<FT4232_FSKCW_PTT)) >> FT4232_FSKCW_PTT);

      if (settings_get_ptt_input_invert() & (1<<SETTING_PTT_INPUT_FSKCW_RTS)) {
        if (event_status.temp_ptt_input)
          event_status.curr_ptt_input |= (1<<STATUS_PTT_INPUT_FSKCW_RTS);
        else
          event_status.curr_ptt_input &= ~(1<<STATUS_PTT_INPUT_FSKCW_RTS);
      }
      else {
        if (event_status.temp_ptt_input)
          event_status.curr_ptt_input &= ~(1<<STATUS_PTT_INPUT_FSKCW_RTS);
        else
          event_status.curr_ptt_input |= (1<<STATUS_PTT_INPUT_FSKCW_RTS);
      }
    }

    if (settings_get_ptt_input_enabled() & (1<<SETTING_PTT_INPUT_WK_RTS)) {
      event_status.temp_ptt_input = ((FT4232_WK_RTS_PORT->FIOPIN & (1<<FT4232_WK_RTS)) >> FT4232_WK_RTS);

      if (settings_get_ptt_input_invert() & (1<<SETTING_PTT_INPUT_WK_RTS)) {
        if (event_status.temp_ptt_input)
          event_status.curr_ptt_input |= (1<<STATUS_PTT_INPUT_WK_RTS);
        else
          event_status.curr_ptt_input &= ~(1<<STATUS_PTT_INPUT_WK_RTS);
      }
      else {
        if (event_status.temp_ptt_input)
          event_status.curr_ptt_input &= ~(1<<STATUS_PTT_INPUT_WK_RTS);
        else
          event_status.curr_ptt_input |= (1<<STATUS_PTT_INPUT_WK_RTS);
      }
    }

    if (settings_get_ptt_input_enabled() & (1<<SETTING_PTT_INPUT_WK_DTR)) {
      event_status.temp_ptt_input = ((FT4232_WK_DTR_PORT->FIOPIN & (1<<FT4232_WK_DTR)) >> FT4232_WK_DTR);

      if (settings_get_ptt_input_invert() & (1<<SETTING_PTT_INPUT_WK_DTR)) {
        if (event_status.temp_ptt_input)
          event_status.curr_ptt_input |= (1<<STATUS_PTT_INPUT_WK_DTR);
        else
          event_status.curr_ptt_input &= ~(1<<STATUS_PTT_INPUT_WK_DTR);
      }
      else {
        if (event_status.temp_ptt_input)
          event_status.curr_ptt_input &= ~(1<<STATUS_PTT_INPUT_WK_DTR);
        else
          event_status.curr_ptt_input |= (1<<STATUS_PTT_INPUT_WK_DTR);
      }
    }

    if (settings_get_ptt_input_enabled() & (1<<SETTING_PTT_INPUT_VOX_SENSE)) {
      event_status.temp_ptt_input = ((PTT_IN_PORT->FIOPIN & (1<<PTT_IN)) >> PTT_IN);

      if (settings_get_ptt_input_invert() & (1<<SETTING_PTT_INPUT_VOX_SENSE)) {
        if (event_status.temp_ptt_input)
          event_status.curr_ptt_input |= (1<<STATUS_PTT_INPUT_VOX_SENSE);
        else
          event_status.curr_ptt_input &= ~(1<<STATUS_PTT_INPUT_VOX_SENSE);
      }
      else {
        if (event_status.temp_ptt_input)
          event_status.curr_ptt_input &= ~(1<<STATUS_PTT_INPUT_VOX_SENSE);
        else
          event_status.curr_ptt_input |= (1<<STATUS_PTT_INPUT_VOX_SENSE);
      }
    }

    if (event_status.curr_ptt_input != event_status.prev_ptt_input) {
      PRINTF("PTT OUTPUT CURR: %i\n",event_status.curr_ptt_input);
      PRINTF("PTT OUTPUT PREV: %i\n",event_status.prev_ptt_input);

      //Execute the PTT sequencing etc
      event_handler_execute_ptt_change(event_status.prev_ptt_input,event_status.curr_ptt_input);

      //Update the PTT status to the computer
      status_set_ptt_input_state(event_status.curr_ptt_input);

      event_status.prev_ptt_input = event_status.curr_ptt_input;
    }

    event_status.flags &= ~(1<<EVENT_HANDLER_CHECK_PTT_INPUTS);
  }
}

void event_handler_1ms_tick(void) {
  counter_event_1ms++;
}

void event_handler_10us_tick(void) {
  if ((counter_event_10us % EVENT_HANDLER_POLL_PTT_STATUS_INTERVAL) == 0) {
    event_status.flags |= (1<<EVENT_HANDLER_CHECK_PTT_INPUTS);
  }

  counter_event_10us++;
}

void EINT1_IRQHandler(void) {
  if(LPC_GPIOINT->IO2IntStatF & (1<<11)) {
    RADIO_CW_PORT->FIOCLR = (1<<RADIO_CW);
    FRONT_LED_CW_PORT->FIOSET = (1<<FRONT_LED_CW);
   }

  if(LPC_GPIOINT->IO2IntStatR & (1<<11)) {
    RADIO_CW_PORT->FIOSET = (1<<RADIO_CW);
    FRONT_LED_CW_PORT->FIOCLR = (1<<FRONT_LED_CW);
  }

  LPC_GPIOINT->IO2IntClr=(1<<11);
}

void EINT2_IRQHandler(void) {
  if(LPC_GPIOINT->IO2IntStatF & (1<<11)) {
    //Enter FSK code here
  }

  if(LPC_GPIOINT->IO2IntStatR & (1<<11)) {
    //Enter FSK code here
  }

  LPC_GPIOINT->IO2IntClr=(1<<12);
}

void EINT3_IRQHandler(void) {
  if(LPC_GPIOINT->IO2IntStatF & (1<<13)) {
    RADIO_CW_PORT->FIOCLR = (1<<RADIO_CW);
    FRONT_LED_CW_PORT->FIOSET = (1<<FRONT_LED_CW);
   }

  if(LPC_GPIOINT->IO2IntStatR & (1<<13)) {
    RADIO_CW_PORT->FIOSET = (1<<RADIO_CW);
    FRONT_LED_CW_PORT->FIOCLR = (1<<FRONT_LED_CW);
  }

  LPC_GPIOINT->IO2IntClr=(1<<13);
}
