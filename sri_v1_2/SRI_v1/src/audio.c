#include <stdio.h>

#include "LPC17xx.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "audio.h"
#include "ctrl.h"
#include "board.h"
#include "status.h"
#include "settings.h"
#include "setting_structs.h"

void audio_init(void) {
  //Set default AF input source as the configured for CW, later changed when CAT data etc received
  audio_radio_afin_source(settings_get_af_input_source_cw(SETTING_AUDIO_ROUTE_RX));

  audio_set_mic_mute_state(SETTING_AUDIO_ROUTE_RX);
  audio_set_mic_preamp_state(SETTING_AUDIO_ROUTE_RX);
  audio_set_mic_ext_preamp_state(SETTING_AUDIO_ROUTE_RX);
  audio_set_mic_path(SETTING_AUDIO_ROUTE_RX);
}

void audio_set_state(uint8_t mode, uint8_t route_index) {
  if (mode != STATUS_RADIO_MODE_TYPE_UNKNOWN) {
    if (mode == STATUS_RADIO_MODE_TYPE_CW) {
      audio_radio_afin_source(settings_get_af_input_source_cw(route_index));
    }
    else if (mode == STATUS_RADIO_MODE_TYPE_PHONE) {
      audio_radio_afin_source(settings_get_af_input_source_phone(route_index));

      audio_set_mic_mute_state(route_index);
      audio_set_mic_preamp_state(route_index);
      audio_set_mic_ext_preamp_state(route_index);
      audio_set_mic_path(route_index);
    }
    else if (mode == STATUS_RADIO_MODE_TYPE_DIGITAL) {
      audio_radio_afin_source(settings_get_af_input_source_digital(route_index));
    }
  }
}

void audio_radio_afin_source(uint8_t source) {
  if (source == SETTING_AUDIO_AF_INPUT_SORUCE_LOW) {
    ctrl_radio_input_source_low();
  }
  else if (source == SETTING_AUDIO_AF_INPUT_SORUCE_HIGH) {
    ctrl_radio_input_source_high();
  }
}

void audio_set_mic_mute_state(uint8_t type) {
  if (settings_get_mic_mute(type)) {
    ctrl_pcm2912_mic_mute_on();
  }
  else {
    ctrl_pcm2912_mic_mute_off();
  }
}

void audio_set_mic_preamp_state(uint8_t type) {
  if (settings_get_mic_preamp(type)) {
    ctrl_pcm2912_mic_preamp_on();
  }
  else
    ctrl_pcm2912_mic_preamp_off();
}

void audio_set_mic_ext_preamp_state(uint8_t type) {
  if (settings_get_mic_ext_preamp(type)) {
    ctrl_mic_ext_preamp_relay_set();
  }
  else
    ctrl_mic_ext_preamp_relay_clr();
}

void audio_set_mic_path(uint8_t type) {
  if (settings_get_mic_path(type) == SETTING_AUDIO_MIC_ROUTE_DIRECT) {
    ctrl_mic_relay_clr();
  }
  else
    ctrl_mic_relay_set();
}
