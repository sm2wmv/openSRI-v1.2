#ifndef __WINKEY_H
#define __WINKEY_H

#include <stdio.h>
#include "lpc17xx.h"

#define WINKEY_REV_NR 23

#define WINKEY_DEFAULT_SPEED  28

//! The size of the Winkey RX buffer
#define WINKEY_RX_BUF_SIZE  64
//! The size of the Winkey TX buffer
#define WINKEY_TX_BUF_SIZE  64

//! Winkey command set
#define WINKEY_CMD_ADMIN              0x00
#define WINKEY_CMD_SIDETONE_FREQ      0x01
#define WINKEY_CMD_SPEED              0x02
#define WINKEY_CMD_WEIGHT             0x03
#define WINKEY_CMD_PTT_LEAD_TAIL      0x04
#define WINKEY_CMD_SPEED_POT_SETUP    0x05
#define WINKEY_CMD_PAUSE              0x06
#define WINKEY_CMD_GET_SPEED_POT      0x07
#define WINKEY_CMD_BACKSPACE          0x08
#define WINKEY_CMD_PIN_CONF           0x09
#define WINKEY_CMD_CLEAR_BUFFER       0x0A
#define WINKEY_CMD_KEY_IMMIDIATE      0x0B
#define WINKEY_CMD_HSCW_SPEED         0x0C
#define WINKEY_CMD_FARNSWORTH         0x0D
#define WINKEY_CMD_WINKEY2_MODE       0x0E
#define WINKEY_CMD_LOAD_DEFAULTS      0x0F
#define WINKEY_CMD_FIRST_EXTENSION    0x10
#define WINKEY_CMD_KEY_COMPENSATION   0x11
#define WINKEY_CMD_PADDE_SWITCHPOINT  0x12
#define WINKEY_CMD_NULL               0x13
#define WINKEY_CMD_SW_PADDLE_INPUT    0x14
#define WINKEY_CMD_WINKEY2_STATUS     0x15
#define WINKEY_CMD_BUFFER_POINTER     0x16
#define WINKEY_CMD_DIH_DAH_RATIO      0x17
#define WINKEY_CMD_PTT_CONTROL        0x18
#define WINKEY_CMD_TIMED_KEY_DOWN     0x19
#define WINKEY_CMD_WAIT               0x1A
#define WINKEY_CMD_MERGE_LETTERS      0x1B
#define WINKEY_CMD_SPEED_CHANGE       0x1C
#define WINKEY_CMD_PORT_SELECT        0x1D
#define WINKEY_CMD_CANCEL_BUFF_SPEED  0x1E
#define WINKEY_CMD_BUFFERED_NOP       0x1F

#define WINKEY_ADMIN_CMD_RESET          01
#define WINKEY_ADMIN_CMD_HOST_OPEN      02
#define WINKEY_ADMIN_CMD_HOST_CLOSE     03
#define WINKEY_ADMIN_CMD_ECHO           04
#define WINKEY_ADMIN_CMD_WK2            11
#define WINKEY_ADMIN_CMD_DUMP_EEPROM    12
#define WINKEY_ADMIN_CMD_LOAD_EEPROM    13

//! Waiting for an echo request from the WK chip
#define WINKEY_STATE_NORMAL               0x01
//! Initialize the chip
#define WINKEY_STATE_ECHO_REQ             0x02
//! We are waiting for the revision reply
#define WINKEY_STATE_REV_REPLY            0x03
//! Get the speed pot value
#define WINKEY_STATE_GET_SPEED_POT_VAL    0x04
//! Undefined
#define WINKEY_STATE_FAILED               0xFF

void winkey_init(void);
void winkey_send_char(uint8_t data);
void  winkey_send_data(uint8_t length, uint8_t *data);
void winkey_rx_char(uint8_t data);
void winkey_1ms_tick(void);

void winkey_setting_changed(uint8_t cmd);
uint8_t winkey_init_sequence(uint8_t state);

uint8_t winkey_get_initialized(void);

uint32_t winkey_receive_from_computer(uint8_t *buffer, uint16_t length);
void winkey_send_to_computer(uint8_t length, uint8_t *data);

int16_t winkey_poll_char(void);
void winkey_execute(void);
void winkey_reset(void);

void winkey_set_speed(uint8_t speed);
void winkey_dec_speed(void);
void winkey_inc_speed(void);

void winkey_clear_buffer(void);
void winkey_send_message(uint8_t length, uint8_t *message);

void winkey_reset_speed(void);

#endif
