#include <stdio.h>
#include <string.h>

#include "status.h"
#include "comm_interface.h"
#include "commands.h"
#include "computer_interface.h"
#include "misc.h"
#include "display.h"
#include "event_handler.h"

struct_status status;

// Sends status data to the computer
void status_update_to_computer(uint8_t *data, uint16_t offset, uint8_t length) {
  if (computer_interface_connection_status())
    comm_interface_add_tx_struct_message(SRI_CMD_SEND_STATUS, length, offset, data);
}

void status_set_vfoA_mode(uint8_t mode) {
  if (mode != status.radio.vfoA_mode) {
    status.radio.vfoA_mode = mode;
    status.radio.vfoA_mode_type = status_get_vfoA_mode_type();

    STATUS_UPDATE_TO_COMPUTER(status.radio);
    event_handler_radio_status_changed(&(status.radio));
  }
}

void status_set_vfoB_mode(uint8_t mode) {
  if (mode != status.radio.vfoB_mode) {
    status.radio.vfoB_mode = mode;
    status.radio.vfoB_mode_type = status_get_vfoB_mode_type();

    STATUS_UPDATE_TO_COMPUTER(status.radio);
    event_handler_radio_status_changed(&(status.radio));
  }
}

uint8_t status_get_vfoA_mode(void) {
  return(status.radio.vfoA_mode);
}

uint8_t status_get_vfoB_mode(void) {
  return(status.radio.vfoB_mode);
}

uint8_t status_get_vfoA_mode_type(void) {
  uint8_t mode = status.radio.vfoA_mode;

  if (mode == STATUS_RADIO_MODE_CW)
    return(STATUS_RADIO_MODE_TYPE_CW);
  else if (mode == STATUS_RADIO_MODE_CWR)
    return(STATUS_RADIO_MODE_TYPE_CW);
  else if (mode == STATUS_RADIO_MODE_LSB)
    return(STATUS_RADIO_MODE_TYPE_PHONE);
  else if (mode == STATUS_RADIO_MODE_USB)
    return(STATUS_RADIO_MODE_TYPE_PHONE);
  else if (mode == STATUS_RADIO_MODE_FM)
    return(STATUS_RADIO_MODE_TYPE_PHONE);
  else if (mode == STATUS_RADIO_MODE_AM)
    return(STATUS_RADIO_MODE_TYPE_PHONE);
  else if (mode == STATUS_RADIO_MODE_LSB)
    return(STATUS_RADIO_MODE_TYPE_PHONE);
  else if (mode == STATUS_RADIO_MODE_FSK)
    return(STATUS_RADIO_MODE_TYPE_DIGITAL);
  else if (mode == STATUS_RADIO_MODE_PKTL)
    return(STATUS_RADIO_MODE_TYPE_DIGITAL);
  else if (mode == STATUS_RADIO_MODE_FSKR)
    return(STATUS_RADIO_MODE_TYPE_DIGITAL);
  else if (mode == STATUS_RADIO_MODE_PKTFM)
    return(STATUS_RADIO_MODE_TYPE_DIGITAL);
  else if (mode == STATUS_RADIO_MODE_FMN)
    return(STATUS_RADIO_MODE_TYPE_PHONE);
  else if (mode == STATUS_RADIO_MODE_PKTU)
    return(STATUS_RADIO_MODE_TYPE_DIGITAL);

  return(STATUS_RADIO_MODE_TYPE_UNKNOWN);
}

uint8_t status_get_vfoB_mode_type(void) {
  uint8_t mode = status.radio.vfoB_mode;

  if (mode == STATUS_RADIO_MODE_CW)
    return(STATUS_RADIO_MODE_TYPE_CW);
  else if (mode == STATUS_RADIO_MODE_CWR)
    return(STATUS_RADIO_MODE_TYPE_CW);
  else if (mode == STATUS_RADIO_MODE_LSB)
    return(STATUS_RADIO_MODE_TYPE_PHONE);
  else if (mode == STATUS_RADIO_MODE_USB)
    return(STATUS_RADIO_MODE_TYPE_PHONE);
  else if (mode == STATUS_RADIO_MODE_FM)
    return(STATUS_RADIO_MODE_TYPE_PHONE);
  else if (mode == STATUS_RADIO_MODE_AM)
    return(STATUS_RADIO_MODE_TYPE_PHONE);
  else if (mode == STATUS_RADIO_MODE_LSB)
    return(STATUS_RADIO_MODE_TYPE_PHONE);
  else if (mode == STATUS_RADIO_MODE_FSK)
    return(STATUS_RADIO_MODE_TYPE_DIGITAL);
  else if (mode == STATUS_RADIO_MODE_PKTL)
    return(STATUS_RADIO_MODE_TYPE_DIGITAL);
  else if (mode == STATUS_RADIO_MODE_FSKR)
    return(STATUS_RADIO_MODE_TYPE_DIGITAL);
  else if (mode == STATUS_RADIO_MODE_PKTFM)
    return(STATUS_RADIO_MODE_TYPE_DIGITAL);
  else if (mode == STATUS_RADIO_MODE_FMN)
    return(STATUS_RADIO_MODE_TYPE_PHONE);
  else if (mode == STATUS_RADIO_MODE_PKTU)
    return(STATUS_RADIO_MODE_TYPE_DIGITAL);

  return(STATUS_RADIO_MODE_TYPE_UNKNOWN);
}

uint8_t status_get_vfo_mode(void) {
  uint8_t state = status_get_vfoAB_txrx_state();

  if ((state & (1<<STATUS_VFO_TXRX_VFOA_TX)) != 0)
    return(status_get_vfoA_mode());
  else if ((state & (1<<STATUS_VFO_TXRX_VFOB_TX)) != 0)
    return(status_get_vfoB_mode());

  return(status_get_vfoA_mode());
}

uint8_t status_get_vfo_mode_type(void) {
  uint8_t state = status_get_vfoAB_txrx_state();

  if ((state & (1<<STATUS_VFO_TXRX_VFOA_TX)) != 0)
    return(status_get_vfoA_mode_type());
  else if ((state & (1<<STATUS_VFO_TXRX_VFOB_TX)) != 0)
    return(status_get_vfoB_mode_type());

  return(status_get_vfoA_mode_type());
}

void status_update_from_computer(uint8_t *data, uint16_t offset, uint8_t length) {
  uint8_t *ptr = (uint8_t *)&status;

  for (uint8_t i=0;i<length;i++)
    *(ptr+offset+i) = data[i];
}

void status_execute_update(void) {
  STATUS_UPDATE_TO_COMPUTER(status);
  display_update();
}


void status_set_vfoA_freq(uint32_t freq) {
  if (freq != status.radio.vfoA_freq) {
    status.radio.vfoA_freq = freq;

    STATUS_UPDATE_TO_COMPUTER(status.radio.vfoA_freq);
    event_handler_radio_status_changed(&(status.radio));
  }
}

void status_set_vfoB_freq(uint32_t freq) {
  if (freq != status.radio.vfoB_freq) {
    status.radio.vfoB_freq = freq;

    STATUS_UPDATE_TO_COMPUTER(status.radio.vfoB_freq);
    event_handler_radio_status_changed(&(status.radio));
  }
}

void status_set_vfoAB_txrx_state(uint8_t state) {
  if (state != status.radio.vfoAB_tx_rx_state) {
    status.radio.vfoAB_tx_rx_state = state;

    STATUS_UPDATE_TO_COMPUTER(status.radio.vfoAB_tx_rx_state);
    event_handler_radio_status_changed(&(status.radio));
  }
}

uint32_t status_get_vfo_freq(void) {
  uint8_t state = status_get_vfoAB_txrx_state();

  if ((state & (1<<STATUS_VFO_TXRX_VFOB_TX)) != 0)
    return(status_get_vfoB_freq());

  return(status_get_vfoA_freq());
}

uint32_t status_get_vfoA_freq(void) {
  return(status.radio.vfoA_freq);
}

uint32_t status_get_vfoB_freq(void) {
  return(status.radio.vfoB_freq);
}

uint8_t status_get_vfoAB_txrx_state(void) {
  return(status.radio.vfoAB_tx_rx_state);
}

void status_set_ptt_input_state_clr(uint8_t bit_nr) {
  status.ptt.ptt_input_state &= ~(1<<bit_nr);

  STATUS_UPDATE_TO_COMPUTER(status.ptt.ptt_input_state);
}

void status_set_ptt_input_state_set(uint8_t bit_nr) {
  status.ptt.ptt_input_state |= (1<<bit_nr);

  STATUS_UPDATE_TO_COMPUTER(status.ptt.ptt_input_state);
}

void status_set_ptt_input_state(uint8_t state) {
  status.ptt.ptt_input_state = state;

  STATUS_UPDATE_TO_COMPUTER(status.ptt.ptt_input_state);
}

uint8_t status_get_ptt_input_state(uint8_t bit_nr) {
  return((status.ptt.ptt_input_state & (1<<bit_nr))>>bit_nr);
}

void status_set_winkey_pot_speed(uint8_t speed) {
  if (speed != status.winkey.pot_speed) {
    status.winkey.pot_speed = speed;

    STATUS_UPDATE_TO_COMPUTER(status.winkey.pot_speed);
    display_update();
  }
}

uint8_t status_get_winkey_pot_speed() {
  return(status.winkey.pot_speed);
}

void status_set_winkey_transmitted_character(uint8_t c) {
  if (computer_interface_connection_status()) {
    uint8_t data[2];
    data[0] = SRI_CMD_SUB_WINKEY_CHARS_SENT;
    data[1] = c;

    comm_interface_add_tx_message(SRI_CMD_WINKEY, 2, data);
  }

  //display_update_winkey_tx_char(c);
}
