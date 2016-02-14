#ifndef SETTINGS_H
#define SETTINGS_H

#include "commands.h"
#include "commclass.h"

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

class Settings {
public:
  Settings();
  void setSerialPtr(CommClass *ptr);
  void newDataToStruct(quint8 *data, quint16 offset, quint8 length);
  void newDataToDevice(quint8 *data, quint16 offset, quint8 length);
  void saveToEEPROM();

  // ==== Radio settings SET ====
  void setRadioModel(quint8 value);
  void setRadioCATInterfaceEnabled(bool state);
  void setRadioCATBaudrate(quint32 value);
  void setRadioCATStopbits(quint8 value);
  void setRadioCATParity(quint8 value);
  void setRadioCATICOMCIV(quint8 value);
  void setRadioCATFlowControl(bool state);
  void setRadioCATJumperRTSCTS(bool state);
  void setRadioCATRetrieveType(qint8 value);
  void setRadioCATPollRate(qint16 value);

  // ==== Radio settings GET ====
  quint8 getRadioModel();
  bool getRadioCATInterfaceEnabled();
  quint32 getRadioCATBaudrate();
  quint8 getRadioCATStopbits();
  quint8 getRadioCATParity();
  quint8 getRadioCATICOMCIV();
  bool getRadioCATFlowControl();
  bool getRadioCATJumperRTSCTS();
  quint8 getRadioCATRetrieveType();
  quint16 getRadioCATPollRate();


  // ==== PTT handler SET ====
  void setPTTInputEnabled(quint8 input);
  void setPTTInputInvert(quint8 input);
  void setPTTRadioPreDelay(quint16 delay);
  void setPTTRadioPostDelay(quint16 delay);
  void setPTTAmpPreDelay(quint16 delay);
  void setPTTAmpPostDelay(quint16 delay);
  void setPTTInhibitPreDelay(quint16 delay);
  void setPTTInhibitPostDelay(quint16 delay);
  void setPTTAntennaPreDelay(quint16 delay);
  void setPTTAntennaPostDelay(quint16 delay);
  void setPTTOutputCWConf(quint8 value);
  void setPTTOutputPhoneConf(quint8 value);
  void setPTTOutputDigitalConf(quint8 value);
  void setPTTOutputAmplifierEnabled(bool value);
  void setPTTOutputInhibitEnabled(bool value);
  void setPTTOutputInhibitInverted(bool value);

  // ==== PTT handler GET ====
  quint8 getPTTInputEnabled();
  quint8 getPTTInputInvert();
  bool getPTTInputEnabled(quint8 bitNr);
  bool getPTTInputInvert(quint8 bitNr);
  quint16 getPTTRadioPreDelay();
  quint16 getPTTRadioPostDelay();
  quint16 getPTTAmpPreDelay();
  quint16 getPTTAmpPostDelay();
  quint16 getPTTInhibitPreDelay();
  quint16 getPTTInhibitPostDelay();
  quint16 getPTTAntennaPreDelay();
  quint16 getPTTAntennaPostDelay();
  quint8 getPTTOutputCWConf();
  quint8 getPTTOutputPhoneConf();
  quint8 getPTTOutputDigitalConf();
  bool getPTTOutputAmplifierEnabled();
  bool getPTTOutputInhibitEnabled();
  bool getPTTOutputInhibitInverted();

  // ==== AUDIO ROUTING SET ====
  void setAudioRoutingLineInSourceCW(quint8 index, quint8 state);
  void setAudioRoutingLineInSourcePhone(quint8 index, quint8 state);
  void setAudioRoutingLineInSourceDigital(quint8 index, quint8 state);
  void setAudioRoutingMicPath(quint8 index, quint8 state);
  void setAudioRoutingMicMute(quint8 index, bool state);
  void setAudioRoutingMicPreamp(quint8 index, bool state);
  void setAudioRoutingMicExtPreamp(quint8 index, bool state);

  quint8 getAudioRoutingLineInSourceCW(quint8 index);
  quint8 getAudioRoutingLineInSourcePhone(quint8 index);
  quint8 getAudioRoutingLineInSourceDigital(quint8 index);
  quint8 getAudioRoutingMicPath(quint8 index);
  bool getAudioRoutingMicMute(quint8 index);
  bool getAudioRoutingMicPreamp(quint8 index);
  bool getAudioRoutingMicExtPreamp(quint8 index);

  // ==== CW SET ====
  void setCWInputSource(quint8 source);

  // ==== CW GET ====
  quint8 getCWInputSource(void);

  // ==== Misc SET ====
  void setBacklightRGBColor(quint8 red, quint8 green, quint8 blue);
  void setBacklightRGBColorRed(quint8 value);
  void setBacklightRGBColorGreen(quint8 value);
  void setBacklightRGBColorBlue(quint8 value);
  void setCallsign(QString call);

  // ==== Misc GET ====
  quint8 getBacklightRGBColorRed();
  quint8 getBacklightRGBColorGreen();
  quint8 getBacklightRGBColorBlue();
  QString getCallsign();

  // ==== Digital SET ====
  void setDigitalFSKBaudrate(quint8 index);
  void setDigitalFSKStopbits(quint8 index);
  void setDigitalFSKBitLength(quint8 index);
  void setDigitalFSKParity(quint8 index);

  // ==== Digital GET ====
  quint8 getDigitalFSKBaudrate();
  quint8 getDigitalFSKStopbits();
  quint8 getDigitalFSKBitLength();
  quint8 getDigitalFSKParity();


  // ==== Winkey SET ====
  void setWinkeyKeyerMode(quint8 value);
  void setWinkeySwapPaddles(bool state);
  void setWinkeyAutoSpace(bool state);
  void setWinkeyCTSpace(bool state);
  void setWinkeySpeedpotMin(quint8 val);
  void setWinkeySpeedpotMax(quint8 val);
  void setWinkeyFarnsworth(quint8 val);
  void setWinkeyWeight(quint8 val);
  void setWinkeyDihDahRatio(quint8 val);
  void setWinkeyFirstExtension(quint8 val);
  void setWinkeyKeyCompensation(quint8 val);

  void setWinkeyLeadTime(quint8 val);
  void setWinkeyTailTime(quint8 val);
  void setWinkeyPaddleMemory(bool state, bool ditState);
  void setWinkeyPriority(bool ditState);
  void setWinkeyPaddleMemorySwPoint(quint8 val);
  void setWinkeyHangtime(quint8 val);
  void setWinkeySerialEchoBack(bool state);
  void setWinkeyPaddleEchoBack(bool state);
  void setWinkeyDefaultSpeed(quint8 value);

  // ==== Winkey GET ====
  quint8 getWinkeyKeyerMode();
  bool getWinkeySwapPaddles();
  bool getWinkeyAutoSpace();
  bool getWinkeyCTSpace();
  quint8 getWinkeySpeedpotMin();
  quint8 getWinkeySpeedpotMax();
  quint8 getWinkeyFarnsworth();
  quint8 getWinkeyWeight();
  quint8 getWinkeyDihDahRatio();
  quint8 getWinkeyFirstExtension();
  quint8 getWinkeyKeyCompensation();

  quint8 getWinkeyLeadTime();
  quint8 getWinkeyTailTime();
  bool getWinkeyPaddleMemory();
  bool getWinkeyPriority();
  quint8 getWinkeyPaddleMemorySwPoint();
  quint8 getWinkeyHangtime();
  bool getWinkeySerialEchoBack();
  bool getWinkeyPaddleEchoBack();
  quint8 getWinkeyDefaultSpeed();

  void setDisplay(quint8 lineID, quint8 mode, quint8 id, QString str);
  quint8 getDisplay(quint8 lineID, quint8 mode);

  QString getCWMessage(quint8 index);
  void setCWMessage(quint8 index, QString str);
  void setCWMessageNoUpdate(quint8 index, QString str);

private:
  CommClass *serialPort;
  struct_settings settings;
  QString cwMessages[8];
  quint16 digitalFSKBaudrate[11];
};

#endif // SETTINGS_H
