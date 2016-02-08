#ifndef __SETTING_H
#define __SETTING_H

#include <stdio.h>
#include "setting_structs.h"
#include "comm_interface.h"
#include "commands.h"
#include "lpc17xx.h"

struct_settings settings;
struct_cw_messages cw_messages;

#define SETTINGS_START_ADDRESS    2048

#define SETTINGS_CW_MESSAGES_START_ADDRESS  4096

//! PTT output #1 for the radio
#define SETTINGS_PTT_RADIO_OUTPUT_1         0
//! PTT output #2 for the radio
#define SETTINGS_PTT_RADIO_OUTPUT_2         1
//! PTT output #1 and #2 for the radio
#define SETTINGS_PTT_RADIO_OUTPUT_1_2       2
//! PTT output for the radio is disabled
#define SETTINGS_PTT_RADIO_OUTPUT_DISABLED  3

void settings_update_to_computer(uint8_t *data, uint16_t offset, uint8_t length);
void settings_update_from_computer(uint8_t *data, uint16_t offset, uint8_t length);

void settings_save_to_eeprom(void);
void settings_save_cw_messages_to_eeprom(void);
void settings_load_from_eeprom(void);

void settings_set_default_settings(void);

/********** START SET RADIO SETTINGS **********/
void settings_set_radio_model(uint8_t value);
void settings_set_cat_interface_enabled(uint8_t value);
void settings_set_cat_baudrate(uint32_t value);
void settings_set_cat_stopbits(uint8_t value);
void settings_set_cat_parity(uint8_t value);
void settings_set_cat_icom_civ(uint8_t value);
void settings_set_cat_flow_control(uint8_t value);
void settings_set_cat_jumper_cts_rts(uint8_t value);
void settings_set_cat_retrieve_type(uint8_t value);
void settings_set_cat_poll_rate(uint8_t value);
/********** END SET RADIO SETTINGS **********/

/********** START GET RADIO SETTINGS **********/
uint8_t settings_get_radio_model(void);
uint8_t settings_get_cat_interface_enabled(void);
uint32_t settings_get_cat_baudrate(void);
uint8_t settings_get_cat_stopbits(void);
uint8_t settings_get_cat_parity(void);
uint8_t settings_get_cat_icom_civ(void);
uint8_t settings_get_cat_flow_control(void);
uint8_t settings_get_cat_jumper_cts_rts(void);
uint8_t settings_get_cat_retrieve_type(void);
uint8_t settings_get_cat_poll_rate(void);
/********** END GET RADIO SETTINGS **********/

/********** START SET PTT SETTINGS **********/
void settings_set_ptt_input_enabled(uint8_t value);
void settings_set_ptt_input_invert(uint8_t value);
void settings_set_ptt_radio_pre_delay(uint8_t value);  //in 10th of ms, so 10 means 100ms
void settings_set_ptt_radio_post_delay(uint8_t value);
void settings_set_ptt_amp_pre_delay(uint8_t value);
void settings_set_ptt_amp_post_delay(uint8_t value);
void settings_set_ptt_inhibit_pre_delay(uint8_t value);
void settings_set_ptt_inhibit_post_delay(uint8_t value);
void settings_set_ptt_antenna_pre_delay(uint8_t value);
void settings_set_ptt_antenna_post_delay(uint8_t value);
void settings_set_radio_digital_ptt(uint8_t value);
void settings_set_radio_cw_ptt(uint8_t value);
void settings_set_radio_phone_ptt(uint8_t value);
void settings_set_amplifier_enabled(uint8_t value);
void settings_set_inhibit_enabled(uint8_t value);
void settings_set_inhibit_inverted(uint8_t value);
/********** END SET PTT SETTINGS **********/

/********** START GET PTT SETTINGS **********/
uint8_t settings_get_ptt_input_enabled(void);
uint8_t settings_get_ptt_input_invert(void);
uint8_t settings_get_ptt_radio_pre_delay(void);  //in 10th of ms, so 10 means 100ms
uint8_t settings_get_ptt_radio_post_delay(void);
uint8_t settings_get_ptt_amp_pre_delay(void);
uint8_t settings_get_ptt_amp_post_delay(void);
uint8_t settings_get_ptt_inhibit_pre_delay(void);
uint8_t settings_get_ptt_inhibit_post_delay(void);
uint8_t settings_get_ptt_antenna_pre_delay(void);
uint8_t settings_get_ptt_antenna_post_delay(void);
uint8_t settings_get_radio_digital_ptt(void);
uint8_t settings_get_radio_cw_ptt(void);
uint8_t settings_get_radio_phone_ptt(void);
uint8_t settings_get_amplifier_enabled(void);
uint8_t settings_get_inhibit_enabled(void);
uint8_t settings_get_inhibit_inverted(void);
/********** END GET PTT SETTINGS **********/

/********** START SET AUDIO SETTINGS **********/
void settings_set_af_input_source_cw(uint8_t type, uint8_t state);
void settings_set_af_input_source_phone(uint8_t type, uint8_t state);
void settings_set_af_input_source_digital(uint8_t type, uint8_t state);
void settings_set_mic_path(uint8_t type, uint8_t state);
void settings_set_mic_mute(uint8_t type, uint8_t state);
void settings_set_mic_preamp(uint8_t type, uint8_t state);
void settings_set_mic_ext_preamp(uint8_t type, uint8_t state);
void settings_set_line_out_mute_phone(uint8_t type, uint8_t state);
void settings_set_line_out_mute_digital(uint8_t type, uint8_t state);
/********** END GET AUDIO SETTINGS **********/

/********** START GET AUDIO SETTINGS **********/
uint8_t settings_get_af_input_source_cw(uint8_t type);
uint8_t settings_get_af_input_source_phone(uint8_t type);
uint8_t settings_get_af_input_source_digital(uint8_t type);
uint8_t settings_get_mic_path(uint8_t type);
uint8_t settings_get_mic_mute(uint8_t type);
uint8_t settings_get_mic_preamp(uint8_t type);
uint8_t settings_get_mic_ext_preamp(uint8_t type);
/********** END GET AUDIO SETTINGS **********/

/********** START SET CW SETTINGS **********/
void settings_set_cw_input_source(uint8_t value);
/********** END SET CW SETTINGS **********/

/********** START GET CW SETTINGS **********/
uint8_t settings_get_cw_input_source(void);
/********** END GET CW SETTINGS **********/

/********** START SET MISC SETTINGS **********/
void settings_set_backlight_rgb(uint8_t red, uint8_t green, uint8_t blue);
/********** END SET MISC SETTINGS **********/

/********** START GET MISC SETTINGS **********/
uint8_t settings_get_backlight_rgb_red(void);
uint8_t settings_get_backlight_rgb_green(void);
uint8_t settings_get_backlight_rgb_blue(void);
uint8_t *settings_get_callsign(void);
/********** END GET MISC SETTINGS **********/

/********** START SET WINKEY SETTINGS **********/
void settings_set_winkey_keyer_mode(uint8_t value);
void settings_set_winkey_swap_paddles(uint8_t state);
void settings_set_winkey_auto_space(uint8_t state);
void settings_set_winkey_ct_space(uint8_t state);
void settings_set_winkey_speed_pot_min(uint8_t value);
void settings_set_winkey_speed_pot_max(uint8_t value);
void settings_set_winkey_farnsworth(uint8_t value);
void settings_set_winkey_weight(uint8_t value);
void settings_set_winkey_dih_dah_ratio(uint8_t value);
void settings_set_winkey_first_extension(uint8_t value);
void settings_set_winkey_key_compensation(uint8_t value);
void settings_set_winkey_lead_time(uint8_t value);
void settings_set_winkey_tail_time(uint8_t value);
void settings_set_winkey_paddle_memory(uint8_t state, uint8_t dit_state);
void settings_set_winkey_priority(uint8_t value);
void settings_set_winkey_paddle_memory_sw_point(uint8_t value);
void settings_set_winkey_hangtime(uint8_t value);
void settings_set_winkey_pincfg(uint8_t value);
void settings_set_winkey_default_speed(uint8_t value);
/********** STOP SET WINKEY SETTINGS **********/

/********** START GET WINKEY SETTINGS **********/
uint8_t settings_get_winkey_modereg(void);
uint8_t settings_get_winkey_keyer_mode(void);
uint8_t settings_get_winkey_swap_paddles(void);
uint8_t settings_get_winkey_auto_space(void);
uint8_t settings_get_winkey_ct_space(void);
uint8_t settings_get_winkey_speed_pot_min(void);
uint8_t settings_get_winkey_speed_pot_max(void);
uint8_t settings_get_winkey_farnsworth(void);
uint8_t settings_get_winkey_weight(void);
uint8_t settings_get_winkey_dih_dah_ratio(void);
uint8_t settings_get_winkey_first_extension(void);
uint8_t settings_get_winkey_key_compensation(void);
uint8_t settings_get_winkey_lead_time(void);
uint8_t settings_get_winkey_tail_time(void);
uint8_t settings_get_winkey_paddle_memory(void);
uint8_t settings_get_winkey_priority(void);
uint8_t settings_get_winkey_paddle_memory_sw_point(void);
uint8_t settings_get_winkey_hangtime(void);
uint8_t settings_get_winkey_pincfg(void);
uint8_t settings_get_winkey_default_speed(void);
/********** STOP GET WINKEY SETTINGS **********/

/********** START SET DISPLAY SETTINGS **********/
void settings_set_display(uint8_t line_id, uint8_t mode, uint8_t id, uint8_t *str);
/********** STOP SET DISPLAY SETTINGS **********/

/********** START GET DISPLAY SETTINGS **********/
uint8_t *settings_get_display_text(uint8_t line_id, uint8_t mode);
uint8_t settings_get_display_id(uint8_t line_id, uint8_t mode);
/********** STOP GET DISPLAY SETTINGS **********/


/********** START CW MESSAGES SETTINGS **********/
void settings_send_cw_messages(void);
void settings_set_cw_message(uint8_t index, uint8_t length, uint8_t *data);
uint8_t *settings_get_cw_message(uint8_t index);
/********** STOP CW MESSAGES SETTINGS **********/

#define SETTINGS_UPDATE_TO_COMPUTER(x) settings_update_to_computer((uint8_t *)&x, (uint16_t)((uint32_t)&x - (uint32_t)&settings), sizeof(x))

#endif /* end __SETTING_H */
/****************************************************************************
** End Of File
*****************************************************************************/
