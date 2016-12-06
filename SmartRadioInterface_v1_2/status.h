#ifndef STATUS_H
#define STATUS_H

#include <QString>
#include <QSerialPort>
#include <QDebug>
#include <stdint.h>

#include "commclass.h"
#include "commands.h"
#include "C:/Users/radio/Documents/openSRI-v1.2/sri_v1_2/SRI_v1/include/status.h"

class Status {
public:
  Status();
  void setSerialPtr(CommClass *ptr);
  void newDataToStruct(quint8 *data, quint16 offset, quint8 length);
  void newDataToDevice(quint8 *data, quint16 offset, quint8 length);

  // ==== Radio status get ====
  quint32 getVFOAFreq(void);
  quint32 getVFOBFreq(void);
  quint8 getVFOAMode(void);
  quint8 getVFOAModeType(void);
  quint8 getVFOBMode(void);
  quint8 getVFOBModeType(void);
  quint8 getVFOABTXRXState(void);

  // ==== PTT status get ====
  bool getPTTInputState(quint8 bitNr);
  bool getPTTOutputState(quint8 bitNr);

  // ==== Winkey status get ====
  quint8 getWinkeyPotSpeed();
private:
  CommClass *serialPort;
  struct_status status;
};

#endif // STATUS_H
