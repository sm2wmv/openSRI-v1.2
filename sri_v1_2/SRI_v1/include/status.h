#ifndef __STATUS_H
#define __STATUS_H

#ifdef __LPC17XX__
#include <stdio.h>
#include <stdlib.h>
#include "lpc17xx.h"

#define STATUS_UPDATE_TO_COMPUTER(x) status_update_to_computer((uint8_t *)&x, (uint16_t)((uint32_t)&x - (uint32_t)&status), sizeof(x))

#endif

#ifndef __LPC17XX__
  #define int8_t  qint8
  #define int16_t qint16
  #define int32_t qint32
  #define int64_t qint64
#endif

//! Bit is set if VFO A has got TX focus
#define STATUS_VFO_TXRX_VFOA_TX             0
//! Bit is set if VFO B has got TX focus
#define STATUS_VFO_TXRX_VFOB_TX             1
//! Bit is set if VFO A has got RX focus
#define STATUS_VFO_TXRX_VFOA_RX             2
//! Bit is set if VFO A has got RX Muted
#define STATUS_VFO_TXRX_VFOA_MUTE           3
//! Bit is set if VFO B has got RX focus
#define STATUS_VFO_TXRX_VFOB_RX             4
//! Bit is set if VFO B has got RX Muted
#define STATUS_VFO_TXRX_VFOB_MUTE           5

//! The type of mode the radio has got is CW
#define STATUS_RADIO_MODE_TYPE_UNKNOWN  0
//! The type of mode the radio has got is CW
#define STATUS_RADIO_MODE_TYPE_CW       1
//! The type of mode the radio has got is PHONE
#define STATUS_RADIO_MODE_TYPE_PHONE    2
//! The type of mode the radio has got is DIGITAL
#define STATUS_RADIO_MODE_TYPE_DIGITAL  3

//! This is set if the radio mode is unknown
#define STATUS_RADIO_MODE_UNKNOWN     0
//! This is set if radio is in LSB
#define STATUS_RADIO_MODE_LSB         1
//! This is set if radio is in USB
#define STATUS_RADIO_MODE_USB         2
//! This is set if radio is in CW
#define STATUS_RADIO_MODE_CW          3
//! This is set if radio is in FM
#define STATUS_RADIO_MODE_FM          4
//! This is set if radio is in AM
#define STATUS_RADIO_MODE_AM          5
//! This is set if radio is in FSK
#define STATUS_RADIO_MODE_FSK         6
//! This is set if radio is in CW-R
#define STATUS_RADIO_MODE_CWR         7
//! This is set if radio is in PKT-L
#define STATUS_RADIO_MODE_PKTL        8
//! This is set if radio is in FSK-R
#define STATUS_RADIO_MODE_FSKR        9
//! This is set if radio is in PKT-FM
#define STATUS_RADIO_MODE_PKTFM       10
//! This is set if radio is in FM-N
#define STATUS_RADIO_MODE_FMN         11
//! This is set if radio is in PKT-U
#define STATUS_RADIO_MODE_PKTU        12

//! Bit which shows PTT input via the Footswitch
#define STATUS_PTT_INPUT_FOOTSWITCH    0
//! Bit which shows PTT input via the Winkey ports RTS
#define STATUS_PTT_INPUT_WK_RTS        1
//! Bit which shows PTT input via the Winkey ports DTR
#define STATUS_PTT_INPUT_WK_DTR        2
//! Bit which shows PTT input via the FSK/CW port RTS
#define STATUS_PTT_INPUT_FSKCW_RTS     3
//! Bit which shows PTT input via VOX sense input
#define STATUS_PTT_INPUT_VOX_SENSE     4
//! Bit which shows PTT input via VOX sense input
#define STATUS_PTT_INPUT_WK            5

typedef struct __attribute__ ((aligned(1))) {
  //! VFO A frequency in Hz
  uint32_t vfoA_freq;
  //! VFO B frequency in Hz
  uint32_t vfoB_freq;
  //! The TX/RX state of the VFOs, see defines above
  uint8_t vfoAB_tx_rx_state;
  //! The current VFO A mode
  uint8_t vfoA_mode;
  //! The current VFO B mode
  uint8_t vfoB_mode;
  //! The current VFO A mode type
  uint8_t vfoA_mode_type;
  //! The current VFO B mode type
  uint8_t vfoB_mode_type;
} struct_radio_status;

typedef struct __attribute__ ((aligned(1))) {
  uint8_t pot_speed;
} struct_winkey_status;

typedef struct __attribute__ ((aligned(1))) {
  uint8_t ptt_input_state;
  uint8_t ptt_output_state;
} struct_ptt_status;

typedef struct __attribute__ ((aligned(1))) {
  struct_radio_status radio;
  struct_ptt_status ptt;
  struct_winkey_status winkey;
} struct_status;

#ifdef __LPC17XX__

void status_update_to_computer(uint8_t *data, uint16_t offset, uint8_t length);
void status_update_from_computer(uint8_t *data, uint16_t offset, uint8_t length);
void status_execute_update(void);

void status_set_vfoA_mode(uint8_t mode);
void status_set_vfoB_mode(uint8_t mode);
uint32_t status_get_vfo_freq(void);
void status_set_vfoA_freq(uint32_t freq);
void status_set_vfoB_freq(uint32_t freq);
void status_set_vfoAB_txrx_state(uint8_t state);

uint8_t status_get_vfoA_mode(void);
uint8_t status_get_vfoB_mode(void);
uint8_t status_get_vfoA_mode_type(void);
uint8_t status_get_vfoB_mode_type(void);
uint8_t status_get_vfo_mode(void);
uint8_t status_get_vfo_mode_type(void);
uint32_t status_get_vfoA_freq(void);
uint32_t status_get_vfoB_freq(void);
uint8_t status_get_vfoAB_txrx_state(void);


void status_set_ptt_input_state_clr(uint8_t bit_nr);
void status_set_ptt_input_state_set(uint8_t bit_nr);
void status_set_ptt_input_state(uint8_t state);
uint8_t status_get_ptt_input_state(uint8_t bit_nr);

void status_set_winkey_pot_speed(uint8_t speed);
uint8_t status_get_winkey_pot_speed();

void status_set_winkey_transmitted_character(uint8_t c);

#endif

#endif /* end __STATUS_STRUCTS_H */
/****************************************************************************
** End Of File
*****************************************************************************/
