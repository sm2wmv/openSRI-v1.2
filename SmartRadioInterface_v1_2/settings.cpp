#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <QDebug>

#include "settings.h"
#include "mainwindow.h"

#define UPDATE_TO_DEVICE(x) if (serialPort->isOpen()) newDataToDevice((quint8 *)&x, (quint16)((quint64)&x - (qint64)&settings), sizeof(x))
#define UPDATE_WINKEY_SETTINGS(x) if (serialPort->isOpen()) serialPort->addTXMessage(SRI_CMD_WINKEY,SRI_CMD_SUB_WINKEY_SETTINGS_CHANGED,x);

#define UPDATE_CW_MESSAGE(index,str)  if (serialPort->isOpen()) {\
                                      QByteArray *txMessage = new QByteArray();\
                                      txMessage->append(SRI_SUB_CMD_SET_CW_MESSAGE);\
                                      txMessage->append(index);\
                                      txMessage->append(str);\
                                      txMessage->append("\0");\
                                      serialPort->addTXMessage(SRI_CMD_CW_MESSAGE,txMessage->count(),txMessage);}

Settings::Settings() {
  for (quint8 i=0;i<SETTING_AUDIO_ROUTE_OFFSET_COUNT;i++) {
    settings.audio.af_input_source_cw[i] = 0;
    settings.audio.af_input_source_digital[i] = 0;
    settings.audio.af_input_source_phone[i] = 0;
    settings.audio.mic_path[i] = 0;
    settings.audio.mic_mute[i] = 0;
    settings.audio.mic_preamp[i] = 0;
  }
}

void Settings::setSerialPtr(CommClass *ptr) {
  serialPort = ptr;
}

void Settings::newDataToDevice(quint8 *data, quint16 offset, quint8 length) {
  serialPort->addTXMessageSendStruct(SRI_CMD_SEND_SETTINGS, length, offset, (quint8 *)data);
}

void Settings::newDataToStruct(quint8 *data, quint16 offset, quint8 length) {
  unsigned char *ptr;
  ptr = (unsigned char *)&settings;

  for (unsigned char i=0;i<length;i++) {
    *(ptr+offset+i) = data[i];
  }
}

void Settings::saveToEEPROM() {
  serialPort->addTXMessage(SRI_CMD_EEPROM_SAVE);
}

/*********** START RADIO SETTINGS ***********/
void Settings::setRadioModel(quint8 value) {
  settings.radio.radio_model = value;

  UPDATE_TO_DEVICE(settings.radio.radio_model);
}

void Settings::setRadioCATInterfaceEnabled(bool state) {
  if (state)
    settings.radio.cat_interface_enabled = 1;
  else
    settings.radio.cat_interface_enabled = 0;

  UPDATE_TO_DEVICE(settings.radio.cat_interface_enabled);
}

void Settings::setRadioCATBaudrate(quint32 value) {
  settings.radio.cat_baudrate = value;

  UPDATE_TO_DEVICE(settings.radio.cat_baudrate);
}

void Settings::setRadioCATStopbits(quint8 value) {
  settings.radio.cat_stopbits = value;

  UPDATE_TO_DEVICE(settings.radio.cat_stopbits);
}

void Settings::setRadioCATParity(quint8 value) {
  settings.radio.cat_parity = value;

  UPDATE_TO_DEVICE(settings.radio.cat_parity);
}

void Settings::setRadioCATICOMCIV(quint8 value) {
  settings.radio.cat_icom_civ = value;

  UPDATE_TO_DEVICE(settings.radio.cat_icom_civ);
}

void Settings::setRadioCATFlowControl(bool state) {
  if (state)
    settings.radio.cat_flow_control = 1;
  else
    settings.radio.cat_flow_control = 0;

  UPDATE_TO_DEVICE(settings.radio.cat_flow_control);
}

void Settings::setRadioCATJumperRTSCTS(bool state) {
  if (state)
    settings.radio.cat_jumper_rts_cts = 1;
  else
    settings.radio.cat_jumper_rts_cts = 0;

  UPDATE_TO_DEVICE(settings.radio.cat_jumper_rts_cts);
}

void Settings::setRadioCATRetrieveType(qint8 value) {
  settings.radio.cat_retrieve_type = value;

  UPDATE_TO_DEVICE(settings.radio.cat_retrieve_type);
}

void Settings::setRadioCATPollRate(qint16 value) {
  settings.radio.cat_poll_rate = value/10;

  UPDATE_TO_DEVICE(settings.radio.cat_poll_rate);
}

quint8 Settings::getRadioModel() {
  return(settings.radio.radio_model);
}

bool Settings::getRadioCATInterfaceEnabled() {
  if (settings.radio.cat_interface_enabled)
    return(true);

  return(false);
}

quint32 Settings::getRadioCATBaudrate() {
  return(settings.radio.cat_baudrate);
}

quint8 Settings::getRadioCATStopbits() {
  return(settings.radio.cat_stopbits);
}

quint8 Settings::getRadioCATParity() {
  return(settings.radio.cat_parity);
}

quint8 Settings::getRadioCATICOMCIV() {
  return(settings.radio.cat_icom_civ);
}

bool Settings::getRadioCATFlowControl() {
  if (settings.radio.cat_flow_control)
    return(true);

  return(false);
}

bool Settings::getRadioCATJumperRTSCTS() {
  if (settings.radio.cat_jumper_rts_cts)
    return(true);

  return(false);
}

quint8 Settings::getRadioCATRetrieveType() {
  return(settings.radio.cat_retrieve_type);
}

quint16 Settings::getRadioCATPollRate() {
  return(settings.radio.cat_poll_rate*10);
}

/*********** END RADIO SETTINGS ***********/

/*********** START PTT SETTINGS ***********/
void Settings::setPTTInputEnabled(quint8 input) {
  settings.ptt.ptt_input_enabled = input;

  UPDATE_TO_DEVICE(settings.ptt.ptt_input_enabled);
}

void Settings::setPTTInputInvert(quint8 input) {
  settings.ptt.ptt_input_invert = input;

  UPDATE_TO_DEVICE(settings.ptt.ptt_input_invert);
}

void Settings::setPTTRadioPreDelay(quint16 delay) {
  settings.ptt.radio_pre_delay = delay/10;

  UPDATE_TO_DEVICE(settings.ptt.radio_pre_delay);
}

void Settings::setPTTRadioPostDelay(quint16 delay) {
  settings.ptt.radio_post_delay = delay/10;

  UPDATE_TO_DEVICE(settings.ptt.radio_post_delay);
}

void Settings::setPTTAmpPreDelay(quint16 delay) {
  settings.ptt.amp_pre_delay = delay/10;

  UPDATE_TO_DEVICE(settings.ptt.amp_pre_delay);
}

void Settings::setPTTAmpPostDelay(quint16 delay) {
  settings.ptt.amp_post_delay = delay/10;

  UPDATE_TO_DEVICE(settings.ptt.amp_post_delay);
}

void Settings::setPTTInhibitPreDelay(quint16 delay) {
  settings.ptt.inhibit_pre_delay = delay/10;

  UPDATE_TO_DEVICE(settings.ptt.inhibit_pre_delay);
}

void Settings::setPTTInhibitPostDelay(quint16 delay) {
  settings.ptt.inhibit_post_delay = delay/10;

  UPDATE_TO_DEVICE(settings.ptt.inhibit_post_delay);
}

void Settings::setPTTAntennaPreDelay(quint16 delay) {
  settings.ptt.antenna_pre_delay = delay/10;

  UPDATE_TO_DEVICE(settings.ptt.antenna_pre_delay);
}

void Settings::setPTTAntennaPostDelay(quint16 delay) {
  settings.ptt.antenna_post_delay = delay/10;

  UPDATE_TO_DEVICE(settings.ptt.antenna_post_delay);
}

void Settings::setPTTOutputCWConf(quint8 value) {
  settings.ptt.radio_cw_ptt = value;

  UPDATE_TO_DEVICE(settings.ptt.radio_cw_ptt);
}

void Settings::setPTTOutputPhoneConf(quint8 value) {
  settings.ptt.radio_phone_ptt = value;

  UPDATE_TO_DEVICE(settings.ptt.radio_phone_ptt);
}

void Settings::setPTTOutputDigitalConf(quint8 value) {
  settings.ptt.radio_digital_ptt = value;

  UPDATE_TO_DEVICE(settings.ptt.radio_digital_ptt);
}

void Settings::setPTTOutputAmplifierEnabled(bool value) {
  if (value)
    settings.ptt.amplifier_enabled = 1;
  else
    settings.ptt.amplifier_enabled = 0;

  UPDATE_TO_DEVICE(settings.ptt.amplifier_enabled);
}

void Settings::setPTTOutputInhibitEnabled(bool value) {
  if (value)
    settings.ptt.inhibit_enabled = 1;
  else
    settings.ptt.inhibit_enabled = 0;

  UPDATE_TO_DEVICE(settings.ptt.inhibit_enabled);
}

void Settings::setPTTOutputInhibitInverted(bool value) {
  if (value)
    settings.ptt.inhibit_inverted = 1;
  else
    settings.ptt.inhibit_inverted = 0;

  UPDATE_TO_DEVICE(settings.ptt.inhibit_inverted);
}

quint8 Settings::getPTTInputEnabled() {
  return(settings.ptt.ptt_input_enabled);
}

quint8 Settings::getPTTInputInvert() {
  return(settings.ptt.ptt_input_invert);
}

bool Settings::getPTTInputEnabled(quint8 bitNr) {
  if (settings.ptt.ptt_input_enabled & (1<<bitNr))
    return(true);

  return(false);
}

bool Settings::getPTTInputInvert(quint8 bitNr) {
  if (settings.ptt.ptt_input_invert & (1<<bitNr))
    return(true);

  return(false);
}

quint16 Settings::getPTTRadioPreDelay() {
  return(settings.ptt.radio_pre_delay*10);
}

quint16 Settings::getPTTRadioPostDelay() {
  return(settings.ptt.radio_post_delay*10);
}

quint16 Settings::getPTTAmpPreDelay() {
  return(settings.ptt.amp_pre_delay*10);
}

quint16 Settings::getPTTAmpPostDelay() {
  return(settings.ptt.amp_post_delay*10);
}

quint16 Settings::getPTTInhibitPreDelay() {
  return(settings.ptt.inhibit_pre_delay*10);
}

quint16 Settings::getPTTInhibitPostDelay() {
  return(settings.ptt.inhibit_post_delay*10);
}

quint16 Settings::getPTTAntennaPreDelay() {
  return(settings.ptt.antenna_pre_delay*10);
}

quint16 Settings::getPTTAntennaPostDelay() {
  return(settings.ptt.antenna_post_delay*10);
}

quint8 Settings::getPTTOutputCWConf() {
  return(settings.ptt.radio_cw_ptt);
}

quint8 Settings::getPTTOutputPhoneConf() {
  return(settings.ptt.radio_phone_ptt);
}

quint8 Settings::getPTTOutputDigitalConf() {
  return(settings.ptt.radio_digital_ptt);
}

bool Settings::getPTTOutputAmplifierEnabled() {
  if (settings.ptt.amplifier_enabled)
    return(true);

  return(false);
}

bool Settings::getPTTOutputInhibitEnabled() {
  if (settings.ptt.inhibit_enabled)
    return(true);

  return(false);
}

bool Settings::getPTTOutputInhibitInverted() {
  if (settings.ptt.inhibit_inverted)
    return(true);

  return(false);
}

/*********** END PTT SETTINGS ***********/

/*********** START AUDIO SETTINGS ***********/

void Settings::setAudioRoutingLineInSourceCW(quint8 index, quint8 state) {
  settings.audio.af_input_source_cw[index] = state;

  UPDATE_TO_DEVICE(settings.audio.af_input_source_cw);
}

void Settings::setAudioRoutingLineInSourcePhone(quint8 index, quint8 state) {
  settings.audio.af_input_source_phone[index] = state;

  UPDATE_TO_DEVICE(settings.audio.af_input_source_phone);
}

void Settings::setAudioRoutingLineInSourceDigital(quint8 index, quint8 state) {
  settings.audio.af_input_source_digital[index] = state;

  UPDATE_TO_DEVICE(settings.audio.af_input_source_digital);
}

void Settings::setAudioRoutingMicPath(quint8 index, quint8 state) {
  settings.audio.mic_path[index] = state;

  UPDATE_TO_DEVICE(settings.audio.mic_path);
}

void Settings::setAudioRoutingMicMute(quint8 index, bool state) {
  settings.audio.mic_mute[index] = state;

  UPDATE_TO_DEVICE(settings.audio.mic_mute);
}

void Settings::setAudioRoutingMicPreamp(quint8 index, bool state) {
  settings.audio.mic_preamp[index] = state;

  UPDATE_TO_DEVICE(settings.audio.mic_preamp);
}

void Settings::setAudioRoutingMicExtPreamp(quint8 index, bool state) {
  settings.audio.mic_ext_preamp[index] = state;

  UPDATE_TO_DEVICE(settings.audio.mic_ext_preamp);
}

quint8 Settings::getAudioRoutingLineInSourceCW(quint8 index) {
  return(settings.audio.af_input_source_cw[index]);
}

quint8 Settings::getAudioRoutingLineInSourcePhone(quint8 index) {
  return(settings.audio.af_input_source_phone[index]);
}

quint8 Settings::getAudioRoutingLineInSourceDigital(quint8 index) {
  return(settings.audio.af_input_source_digital[index]);
}

quint8 Settings::getAudioRoutingMicPath(quint8 index) {
  return(settings.audio.mic_path[index]);
}

bool Settings::getAudioRoutingMicMute(quint8 index) {
  return(settings.audio.mic_mute[index]);
}

bool Settings::getAudioRoutingMicPreamp(quint8 index) {
  return(settings.audio.mic_preamp[index]);
}

bool Settings::getAudioRoutingMicExtPreamp(quint8 index) {
  return(settings.audio.mic_ext_preamp[index]);
}

/*********** END AUDIO SETTINGS ***********/

/*********** START CW SETTINGS ***********/

void Settings::setCWInputSource(quint8 source) {
  settings.cw.input_source = source;

  UPDATE_TO_DEVICE(settings.cw.input_source);
}


quint8 Settings::getCWInputSource() {
  return(settings.cw.input_source);
}

/*********** END CW SETTINGS ***********/

/*********** START MISC SETTINGS ***********/
void Settings::setBacklightRGBColor(quint8 red, quint8 green, quint8 blue) {
  settings.misc.display_rgb_red = red;
  settings.misc.display_rgb_green = green;
  settings.misc.display_rgb_blue = blue;

  UPDATE_TO_DEVICE(settings.misc);
}

void Settings::setBacklightRGBColorRed(quint8 value) {
  settings.misc.display_rgb_red = value;
  UPDATE_TO_DEVICE(settings.misc.display_rgb_red);
}

void Settings::setBacklightRGBColorGreen(quint8 value) {
  settings.misc.display_rgb_green = value;
  UPDATE_TO_DEVICE(settings.misc.display_rgb_green);
}

void Settings::setBacklightRGBColorBlue(quint8 value) {
  settings.misc.display_rgb_blue = value;
  UPDATE_TO_DEVICE(settings.misc.display_rgb_blue);
}

void Settings::setCallsign(QString call) {
  QByteArray array = call.toLocal8Bit();
  quint8 len = call.length();

  if (len >= 10)
    len = 10;

  for (quint8 i=0;i<len;i++)
    settings.misc.callsign[i] = array.at(i);

  //Add null termination character
  settings.misc.callsign[len] = 0;

  UPDATE_TO_DEVICE(settings.misc.callsign);
}

quint8 Settings::getBacklightRGBColorRed() {
  return(settings.misc.display_rgb_red);
}

quint8 Settings::getBacklightRGBColorGreen() {
  return(settings.misc.display_rgb_green);
}

quint8 Settings::getBacklightRGBColorBlue() {
  return(settings.misc.display_rgb_blue);
}

QString Settings::getCallsign() {
  QString str;

  for (quint8 i=0;i<11;i++)
    if (settings.misc.callsign[i] != 0)
      str.append(settings.misc.callsign[i]);
    else
      break;

  return(str);
}

/*********** STOP MISC SETTINGS ***********/

/*********** START WINKEY SETTINGS ***********/

void Settings::setWinkeyKeyerMode(quint8 value) {
  //Clear the data
  settings.winkey.modereg &= ~(1<<5);
  settings.winkey.modereg &= ~(1<<4);

  settings.winkey.modereg |= (value<<4);

  UPDATE_TO_DEVICE(settings.winkey.modereg);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_WINKEY2_MODE);
}

void Settings::setWinkeySwapPaddles(bool state) {
  if (state)
    settings.winkey.modereg |= (1<<3);
  else
    settings.winkey.modereg &= ~(1<<3);

  UPDATE_TO_DEVICE(settings.winkey.modereg);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_WINKEY2_MODE);
}

void Settings::setWinkeyAutoSpace(bool state) {
  if (state)
    settings.winkey.modereg |= (1<<1);
  else
    settings.winkey.modereg &= ~(1<<1);

  UPDATE_TO_DEVICE(settings.winkey.modereg);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_WINKEY2_MODE);
}

void Settings::setWinkeyCTSpace(bool state) {
  if (state)
    settings.winkey.modereg |= (1<<0);
  else
    settings.winkey.modereg &= ~(1<<0);

  UPDATE_TO_DEVICE(settings.winkey.modereg);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_WINKEY2_MODE);
}

void Settings::setWinkeySpeedpotMin(quint8 val) {
  settings.winkey.minwpm = val;

  UPDATE_TO_DEVICE(settings.winkey.minwpm);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_SPEED_POT_SETUP);
}

void Settings::setWinkeySpeedpotMax(quint8 val) {
  settings.winkey.wpmrange = val - settings.winkey.minwpm;

  UPDATE_TO_DEVICE(settings.winkey.wpmrange);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_SPEED_POT_SETUP);
}

void Settings::setWinkeyFarnsworth(quint8 val) {
  settings.winkey.farnswpm = val;

  UPDATE_TO_DEVICE(settings.winkey.farnswpm);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_FARNSWORTH);
}

void Settings::setWinkeyWeight(quint8 val) {
  settings.winkey.weight = val;

  UPDATE_TO_DEVICE(settings.winkey.weight);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_WEIGHT);
}

void Settings::setWinkeyDihDahRatio(quint8 val) {
  settings.winkey.ratio = val;

  UPDATE_TO_DEVICE(settings.winkey.ratio);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_DIH_DAH_RATIO);
}

void Settings::setWinkeyFirstExtension(quint8 val) {
  settings.winkey.xtnd = val;

  UPDATE_TO_DEVICE(settings.winkey.xtnd);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_FIRST_EXTENSION);
}

void Settings::setWinkeyKeyCompensation(quint8 val) {
  settings.winkey.kcomp = val;

  UPDATE_TO_DEVICE(settings.winkey.kcomp);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_KEY_COMPENSATION);
}

void Settings::setWinkeyLeadTime(quint8 val) {
  settings.winkey.lead_time = val;

  UPDATE_TO_DEVICE(settings.winkey.lead_time);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_PTT_LEAD_TAIL);
}

void Settings::setWinkeyTailTime(quint8 val) {
  settings.winkey.tail_time = val;

  UPDATE_TO_DEVICE(settings.winkey.tail_time);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_PTT_LEAD_TAIL);
}

void Settings::setWinkeyPaddleMemory(bool state, bool ditState) {
  settings.winkey.pincfg &= ~(1<<7);
  settings.winkey.pincfg &= ~(1<<6);

  if (state) {
    if (ditState)
      settings.winkey.pincfg |= (1<<7);
    else
      settings.winkey.pincfg |= (1<<6);
  }

  UPDATE_TO_DEVICE(settings.winkey.pincfg);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_PIN_CONF);
}

void Settings::setWinkeyPriority(bool ditState) {
  settings.winkey.pincfg &= ~(1<<7);
  settings.winkey.pincfg &= ~(1<<6);

  if (ditState)
    settings.winkey.pincfg |= (1<<7);
  else
    settings.winkey.pincfg |= (1<<6);

  UPDATE_TO_DEVICE(settings.winkey.pincfg);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_PIN_CONF);
}

void Settings::setWinkeyPaddleMemorySwPoint(quint8 val) {
  settings.winkey.sampadj = val;

  UPDATE_TO_DEVICE(settings.winkey.sampadj);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_PADDE_SWITCHPOINT);
}

void Settings::setWinkeyHangtime(quint8 val) {
  settings.winkey.pincfg &= ~(1<<5);
  settings.winkey.pincfg &= ~(1<<4);

  settings.winkey.pincfg |= (val << 4);

  UPDATE_TO_DEVICE(settings.winkey.pincfg);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_PIN_CONF);
}

void Settings::setWinkeyPaddleEchoBack(bool state) {
  if (state)
    settings.winkey.modereg |= (1<<6);
  else
    settings.winkey.modereg &= ~(1<<6);

  UPDATE_TO_DEVICE(settings.winkey.modereg);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_WINKEY2_MODE);
}

void Settings::setWinkeySerialEchoBack(bool state) {
  if (state)
    settings.winkey.modereg |= (1<<2);
  else
    settings.winkey.modereg &= ~(1<<2);

  UPDATE_TO_DEVICE(settings.winkey.modereg);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_WINKEY2_MODE);
}

void Settings::setWinkeyDefaultSpeed(quint8 value) {
  settings.winkey.def_speed = value;

  UPDATE_TO_DEVICE(settings.winkey.def_speed);
  UPDATE_WINKEY_SETTINGS(WINKEY_CMD_SPEED);
}

quint8 Settings::getWinkeyKeyerMode() {
  return((settings.winkey.modereg & 0x30) >> 4);
}

bool Settings::getWinkeySwapPaddles() {
  if (settings.winkey.modereg & (1<<3))
    return(true);

  return(false);
}

bool Settings::getWinkeyAutoSpace() {
  if (settings.winkey.modereg & (1<<1))
    return(true);

  return(false);
}

bool Settings::getWinkeyCTSpace() {
  if (settings.winkey.modereg & (1<<0))
    return(true);

  return(false);
}

quint8 Settings::getWinkeySpeedpotMin() {
  return(settings.winkey.minwpm);
}

quint8 Settings::getWinkeySpeedpotMax() {
  return(settings.winkey.wpmrange + settings.winkey.minwpm);
}

quint8 Settings::getWinkeyFarnsworth() {
  return(settings.winkey.farnswpm);
}

quint8 Settings::getWinkeyWeight() {
  return(settings.winkey.weight);
}

quint8 Settings::getWinkeyDihDahRatio() {
  return(settings.winkey.ratio);
}

quint8 Settings::getWinkeyFirstExtension() {
  return(settings.winkey.xtnd);
}

quint8 Settings::getWinkeyKeyCompensation() {
  return(settings.winkey.kcomp);
}

quint8 Settings::getWinkeyLeadTime() {
  return(settings.winkey.lead_time);
}

quint8 Settings::getWinkeyTailTime() {
  return(settings.winkey.tail_time);
}

bool Settings::getWinkeyPaddleMemory() {
  if (settings.winkey.pincfg & (1<<7))
    return(true);
  if (settings.winkey.pincfg & (1<<6))
    return(true);

  return(false);
}

bool Settings::getWinkeyPriority() {
  if (settings.winkey.pincfg & (1<<7))
    return(true);

  return(false);
}

quint8 Settings::getWinkeyPaddleMemorySwPoint() {
  return(settings.winkey.sampadj);
}

quint8 Settings::getWinkeyHangtime() {
  return((settings.winkey.pincfg & 0x30) >> 4);
}

bool Settings::getWinkeySerialEchoBack() {
  if ((settings.winkey.modereg & (1<<2)) != 0)
    return(true);

  return(false);
}

bool Settings::getWinkeyPaddleEchoBack() {
  if ((settings.winkey.modereg & (1<<6)) != 0)
    return(true);

  return(false);
}

quint8 Settings::getWinkeyDefaultSpeed() {
  return(settings.winkey.def_speed);
}

/*********** STOP WINKEY SETTINGS ***********/


/*********** START DISPLAY SETTINGS ***********/
void Settings::setDisplay(quint8 lineID, quint8 mode, quint8 id, QString str) {
  QByteArray array = str.toLocal8Bit();

  if (mode == DISPLAY_MODE_CW) {
    settings.display_cw.id[lineID] = id;

    for (unsigned char i=0;i<array.length();i++)
      settings.display_cw.text[lineID][i] = array.at(i);

    settings.display_cw.text[lineID][array.length()] = 0;

    UPDATE_TO_DEVICE(settings.display_cw);
  }
  else if (mode == DISPLAY_MODE_PHONE) {
    settings.display_phone.id[lineID] = id;

    for (unsigned char i=0;i<array.length();i++)
      settings.display_phone.text[lineID][i] = array.at(i);

    settings.display_phone.text[lineID][array.length()] = 0;

    UPDATE_TO_DEVICE(settings.display_phone);
  }
  else if (mode == DISPLAY_MODE_DIGITAL) {
    settings.display_digital.id[lineID] = id;

    for (unsigned char i=0;i<array.length();i++)
      settings.display_digital.text[lineID][i] = array.at(i);

    settings.display_digital.text[lineID][array.length()] = 0;

    UPDATE_TO_DEVICE(settings.display_digital);
  }
}

quint8 Settings::getDisplay(quint8 lineID, quint8 mode) {
  if (mode == DISPLAY_MODE_CW) {
    return(settings.display_cw.id[lineID]);
  }
  else if (mode == DISPLAY_MODE_PHONE) {
    return(settings.display_phone.id[lineID]);
  }
  else if (mode == DISPLAY_MODE_DIGITAL) {
    return(settings.display_digital.id[lineID]);
  }

  return(0);
}
/*********** STOP DISPLAY SETTINGS ***********/


/*********** START CW MESSAGES SETTINGS ***********/

QString Settings::getCWMessage(quint8 index) {
  return(cwMessages[index]);
}

void Settings::setCWMessage(quint8 index, QString str) {
  cwMessages[index] = str;

  UPDATE_CW_MESSAGE(index,str);
}

void Settings::setCWMessageNoUpdate(quint8 index, QString str) {
  cwMessages[index] = str;
}

/*********** STOP CW MESSAGES SETTINGS ***********/
