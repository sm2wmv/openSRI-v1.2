#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <QDebug>

#include "status.h"

#define UPDATE_TO_DEVICE(x) if (serialPort->isOpen()) newDataToDevice((quint8 *)&x, (quint16)((quint64)&x - (qint64)&status), sizeof(x))

Status::Status() {

}

void Status::setSerialPtr(CommClass *ptr) {
  serialPort = ptr;
}

void Status::newDataToDevice(quint8 *data, quint16 offset, quint8 length) {
  serialPort->addTXMessageSendStruct(SRI_CMD_SEND_STATUS, length, offset, (quint8 *)data);
}

void Status::newDataToStruct(quint8 *data, quint16 offset, quint8 length) {
  unsigned char *ptr;
  ptr = (unsigned char *)&status;

  for (unsigned char i=0;i<length;i++) {
    *(ptr+offset+i) = data[i];
  }
}

quint32 Status::getVFOAFreq(void) {
  return(status.radio.vfoA_freq);
}

quint32 Status::getVFOBFreq(void) {
  return(status.radio.vfoB_freq);
}

quint8 Status::getVFOAMode(void) {
  return(status.radio.vfoA_mode);
}

quint8 Status::getVFOAModeType(void) {
  return(status.radio.vfoA_mode_type);
}

quint8 Status::getVFOBMode(void) {
  return(status.radio.vfoB_mode);
}

quint8 Status::getVFOBModeType(void) {
  return(status.radio.vfoB_mode_type);
}

quint8 Status::getVFOABTXRXState(void) {
  return(status.radio.vfoAB_tx_rx_state);
}

bool Status::getPTTInputState(quint8 bitNr) {
  if (status.ptt.ptt_input_state & (1<<bitNr))
    return(true);

  return(false);
}

bool Status::getPTTOutputState(quint8 bitNr) {
  if (status.ptt.ptt_output_state & (1<<bitNr))
    return(true);

  return(false);
}

quint8 Status::getWinkeyPotSpeed() {
  return(status.winkey.pot_speed);
}
