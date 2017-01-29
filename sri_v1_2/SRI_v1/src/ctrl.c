#include <stdio.h>

#include "LPC17xx.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "ctrl.h"
#include "board.h"
#include "main.h"
#include "misc.h"

//#define CTRL_DEBUG

unsigned char io_exp_a0_out_status = 0xFF;
unsigned char io_exp_a1_out_status = (1<<6) | (1<<4) | (1<<5) | (1<<3);
unsigned char io_exp_a2_out_status = (1<<6) | (1<<7);
unsigned char io_exp_a3_out_status = 0xFF;

unsigned char io_exp_a4_out_status = 0x00;
unsigned char io_exp_a5_out_status = 0x00;
unsigned char io_exp_a6_out_status = 0xFF;
unsigned char io_exp_a7_out_status = 0xFF;
unsigned char io_exp_a8_out_status = 0xFF;

uint8_t ctrl_i2c_io_exp_a0_get(void) {
	unsigned char ret;

	I2C_M_SETUP_Type transferMCfg;
	transferMCfg.sl_addr7bit = IO_EXPA0_ADDR >> 1;
	transferMCfg.tx_data = NULL;
	transferMCfg.tx_length = 0;
	transferMCfg.rx_data = &ret;
	transferMCfg.rx_length = 1;
	transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

	I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

	return(ret);
}

uint8_t ctrl_i2c_io_exp_a1_get(void) {
  unsigned char ret;

  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = IO_EXPA1_ADDR >> 1;
  transferMCfg.tx_data = NULL;
  transferMCfg.tx_length = 0;
  transferMCfg.rx_data = &ret;
  transferMCfg.rx_length = 1;
  transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  return(ret);
}

uint8_t ctrl_i2c_io_exp_a2_get(void) {
	unsigned char ret;

	I2C_M_SETUP_Type transferMCfg;
	transferMCfg.sl_addr7bit = IO_EXPA2_ADDR >> 1;
	transferMCfg.tx_data = NULL;
	transferMCfg.tx_length = 0;
	transferMCfg.rx_data = &ret;
	transferMCfg.rx_length = 1;
	transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

	I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

	return(ret);
}

uint8_t ctrl_i2c_io_exp_a3_get(void) {
	unsigned char ret;

	I2C_M_SETUP_Type transferMCfg;
	transferMCfg.sl_addr7bit = IO_EXPA3_ADDR >> 1;
	transferMCfg.tx_data = NULL;
	transferMCfg.tx_length = 0;
	transferMCfg.rx_data = &ret;
	transferMCfg.rx_length = 1;
	transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

	I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

	return(ret);
}

uint8_t ctrl_i2c_io_exp_a4_get(void) {
  unsigned char ret;

  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = IO_EXPA4_ADDR >> 1;
  transferMCfg.tx_data = NULL;
  transferMCfg.tx_length = 0;
  transferMCfg.rx_data = &ret;
  transferMCfg.rx_length = 1;
  transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  return(ret);
}

uint8_t ctrl_i2c_io_exp_a5_get(void) {
  unsigned char ret;

  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = IO_EXPA5_ADDR >> 1;
  transferMCfg.tx_data = NULL;
  transferMCfg.tx_length = 0;
  transferMCfg.rx_data = &ret;
  transferMCfg.rx_length = 1;
  transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  return(ret);
}

uint8_t ctrl_i2c_io_exp_a6_get(void) {
  unsigned char ret;

  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = IO_EXPA6_ADDR >> 1;
  transferMCfg.tx_data = NULL;
  transferMCfg.tx_length = 0;
  transferMCfg.rx_data = &ret;
  transferMCfg.rx_length = 1;
  transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  return(ret);
}

uint8_t ctrl_i2c_io_exp_a7_get(void) {
  unsigned char ret;

  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = IO_EXPA7_ADDR >> 1;
  transferMCfg.tx_data = NULL;
  transferMCfg.tx_length = 0;
  transferMCfg.rx_data = &ret;
  transferMCfg.rx_length = 1;
  transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  return(ret);
}

uint8_t ctrl_i2c_io_exp_a8_get(void) {
  unsigned char ret;

  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = IO_EXPA8_ADDR >> 1;
  transferMCfg.tx_data = NULL;
  transferMCfg.tx_length = 0;
  transferMCfg.rx_data = &ret;
  transferMCfg.rx_length = 1;
  transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  return(ret);
}

uint8_t ctrl_i2c_io_exp_a0_set(void) {
	I2C_M_SETUP_Type transferMCfg;
	transferMCfg.sl_addr7bit = IO_EXPA0_ADDR >> 1;
	transferMCfg.tx_data = &io_exp_a0_out_status;
	transferMCfg.tx_length = 1;
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;
	transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

	return(I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING));
}

uint8_t ctrl_i2c_io_exp_a1_set(void) {
	I2C_M_SETUP_Type transferMCfg;
	transferMCfg.sl_addr7bit = IO_EXPA1_ADDR >> 1;
	transferMCfg.tx_data = &io_exp_a1_out_status;
	transferMCfg.tx_length = 1;
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;
	transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

	return(I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING));
}

uint8_t ctrl_i2c_io_exp_a2_set(void) {
	I2C_M_SETUP_Type transferMCfg;
	transferMCfg.sl_addr7bit = IO_EXPA2_ADDR >> 1;
	transferMCfg.tx_data = &io_exp_a2_out_status;
	transferMCfg.tx_length = 1;
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;
	transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

	return(I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING));
}

uint8_t ctrl_i2c_io_exp_a3_set(void) {
	I2C_M_SETUP_Type transferMCfg;
	transferMCfg.sl_addr7bit = IO_EXPA3_ADDR >> 1;
	transferMCfg.tx_data = &io_exp_a3_out_status;
	transferMCfg.tx_length = 1;
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;
	transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

	return(I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING));
}

uint8_t ctrl_i2c_io_exp_a4_set(void) {
  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = IO_EXPA4_ADDR >> 1;
  transferMCfg.tx_data = &io_exp_a4_out_status;
  transferMCfg.tx_length = 1;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

  return(I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING));
}

uint8_t ctrl_i2c_io_exp_a5_set(void) {
  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = IO_EXPA5_ADDR >> 1;
  transferMCfg.tx_data = &io_exp_a5_out_status;
  transferMCfg.tx_length = 1;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

  return(I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING));
}

uint8_t ctrl_i2c_io_exp_a6_set(void) {
  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = IO_EXPA6_ADDR >> 1;
  transferMCfg.tx_data = &io_exp_a6_out_status;
  transferMCfg.tx_length = 1;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

  return(I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING));
}

uint8_t ctrl_i2c_io_exp_a7_set(void) {
  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = IO_EXPA7_ADDR >> 1;
  transferMCfg.tx_data = &io_exp_a7_out_status;
  transferMCfg.tx_length = 1;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

  return(I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING));
}

uint8_t ctrl_i2c_io_exp_a8_set(void) {
  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = IO_EXPA8_ADDR >> 1;
  transferMCfg.tx_data = &io_exp_a8_out_status;
  transferMCfg.tx_length = 1;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = CTRL_IO_EXP_RETRANSMISSIONS_MAX;

  return(I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING));
}

void ctrl_radio_cat_rts_set(void) {
	LPC1765_CAT_RTS_PORT->FIOSET = (1<<LPC1765_CAT_RTS);
}

void ctrl_radio_cat_rts_clr(void) {
	LPC1765_CAT_RTS_PORT->FIOCLR = (1<<LPC1765_CAT_RTS);
}

uint8_t ctrl_radio_cat_cts_get(void) {
	return(LPC1765_CAT_CTS_PORT->FIOPIN & (1<<LPC1765_CAT_CTS));
}

uint8_t ctrl_computer_cat_rts_get(void) {
	return(FT4232_CAT_RTS_PORT->FIOPIN & (1<<FT4232_CAT_RTS));
}

void ctrl_computer_cat_cts_set(void) {
	FT4232_CAT_CTS_PORT->FIOSET = (1<<FT4232_CAT_CTS);
}

void ctrl_computer_cat_cts_clr(void) {
	FT4232_CAT_CTS_PORT->FIOCLR = (1<<FT4232_CAT_CTS);
}

void ctrl_fsk_tx_enable_set(void) {
  io_exp_a1_out_status |= (1<<7);
  ctrl_i2c_io_exp_a1_set();
}

void ctrl_fsk_tx_enable_clr(void) {
  io_exp_a1_out_status &= ~(1<<7);
  ctrl_i2c_io_exp_a1_set();
}

void ctrl_ptt_amp_set(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: PTT AMP SET\n");
  #endif

    PTT_AMP_PORT->FIOSET = (1<<PTT_AMP);
}

void ctrl_ptt_amp_clr(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: PTT AMP CLR\n");
  #endif

  PTT_AMP_PORT->FIOCLR = (1<<PTT_AMP);
}

void ctrl_radio_ptt_1_set(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: RADIO PTT 1 SET\n");
  #endif

    RADIO_PTT_1_PORT->FIOSET = (1<<RADIO_PTT_1);
}

void ctrl_radio_ptt_1_clr(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: RADIO PTT 1 CLR\n");
  #endif

    RADIO_PTT_1_PORT->FIOCLR = (1<<RADIO_PTT_1);
}

void ctrl_radio_ptt_2_set(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: RADIO PTT 2 SET\n");
  #endif

    RADIO_PTT_2_PORT->FIOSET = (1<<RADIO_PTT_2);
}

void ctrl_radio_ptt_2_clr(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: RADIO PTT 2 CLR\n");
  #endif

    RADIO_PTT_2_PORT->FIOCLR = (1<<RADIO_PTT_2);
}

void ctrl_radio_fsk_set(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: RADIO FSK SET\n");
  #endif

  RADIO_FSK_PORT->FIOSET = (1<<RADIO_FSK);
}

void ctrl_radio_fsk_clr(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: RADIO FSK CLR\n");
  #endif

  RADIO_FSK_PORT->FIOCLR = (1<<RADIO_FSK);
}

void ctrl_radio_cw_set(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: RADIO CW SET\n");
  #endif

  RADIO_CW_PORT->FIOSET = (1<<RADIO_CW);
}

void ctrl_radio_cw_clr(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: RADIO CW CLR\n");
  #endif

  RADIO_CW_PORT->FIOCLR = (1<<RADIO_CW);
}

void ctrl_radio_inhibit_set(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: RADIO INHIBIT SET\n");
  #endif

  RADIO_INHIBIT_PORT->FIOSET = (1<<RADIO_INHIBIT);
}

void ctrl_radio_inhibit_clr(void) {
#ifdef CTRL_DEBUG
  PRINTF("CTRL: RADIO INHIBIT CLR\n");
#endif

RADIO_INHIBIT_PORT->FIOCLR = (1<<RADIO_INHIBIT);
}

void ctrl_ant_set(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: ANT SET\n");
  #endif

  FLOOR_AUX0_PORT->FIOSET = (1<<FLOOR_AUX0);
}

void ctrl_ant_clr(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: ANT CLR\n");
  #endif

  FLOOR_AUX0_PORT->FIOCLR = (1<<FLOOR_AUX0);
}

void ctrl_mic_relay_set(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: MIC RELAY SET\n");
  #endif

  io_exp_a2_out_status |= (1<<0);

  ctrl_i2c_io_exp_a2_set();
}

void ctrl_mic_relay_clr(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: MIC RELAY CLR\n");
  #endif

  io_exp_a2_out_status &= ~(1<<0);

  ctrl_i2c_io_exp_a2_set();
}

void ctrl_mic_ext_preamp_relay_set(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: EXT PREAMP RELAY SET\n");
  #endif

  io_exp_a2_out_status |= (1<<3);

  ctrl_i2c_io_exp_a2_set();
}

void ctrl_mic_ext_preamp_relay_clr(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: EXT PREAMP RELAY CLR\n");
  #endif

  io_exp_a2_out_status &= ~(1<<3);

  ctrl_i2c_io_exp_a2_set();
}

/*! \brief Selects the low level audio input as input source for the main VFO */
void ctrl_radio_input_source_low(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: RADIO INPUT SOURCE LOW\n");
  #endif

  io_exp_a1_out_status &= ~(1<<0);

  ctrl_i2c_io_exp_a1_set();
}

/*! \brief Selects the AF (high) level audio input as input source for both VFO */
void ctrl_radio_input_source_high(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: RADIO INPUT SOURCE HIGH\n");
  #endif

  io_exp_a1_out_status |= (1<<0);

  ctrl_i2c_io_exp_a1_set();
}

/*! \brief Set the FT4232 reset pin high */
void ctrl_ft4232_reset_set(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: FT4232 RESET SET\n");
  #endif

  io_exp_a1_out_status |= (1<<6);

  ctrl_i2c_io_exp_a1_set();
}

/*! \brief Set the FT4232 reset pin low */
void ctrl_ft4232_reset_clr(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: FT4232 RESET CLR\n");
  #endif

  io_exp_a1_out_status &= ~(1<<6);

  ctrl_i2c_io_exp_a1_set();
}

/*! \brief Set the HUB reset pin high */
void ctrl_hub_reset_set(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: USB HUB RESET SET\n");
  #endif

  io_exp_a1_out_status |= (1<<5);

  ctrl_i2c_io_exp_a1_set();
}

/*! \brief Set the HUB reset pin low */
void ctrl_hub_reset_clr(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: USB HUB RESET CLR\n");
  #endif

  io_exp_a1_out_status &= ~(1<<5);

  ctrl_i2c_io_exp_a1_set();
}

void ctrl_pcm2912_mic_preamp_on(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: PCM2912 MIC PREAMP ON\n");
  #endif

  io_exp_a2_out_status |= (1<<4);

  ctrl_i2c_io_exp_a2_set();
}

void ctrl_pcm2912_mic_preamp_off(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: PCM2912 MIC PREAMP OFF\n");
  #endif

  io_exp_a2_out_status &= ~(1<<4);

  ctrl_i2c_io_exp_a2_set();
}

void ctrl_pcm2912_mic_mute_on(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: PCM2912 MIC MUTE ON\n");
  #endif

  io_exp_a2_out_status |= (1<<5);

  ctrl_i2c_io_exp_a2_set();
}

void ctrl_pcm2912_mic_mute_off(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: PCM2912 MIC MUTE OFF\n");
  #endif

  io_exp_a2_out_status &= ~(1<<5);

  ctrl_i2c_io_exp_a2_set();
}

void ctrl_led_fsk_on(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED FSK ON\n");
  #endif

  io_exp_a0_out_status &= ~(1<<6);
  ctrl_i2c_io_exp_a0_set();
}

void ctrl_led_fsk_off(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED FSK OFF\n");
  #endif

  io_exp_a0_out_status |= (1<<6);
  ctrl_i2c_io_exp_a0_set();
}

void ctrl_led_cw_on(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED CW ON\n");
  #endif

  FRONT_LED_CW_PORT->FIOCLR = (1<<FRONT_LED_CW);
}

void ctrl_led_cw_off(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED CW OFF\n");
  #endif

  FRONT_LED_CW_PORT->FIOSET = (1<<FRONT_LED_CW);
}


void ctrl_led_pwr_on(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED PWR ON\n");
  #endif

  io_exp_a0_out_status &= ~(1<<7);
  ctrl_i2c_io_exp_a0_set();
}

void ctrl_led_pwr_off(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED PWR OFF\n");
  #endif

  io_exp_a0_out_status |= (1<<7);
  ctrl_i2c_io_exp_a0_set();
}

void ctrl_led_ptt_on(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED PTT ON\n");
  #endif

  ctrl_led_inhibit_off();

  io_exp_a0_out_status &= ~(1<<0);
  ctrl_i2c_io_exp_a0_set();
}

void ctrl_led_ptt_off(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED PTT OFF\n");
  #endif

  io_exp_a0_out_status |= (1<<0);
  ctrl_i2c_io_exp_a0_set();
}

void ctrl_led_inhibit_on(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED INHIBIT ON\n");
  #endif

  ctrl_led_ptt_off();

  io_exp_a3_out_status &= ~(1<<4);
  ctrl_i2c_io_exp_a3_set();
}

void ctrl_led_inhibit_off(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED INHIBIT OFF\n");
  #endif

  io_exp_a3_out_status |= (1<<4);
  ctrl_i2c_io_exp_a3_set();
}

void ctrl_led_button2_on(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED BUTTON2 ON\n");
  #endif

  io_exp_a3_out_status &= ~(1<<0);
  ctrl_i2c_io_exp_a3_set();
}

void ctrl_led_button2_off(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED BUTTON2 OFF\n");
  #endif

  io_exp_a3_out_status |= (1<<0);
  ctrl_i2c_io_exp_a3_set();
}

void ctrl_led_button3_on(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED BUTTON3 ON\n");
  #endif

  io_exp_a3_out_status &= ~(1<<1);
  ctrl_i2c_io_exp_a3_set();
}

void ctrl_led_button3_off(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED BUTTON3 OFF\n");
  #endif

  io_exp_a3_out_status |= (1<<1);
  ctrl_i2c_io_exp_a3_set();
}

void ctrl_led_button4_on(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED BUTTON4 ON\n");
  #endif

  io_exp_a3_out_status &= ~(1<<2);
  ctrl_i2c_io_exp_a3_set();
}

void ctrl_led_button4_off(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED BUTTON4 OFF\n");
  #endif

  io_exp_a3_out_status |= (1<<2);
  ctrl_i2c_io_exp_a3_set();
}

void ctrl_led_aux_on(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED AUX ON\n");
  #endif

  io_exp_a3_out_status &= ~(1<<3);
  ctrl_i2c_io_exp_a3_set();
}

void ctrl_led_aux_off(void) {
  #ifdef CTRL_DEBUG
    PRINTF("CTRL: LED AUX OFF\n");
  #endif

  io_exp_a3_out_status |= (1<<3);
  ctrl_i2c_io_exp_a3_set();
}

uint8_t ctrl_get_radio_state(void) {
  if ((ctrl_i2c_io_exp_a1_get() & (1<<3)) != 0)
    return(1);

  return(0);
}

uint8_t ctrl_buttons_get_status(void) {
  //Invert the result and return it
  return(~((FRONT_ENCODER_BUTTON_PORT->FIOPIN & (1<<FRONT_ENCODER_BUTTON))>>FRONT_ENCODER_BUTTON));
}

uint8_t ctrl_conf_get_state(void);

void ctrl_backlight_init(uint8_t red, uint8_t green, uint8_t blue) {
  uint8_t data[2];

  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = LP5521_ADDR >> 1;

  delay_ms(10);

  data[0] = 0x00;
  data[1] = (1<<7) | (1<<6);

  transferMCfg.tx_data = data;
  transferMCfg.tx_length = 2;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = 1;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  delay_ms(10);

  data[0] = 0x08;
  data[1] = (1<<3) | (1<<0);

  transferMCfg.tx_data = data;
  transferMCfg.tx_length = 2;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = 1;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);


  data[0] = 0x01;
  data[1] = 0x3F;

  transferMCfg.tx_data = data;
  transferMCfg.tx_length = 2;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = 1;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  data[0] = 0x02;
  data[1] = red;

  transferMCfg.tx_data = data;
  transferMCfg.tx_length = 2;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = 1;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  data[0] = 0x03;
  data[1] = green;

  transferMCfg.tx_data = data;
  transferMCfg.tx_length = 2;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = 1;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  data[0] = 0x04;
  data[1] = blue;

  transferMCfg.tx_data = data;
  transferMCfg.tx_length = 2;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = 1;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);
}

void ctrl_backlight_set_rgb(uint8_t red, uint8_t green, uint8_t blue) {
  uint8_t data[2];

  I2C_M_SETUP_Type transferMCfg;
  transferMCfg.sl_addr7bit = LP5521_ADDR >> 1;

  data[0] = 0x02;
  data[1] = red;

  transferMCfg.tx_data = data;
  transferMCfg.tx_length = 2;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = 1;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  data[0] = 0x03;
  data[1] = green;

  transferMCfg.tx_data = data;
  transferMCfg.tx_length = 2;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = 1;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);

  data[0] = 0x04;
  data[1] = blue;

  transferMCfg.tx_data = data;
  transferMCfg.tx_length = 2;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = 1;

  I2C_MasterTransferData(LPC_I2C0, &transferMCfg, I2C_TRANSFER_POLLING);
}

void ctrl_aux_relay_set(uint16_t conf) {
  uint8_t low = conf & 0xFF;
  uint8_t high = conf >> 8;

  io_exp_a4_out_status |= low;
  PRINTF("AUX RELAY >> I2C response A4: 0x%02X\n",ctrl_i2c_io_exp_a4_set());

  io_exp_a5_out_status |= high;
  PRINTF("AUX RELAY >> I2C response A5: 0x%02X\n",ctrl_i2c_io_exp_a5_set());
}

void ctrl_aux_relay_clr(uint16_t conf) {
  uint8_t low = conf & 0xFF;
  uint8_t high = conf >> 8;

  io_exp_a4_out_status &= ~low;
  PRINTF("AUX RELAY >> I2C response A4: 0x%02X\n",ctrl_i2c_io_exp_a4_set());

  io_exp_a4_out_status &= ~high;
  PRINTF("AUX RELAY >> I2C response A5: 0x%02X\n",ctrl_i2c_io_exp_a5_set());
}
