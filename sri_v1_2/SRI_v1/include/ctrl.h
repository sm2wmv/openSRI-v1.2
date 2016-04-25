#ifndef __CTRL_H
#define __CTRL_H

#define CTRL_IO_EXP_RETRANSMISSIONS_MAX 3

void ctrl_ptt_amp_set(void);
void ctrl_ptt_amp_clr(void);

void ctrl_radio_cw_set(void);
void ctrl_radio_cw_clr(void);

void ctrl_mic_relay_set(void);
void ctrl_mic_relay_clr(void);

void ctrl_mic_ext_preamp_relay_set(void);
void ctrl_mic_ext_preamp_relay_clr(void);

void ctrl_hub_reset_set(void);
void ctrl_hub_reset_clr(void);

void ctrl_ft4232_reset_set(void);
void ctrl_ft4232_reset_clr(void);

uint8_t ctrl_i2c_io_exp_a0_get(void);
uint8_t ctrl_i2c_io_exp_a1_get(void);
uint8_t ctrl_i2c_io_exp_a2_get(void);
uint8_t ctrl_i2c_io_exp_a3_get(void);
uint8_t ctrl_i2c_io_exp_a4_get(void);
uint8_t ctrl_i2c_io_exp_a5_get(void);
uint8_t ctrl_i2c_io_exp_a6_get(void);
uint8_t ctrl_i2c_io_exp_a7_get(void);
uint8_t ctrl_i2c_io_exp_a8_get(void);

uint8_t ctrl_i2c_io_exp_a0_set(void);
uint8_t ctrl_i2c_io_exp_a1_set(void);
uint8_t ctrl_i2c_io_exp_a2_set(void);
uint8_t ctrl_i2c_io_exp_a3_set(void);
uint8_t ctrl_i2c_io_exp_a4_set(void);
uint8_t ctrl_i2c_io_exp_a5_set(void);
uint8_t ctrl_i2c_io_exp_a6_set(void);
uint8_t ctrl_i2c_io_exp_a7_set(void);
uint8_t ctrl_i2c_io_exp_a8_set(void);

void ctrl_pcm2912_mic_preamp_on(void);
void ctrl_pcm2912_mic_preamp_off(void);

void ctrl_pcm2912_mic_mute_on(void);
void ctrl_pcm2912_mic_mute_off(void);

void ctrl_led_fsk_on(void);
void ctrl_led_fsk_off(void);

void ctrl_led_cw_on(void);
void ctrl_led_cw_off(void);

void ctrl_led_pwr_on(void);
void ctrl_led_pwr_off(void);

void ctrl_led_ptt_on(void);
void ctrl_led_ptt_off(void);

void ctrl_led_inhibit_on(void);
void ctrl_led_inhibit_off(void);

void ctrl_led_button2_on(void);
void ctrl_led_button2_off(void);

void ctrl_led_button3_on(void);
void ctrl_led_button3_off(void);

void ctrl_led_button4_on(void);
void ctrl_led_button4_off(void);

void ctrl_led_aux_on(void);
void ctrl_led_aux_off(void);

void ctrl_radio_ptt_1_set(void);
void ctrl_radio_ptt_1_clr(void);
void ctrl_radio_ptt_2_set(void);
void ctrl_radio_ptt_2_clr(void);

void ctrl_radio_inhibit_set(void);
void ctrl_radio_inhibit_clr(void);

void ctrl_ant_set(void);
void ctrl_ant_clr(void);

void ctrl_fsk_tx_enable_set(void);
void ctrl_fsk_tx_enable_clr(void);

uint8_t ctrl_buttons_get_status(void);

uint8_t ctrl_get_radio_state(void);

uint8_t ctrl_conf_get_state(void);

void ctrl_backlight_init(uint8_t red, uint8_t green, uint8_t blue);
void ctrl_backlight_set_rgb(uint8_t red, uint8_t green, uint8_t blue);

void ctrl_radio_input_source_low(void);
void ctrl_radio_input_source_high(void);

void ctrl_aux_relay_set(uint16_t conf);
void ctrl_aux_relay_clr(uint16_t conf);

#endif /* end __CTRL_H */

/****************************************************************************
**                            End Of File
*****************************************************************************/
