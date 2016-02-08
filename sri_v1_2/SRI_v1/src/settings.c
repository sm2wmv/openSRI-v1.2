#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "settings.h"
#include "eeprom_m24.h"
#include "main.h"
#include "computer_interface.h"
#include "comm_interface.h"
#include "misc.h"
#include "event_queue.h"

// Sends settings data to the computer
void settings_update_to_computer(uint8_t *data, uint16_t offset, uint8_t length) {
  if (computer_interface_connection_status())
    comm_interface_add_tx_struct_message(SRI_CMD_SEND_SETTINGS, length, offset, data);
}

void settings_update_from_computer(uint8_t *data, uint16_t offset, uint8_t length) {
  uint8_t *ptr = (uint8_t *)&settings;

  for (uint8_t i=0;i<length;i++)
    *(ptr+offset+i) = data[i];
}

void settings_save_to_eeprom(void) {
  if (eeprom_m24_get_write_in_progress()) {
    //If an write event is ongoing then we try to write again in 100 ms
    event_add_message((void *)settings_save_to_eeprom, 100, EEPROM_EVENT_TYPE_WRITE, EVENT_PRIORITY_LEVEL_MAINLOOP);
  }
  else
    eeprom_m24_write_block(SETTINGS_START_ADDRESS, sizeof(settings),(uint8_t *)&settings);
}

void settings_save_cw_messages_to_eeprom(void) {
  if (eeprom_m24_get_write_in_progress()) {
    //If an write event is ongoing then we try to write again in 100 ms
    event_add_message((void *)settings_save_cw_messages_to_eeprom, 100, EEPROM_EVENT_TYPE_WRITE, EVENT_PRIORITY_LEVEL_MAINLOOP);
  }
  else
    eeprom_m24_write_block(SETTINGS_CW_MESSAGES_START_ADDRESS, sizeof(cw_messages),(uint8_t *)&cw_messages);
}

void settings_load_from_eeprom(void) {
  eeprom_m24_read_block(SETTINGS_START_ADDRESS, sizeof(settings),(uint8_t *)&settings);
  eeprom_m24_read_block(SETTINGS_CW_MESSAGES_START_ADDRESS, sizeof(cw_messages),(uint8_t *)&cw_messages);
}

void settings_set_default_settings(void) {
  settings.radio.cat_baudrate = 4800;
  settings.radio.cat_flow_control = 0;
  settings.radio.cat_icom_civ = 40;
  settings.radio.cat_interface_enabled = 0;
  settings.radio.cat_jumper_rts_cts = 0;
  settings.radio.cat_parity = 0;
  settings.radio.cat_poll_rate = 50;
  settings.radio.cat_retrieve_type = 0;
  settings.radio.radio_model = 0;

  settings.audio.af_input_source_cw[0] = 1;
  settings.audio.af_input_source_cw[1] = 1;
  settings.audio.af_input_source_cw[2] = 1;
  settings.audio.af_input_source_cw[3] = 1;
  settings.audio.af_input_source_cw[4] = 1;

  settings.audio.af_input_source_phone[0] = 1;
  settings.audio.af_input_source_phone[1] = 1;
  settings.audio.af_input_source_phone[2] = 1;
  settings.audio.af_input_source_phone[3] = 1;
  settings.audio.af_input_source_phone[4] = 1;

  settings.audio.af_input_source_digital[0] = 0;
  settings.audio.af_input_source_digital[1] = 0;
  settings.audio.af_input_source_digital[2] = 0;
  settings.audio.af_input_source_digital[3] = 0;
  settings.audio.af_input_source_digital[4] = 0;

  settings.audio.mic_mute[0] = 1;
  settings.audio.mic_mute[1] = 0;
  settings.audio.mic_mute[2] = 0;
  settings.audio.mic_mute[3] = 1;
  settings.audio.mic_mute[4] = 0;

  settings.audio.mic_path[0] = 0;
  settings.audio.mic_path[1] = 0;
  settings.audio.mic_path[2] = 1;
  settings.audio.mic_path[3] = 0;
  settings.audio.mic_path[4] = 0;

  settings.audio.mic_preamp[0] = 0;
  settings.audio.mic_preamp[1] = 0;
  settings.audio.mic_preamp[2] = 0;
  settings.audio.mic_preamp[3] = 0;
  settings.audio.mic_preamp[4] = 0;

  settings.cw.input_source = (1<<SETTING_CW_INPUT_SOURCE_WINKEY);

  strcpy((char *)settings.misc.callsign,(char *)"SJ2W");

  settings.misc.display_rgb_red = 255;
  settings.misc.display_rgb_green = 210;
  settings.misc.display_rgb_blue = 0;

  settings.ptt.amp_pre_delay = 0;
  settings.ptt.amp_post_delay = 3;
  settings.ptt.radio_pre_delay = 1;
  settings.ptt.radio_post_delay = 0;
  settings.ptt.antenna_pre_delay = 0;
  settings.ptt.antenna_post_delay = 4;

  settings.ptt.inhibit_pre_delay = 0;
  settings.ptt.inhibit_post_delay = 0;
  settings.ptt.inhibit_inverted = 0;
  settings.ptt.inhibit_enabled = 0;

  settings.ptt.ptt_input_enabled = (1<<SETTING_PTT_INPUT_FOOTSWITCH) | (1<<SETTING_PTT_INPUT_WK);
  settings.ptt.ptt_input_invert = 0;
  settings.ptt.radio_cw_ptt = 0;
  settings.ptt.radio_phone_ptt = 0;
  settings.ptt.radio_digital_ptt = 1;

  settings.winkey.def_speed = 28;
  settings.winkey.farnswpm = 0;
  settings.winkey.kcomp = 0;
  settings.winkey.lead_time = 1;
  settings.winkey.magic = 0;
  settings.winkey.minwpm = 5;
  settings.winkey.modereg = (1<<2) | (1<<4) | (1<<6);
  settings.winkey.oprrate = 0;
  settings.winkey.pincfg = 0;
  settings.winkey.ratio = 50;
  settings.winkey.sampadj = 50;
  settings.winkey.stconst = 0;
  settings.winkey.tail_time = 0;
  settings.winkey.weight = 50;
  settings.winkey.wpmrange = 45;
  settings.winkey.xtnd = 0;

  settings.ptt.amplifier_enabled = 0;

  strcpy((char *)cw_messages.str[0],(char *)"MESSAGE1");
  strcpy((char *)cw_messages.str[1],(char *)"MESSAGE2");
  strcpy((char *)cw_messages.str[2],(char *)"MESSAGE3");
  strcpy((char *)cw_messages.str[3],(char *)"MESSAGE4");
  strcpy((char *)cw_messages.str[4],(char *)"MESSAGE5");
  strcpy((char *)cw_messages.str[5],(char *)"MESSAGE6");
  strcpy((char *)cw_messages.str[6],(char *)"MESSAGE7");
  strcpy((char *)cw_messages.str[7],(char *)"MESSAGE8");

  settings.display_cw.id[0] = 1;
  settings.display_cw.id[1] = 4;
  settings.display_cw.id[2] = 1;
  settings.display_cw.id[3] = 4;

  settings.display_phone.id[0] = 1;
  settings.display_phone.id[1] = 3;
  settings.display_phone.id[2] = 1;
  settings.display_phone.id[3] = 3;

  settings.display_digital.id[0] = 1;
  settings.display_digital.id[1] = 3;
  settings.display_digital.id[2] = 1;
  settings.display_digital.id[3] = 3;

  strcpy((char *)settings.display_cw.text[0], "Call: %s");
  strcpy((char *)settings.display_cw.text[1], "CW Speed: %i");
  strcpy((char *)settings.display_cw.text[2], "Call: %s");
  strcpy((char *)settings.display_cw.text[3], "CW Speed: %i");

  strcpy((char *)settings.display_phone.text[0], "Call: %s");
  strcpy((char *)settings.display_phone.text[1], "Mode: %s");
  strcpy((char *)settings.display_phone.text[2], "Call: %s");
  strcpy((char *)settings.display_phone.text[3], "Mode: %s");

  strcpy((char *)settings.display_digital.text[0], "Call: %s");
  strcpy((char *)settings.display_digital.text[1], "Mode: %s");
  strcpy((char *)settings.display_digital.text[2], "Call: %s");
  strcpy((char *)settings.display_digital.text[3], "Mode: %s");
}

/********** START SET RADIO SETTINGS **********/
void settings_set_radio_model(uint8_t value) {
  settings.radio.radio_model = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.radio.radio_model);
}

void settings_set_cat_interface_enabled(uint8_t value) {
  settings.radio.cat_interface_enabled = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.radio.cat_interface_enabled);
}

void settings_set_cat_baudrate(uint32_t value) {
  settings.radio.cat_baudrate = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.radio.cat_baudrate);
}

void settings_set_cat_stopbits(uint8_t value) {
  settings.radio.cat_stopbits = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.radio.cat_stopbits);
}

void settings_set_cat_parity(uint8_t value) {
  settings.radio.cat_parity = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.radio.cat_parity);
}

void settings_set_cat_icom_civ(uint8_t value) {
  settings.radio.cat_icom_civ = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.radio.cat_icom_civ);
}

void settings_set_cat_flow_control(uint8_t value) {
  settings.radio.cat_flow_control = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.radio.cat_flow_control);
}

void settings_set_cat_jumper_cts_rts(uint8_t value) {
  settings.radio.cat_jumper_rts_cts = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.radio.cat_jumper_rts_cts);
}

void settings_set_cat_retrieve_type(uint8_t value) {
  settings.radio.cat_retrieve_type = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.radio.cat_retrieve_type);
}

void settings_set_cat_poll_rate(uint8_t value) {
  settings.radio.cat_poll_rate = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.radio.cat_poll_rate);
}

/********** END SET RADIO SETTINGS **********/

/********** START GET RADIO SETTINGS **********/

uint8_t settings_get_radio_model(void) {
  return(settings.radio.radio_model);
}

uint8_t settings_get_cat_interface_enabled(void) {
  return(settings.radio.cat_interface_enabled);
}

uint32_t settings_get_cat_baudrate(void) {
  return(settings.radio.cat_baudrate);
}

uint8_t settings_get_cat_stopbits(void) {
  return(settings.radio.cat_stopbits);
}

uint8_t settings_get_cat_parity(void) {
  return(settings.radio.cat_parity);
}

uint8_t settings_get_cat_icom_civ(void) {
  return(settings.radio.cat_icom_civ);
}

uint8_t settings_get_cat_flow_control(void) {
  return(settings.radio.cat_flow_control);
}

uint8_t settings_get_cat_jumper_cts_rts(void) {
  return(settings.radio.cat_jumper_rts_cts);
}

uint8_t settings_get_cat_retrieve_type(void) {
  return(settings.radio.cat_retrieve_type);
}

uint8_t settings_get_cat_poll_rate(void) {
  return(settings.radio.cat_poll_rate);
}

/********** END GET RADIO SETTINGS **********/

/********** START GET PTT SETTINGS **********/
uint8_t settings_get_ptt_input_enabled(void) {
  return(settings.ptt.ptt_input_enabled);
}

uint8_t settings_get_ptt_input_invert(void) {
  return(settings.ptt.ptt_input_invert);
}

uint8_t settings_get_ptt_radio_pre_delay(void) {
  return(settings.ptt.radio_pre_delay);
}

uint8_t settings_get_ptt_radio_post_delay(void) {
  return(settings.ptt.radio_post_delay);
}

uint8_t settings_get_ptt_amp_pre_delay(void) {
  return(settings.ptt.amp_pre_delay);
}

uint8_t settings_get_ptt_amp_post_delay(void) {
  return(settings.ptt.amp_post_delay);
}

uint8_t settings_get_ptt_inhibit_pre_delay(void) {
  return(settings.ptt.inhibit_pre_delay);
}

uint8_t settings_get_ptt_inhibit_post_delay(void) {
  return(settings.ptt.inhibit_post_delay);
}

uint8_t settings_get_ptt_antenna_pre_delay(void) {
  return(settings.ptt.antenna_pre_delay);
}

uint8_t settings_get_ptt_antenna_post_delay(void) {
  return(settings.ptt.antenna_post_delay);
}

uint8_t settings_get_radio_digital_ptt(void) {
  return(settings.ptt.radio_digital_ptt);
}

uint8_t settings_get_radio_cw_ptt(void) {
  return(settings.ptt.radio_cw_ptt);
}

uint8_t settings_get_radio_phone_ptt(void) {
  return(settings.ptt.radio_phone_ptt);
}

uint8_t settings_get_amplifier_enabled(void) {
  return(settings.ptt.amplifier_enabled);
}

uint8_t settings_get_inhibit_enabled(void) {
  return(settings.ptt.inhibit_enabled);
}

uint8_t settings_get_inhibit_inverted(void) {
  return(settings.ptt.inhibit_inverted);
}
/********** END GET PTT SETTINGS **********/

/********** START SET PTT SETTINGS **********/
void settings_set_ptt_input_enabled(uint8_t value) {
  settings.ptt.ptt_input_enabled = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.ptt_input_enabled);
}

void settings_set_ptt_input_invert(uint8_t value) {
  settings.ptt.ptt_input_invert = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.ptt_input_invert);
}

void settings_set_ptt_radio_pre_delay(uint8_t value) {
  settings.ptt.radio_pre_delay = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.radio_pre_delay);
}

void settings_set_ptt_radio_post_delay(uint8_t value) {
  settings.ptt.radio_post_delay = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.radio_post_delay);
}

void settings_set_ptt_amp_pre_delay(uint8_t value) {
  settings.ptt.amp_pre_delay = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.amp_pre_delay);
}

void settings_set_ptt_amp_post_delay(uint8_t value) {
  settings.ptt.amp_post_delay = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.amp_post_delay);
}

void settings_set_ptt_inhibit_pre_delay(uint8_t value) {
  settings.ptt.inhibit_pre_delay = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.inhibit_pre_delay);
}

void settings_set_ptt_inhibit_post_delay(uint8_t value) {
  settings.ptt.inhibit_post_delay = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.inhibit_post_delay);
}

void settings_set_ptt_antenna_pre_delay(uint8_t value) {
  settings.ptt.antenna_pre_delay = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.antenna_pre_delay);
}

void settings_set_ptt_antenna_post_delay(uint8_t value) {
  settings.ptt.antenna_post_delay = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.antenna_post_delay);
}

void settings_set_radio_digital_ptt(uint8_t value) {
  settings.ptt.radio_digital_ptt = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.radio_digital_ptt);
}

void settings_set_radio_cw_ptt(uint8_t value) {
  settings.ptt.radio_cw_ptt = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.radio_cw_ptt);
}

void settings_set_radio_phone_ptt(uint8_t value) {
  settings.ptt.radio_phone_ptt = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.radio_phone_ptt);
}

void settings_set_amplifier_enabled(uint8_t value) {
  settings.ptt.amplifier_enabled = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.amplifier_enabled);
}

void settings_set_inhibit_enabled(uint8_t value) {
  settings.ptt.inhibit_enabled = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.inhibit_enabled);
}

void settings_set_inhibit_inverted(uint8_t value) {
  settings.ptt.inhibit_inverted = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.ptt.inhibit_inverted);
}
/********** END SET PTT SETTINGS **********/

/********** START GET AUDIO SETTINGS **********/
void settings_set_af_input_source_cw(uint8_t type, uint8_t state) {
  settings.audio.af_input_source_cw[type] = state;

  SETTINGS_UPDATE_TO_COMPUTER(settings.audio.af_input_source_cw);
}

void settings_set_af_input_source_phone(uint8_t type, uint8_t state) {
  settings.audio.af_input_source_phone[type] = state;

  SETTINGS_UPDATE_TO_COMPUTER(settings.audio.af_input_source_phone);
}

void settings_set_af_input_source_digital(uint8_t type, uint8_t state) {
  settings.audio.af_input_source_digital[type] = state;

  SETTINGS_UPDATE_TO_COMPUTER(settings.audio.af_input_source_digital);
}

void settings_set_mic_path(uint8_t type, uint8_t state) {
  settings.audio.mic_path[type] = state;

  SETTINGS_UPDATE_TO_COMPUTER(settings.audio.mic_path);
}

void settings_set_mic_mute(uint8_t type, uint8_t state) {
  settings.audio.mic_mute[type] = state;

  SETTINGS_UPDATE_TO_COMPUTER(settings.audio.mic_mute);
}

void settings_set_mic_preamp(uint8_t type, uint8_t state) {
  settings.audio.mic_preamp[type] = state;

  SETTINGS_UPDATE_TO_COMPUTER(settings.audio.mic_preamp);
}

void settings_set_mic_ext_preamp(uint8_t type, uint8_t state) {
  settings.audio.mic_ext_preamp[type] = state;

  SETTINGS_UPDATE_TO_COMPUTER(settings.audio.mic_ext_preamp);
}
/********** END GET AUDIO SETTINGS **********/

/********** START GET AUDIO SETTINGS **********/
uint8_t settings_get_af_input_source_cw(uint8_t type) {
  return(settings.audio.af_input_source_cw[type]);
}

uint8_t settings_get_af_input_source_phone(uint8_t type) {
  return(settings.audio.af_input_source_phone[type]);
}

uint8_t settings_get_af_input_source_digital(uint8_t type) {
  return(settings.audio.af_input_source_digital[type]);
}

uint8_t settings_get_mic_path(uint8_t type) {
  return(settings.audio.mic_path[type]);
}

uint8_t settings_get_mic_mute(uint8_t type) {
  return(settings.audio.mic_mute[type]);
}

uint8_t settings_get_mic_preamp(uint8_t type) {
  return(settings.audio.mic_preamp[type]);
}

uint8_t settings_get_mic_ext_preamp(uint8_t type) {
  return(settings.audio.mic_ext_preamp[type]);
}
/********** END GET AUDIO SETTINGS **********/

/********** START SET CW SETTINGS **********/
void settings_set_cw_input_source(uint8_t value) {
  settings.cw.input_source = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.cw.input_source);
}

/********** END SET CW SETTINGS **********/

/********** START GET CW SETTINGS **********/
uint8_t settings_get_cw_input_source(void) {
  return(settings.cw.input_source);
}
/********** END GET CW SETTINGS **********/


/********** START SET MISC SETTINGS **********/
void settings_set_backlight_rgb(uint8_t red, uint8_t green, uint8_t blue) {
  settings.misc.display_rgb_red = red;
  settings.misc.display_rgb_green = green;
  settings.misc.display_rgb_blue = blue;

  SETTINGS_UPDATE_TO_COMPUTER(settings.misc);
}
/********** END SET MISC SETTINGS **********/

/********** START GET MISC SETTINGS **********/
uint8_t settings_get_backlight_rgb_red(void) {
  return(settings.misc.display_rgb_red);
}

uint8_t settings_get_backlight_rgb_green(void) {
  return(settings.misc.display_rgb_green);
}

uint8_t settings_get_backlight_rgb_blue(void) {
  return(settings.misc.display_rgb_blue);
}

uint8_t *settings_get_callsign(void) {
  return(settings.misc.callsign);
}

/********** END GET MISC SETTINGS **********/

/********** START SET WINKEY SETTINGS **********/
void settings_set_winkey_keyer_mode(uint8_t value) {
  settings.winkey.modereg &= ~(1<<5);
  settings.winkey.modereg &= ~(1<<4);
  settings.winkey.modereg |= (value<<4);

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.modereg);
}

void settings_set_winkey_swap_paddles(uint8_t state) {
  if (state)
    settings.winkey.modereg |= (1<<3);
  else
    settings.winkey.modereg &= ~(1<<3);

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.modereg);
}

void settings_set_winkey_auto_space(uint8_t state) {
  if (state)
    settings.winkey.modereg |= (1<<1);
  else
    settings.winkey.modereg &= ~(1<<1);

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.modereg);
}

void settings_set_winkey_ct_space(uint8_t state) {
  if (state)
    settings.winkey.modereg |= (1<<0);
  else
    settings.winkey.modereg &= ~(1<<0);

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.modereg);
}

void settings_set_winkey_speed_pot_min(uint8_t value) {
  settings.winkey.minwpm = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.minwpm);
}

void settings_set_winkey_speed_pot_max(uint8_t value) {
  settings.winkey.wpmrange = value - settings.winkey.minwpm;

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.wpmrange);
}

void settings_set_winkey_farnsworth(uint8_t value) {
  settings.winkey.farnswpm = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.farnswpm);
}

void settings_set_winkey_weight(uint8_t value) {
  settings.winkey.weight = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.weight);
}

void settings_set_winkey_dih_dah_ratio(uint8_t value) {
  settings.winkey.ratio = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.ratio);
}

void settings_set_winkey_first_extension(uint8_t value) {
  settings.winkey.xtnd = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.xtnd);
}

void settings_set_winkey_key_compensation(uint8_t value) {
  settings.winkey.kcomp = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.kcomp);
}

void settings_set_winkey_lead_time(uint8_t value) {
  settings.winkey.lead_time = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.lead_time);
}

void settings_set_winkey_tail_time(uint8_t value) {
  settings.winkey.tail_time = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.tail_time);
}

void settings_set_winkey_paddle_memory(uint8_t state, uint8_t dit_state) {
  settings.winkey.pincfg &= ~(1<<7);
  settings.winkey.pincfg &= ~(1<<6);

  if (state) {
    if (dit_state)
      settings.winkey.pincfg |= (1<<7);
    else
      settings.winkey.pincfg |= (1<<6);
  }

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.pincfg);
}

void settings_set_winkey_priority(uint8_t state) {
  settings.winkey.pincfg &= ~(1<<7);
  settings.winkey.pincfg &= ~(1<<6);

  if (state)
    settings.winkey.pincfg |= (1<<7);
  else
    settings.winkey.pincfg |= (1<<6);

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.pincfg);
}

void settings_set_winkey_paddle_memory_sw_point(uint8_t value) {
  settings.winkey.sampadj = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.sampadj);
}

void settings_set_winkey_hangtime(uint8_t value) {
  settings.winkey.pincfg &= ~(1<<5);
  settings.winkey.pincfg &= ~(1<<4);

  settings.winkey.pincfg |= (value << 4);

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.pincfg);
}

void settings_set_winkey_pincfg(uint8_t value) {
  settings.winkey.pincfg = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.pincfg);
}

void settings_set_winkey_default_speed(uint8_t value) {
  settings.winkey.def_speed = value;

  SETTINGS_UPDATE_TO_COMPUTER(settings.winkey.def_speed);
}
/********** STOP SET WINKEY SETTINGS **********/

/********** START GET WINKEY SETTINGS **********/
uint8_t settings_get_winkey_modereg(void) {
  return(settings.winkey.modereg);
}

uint8_t settings_get_winkey_keyer_mode(void) {
  return((settings.winkey.modereg & 0x30) >> 4);
}

uint8_t settings_get_winkey_swap_paddles(void) {
  if (settings.winkey.modereg & (1<<3))
    return(1);

  return(0);
}

uint8_t settings_get_winkey_auto_space(void) {
  if (settings.winkey.modereg & (1<<1))
    return(1);

  return(0);
}

uint8_t settings_get_winkey_ct_space(void) {
  if (settings.winkey.modereg & (1<<0))
    return(1);

  return(0);
}

uint8_t settings_get_winkey_speed_pot_min(void) {
  return(settings.winkey.minwpm);
}

uint8_t settings_get_winkey_speed_pot_max(void) {
  return(settings.winkey.wpmrange + settings.winkey.minwpm);
}

uint8_t settings_get_winkey_farnsworth(void) {
  return(settings.winkey.farnswpm);
}

uint8_t settings_get_winkey_weight(void) {
  return(settings.winkey.weight);
}

uint8_t settings_get_winkey_dih_dah_ratio(void) {
  return(settings.winkey.ratio);
}

uint8_t settings_get_winkey_first_extension(void) {
  return(settings.winkey.xtnd);
}

uint8_t settings_get_winkey_key_compensation(void) {
  return(settings.winkey.kcomp);
}

uint8_t settings_get_winkey_lead_time(void) {
  return(settings.winkey.lead_time);
}

uint8_t settings_get_winkey_tail_time(void) {
  return(settings.winkey.tail_time);
}

uint8_t settings_get_winkey_paddle_memory(void) {
  if (settings.winkey.pincfg & (1<<7))
    return(1);
  if (settings.winkey.pincfg & (1<<6))
    return(1);

  return(0);
}

uint8_t settings_get_winkey_priority(void) {
  if (settings.winkey.pincfg & (1<<7))
    return(1);

  return(0);
}

uint8_t settings_get_winkey_paddle_memory_sw_point(void) {
  return(settings.winkey.sampadj);
}

uint8_t settings_get_winkey_hangtime(void) {
  return((settings.winkey.pincfg & 0x30) >> 4);
}

uint8_t settings_get_winkey_pincfg(void) {
  //Enable PTT output
  settings.winkey.pincfg |= (1<<0);
  //Enable Pin 3 keyout enable
  settings.winkey.pincfg |= (1<<2);
  //Make sure PTT output #2 is not enabled
  settings.winkey.pincfg &= ~(1<<3);

  return(settings.winkey.pincfg);
}

uint8_t settings_get_winkey_default_speed(void) {
  return(settings.winkey.def_speed);
}

/********** STOP GET WINKEY SETTINGS **********/

/*********** START DISPLAY SETTINGS ***********/

void settings_set_display(uint8_t line_id, uint8_t mode, uint8_t id, uint8_t *str) {
  if (mode == DISPLAY_MODE_CW) {
    settings.display_cw.id[line_id] = id;
    strcpy((char *)settings.display_cw.text[line_id],(char *)str);

    SETTINGS_UPDATE_TO_COMPUTER(settings.display_cw);
  }
  else if (mode == DISPLAY_MODE_PHONE) {
    settings.display_phone.id[line_id] = id;
    strcpy((char *)settings.display_phone.text[line_id],(char *)str);

    SETTINGS_UPDATE_TO_COMPUTER(settings.display_phone);
  }
  else if (mode == DISPLAY_MODE_DIGITAL) {
    settings.display_digital.id[line_id] = id;
    strcpy((char *)settings.display_digital.text[line_id],(char *)str);

    SETTINGS_UPDATE_TO_COMPUTER(settings.display_digital);
  }
}

uint8_t *settings_get_display_text(uint8_t line_id, uint8_t mode) {
  if (mode == DISPLAY_MODE_CW)
    return(settings.display_cw.text[line_id]);
  else if (mode == DISPLAY_MODE_PHONE)
    return(settings.display_phone.text[line_id]);
  else if (mode == DISPLAY_MODE_DIGITAL)
    return(settings.display_digital.text[line_id]);

  return(0);
}

uint8_t settings_get_display_id(uint8_t line_id, uint8_t mode) {
  if (mode == DISPLAY_MODE_CW)
    return(settings.display_cw.id[line_id]);
  else if (mode == DISPLAY_MODE_PHONE)
    return(settings.display_phone.id[line_id]);
  else if (mode == DISPLAY_MODE_DIGITAL)
    return(settings.display_digital.id[line_id]);

  return(0);
}

/*********** STOP DISPLAY SETTINGS ***********/

/********** START CW MESSAGES SETTINGS **********/

void settings_send_cw_messages(void) {
  if (computer_interface_connection_status()) {
    uint8_t temp_data[53];
    uint8_t len=0;

    for (uint8_t i=0;i<8;i++) {
      len = strlen((char *)(cw_messages.str[i]));

      if ((len > 0) && (len < 51)) {
        temp_data[0] = SRI_SUB_CMD_SET_CW_MESSAGE;
        temp_data[1] = i;

        for (uint8_t j=0;j<len;j++)
          temp_data[2+j] = cw_messages.str[i][j];

        temp_data[2+len] = 0;

        comm_interface_add_tx_message(SRI_CMD_CW_MESSAGE, len+2, temp_data);
      }
    }
  }
}

void settings_set_cw_message(uint8_t index, uint8_t length, uint8_t *data) {
  strncpy((char *)cw_messages.str[index], (char *)data,length);
  cw_messages.str[index][length] = 0;
}

uint8_t *settings_get_cw_message(uint8_t index) {
  return(cw_messages.str[index]);
}

/********** STOP CW MESSAGES SETTINGS **********/
