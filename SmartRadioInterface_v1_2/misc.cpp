#include "misc.h"
#include "commands.h"
#include "../sri_v1_2/SRI_v1/include/status.h"

Misc::Misc() {
}


QString Misc::transformFreqToString(quint32 freq) {
  QString reply;

  reply.append(QString::number((freq/10)));
  reply.insert(2,".");
  reply.insert(6,".");

  return(reply);
}

QString Misc::transformModeToString(quint8 mode) {
  if (mode == STATUS_RADIO_MODE_CW)
    return("CW");
  else if (mode == STATUS_RADIO_MODE_CWR)
    return("CW-R");
  else if (mode == STATUS_RADIO_MODE_LSB)
    return("LSB");
  else if (mode == STATUS_RADIO_MODE_USB)
    return("USB");
  else if (mode == STATUS_RADIO_MODE_AM)
    return("AM");
  else if (mode == STATUS_RADIO_MODE_FM)
    return("FM");
  else if (mode == STATUS_RADIO_MODE_FMN)
    return("FM-N");
  else if (mode == STATUS_RADIO_MODE_FSK)
    return("FSK");
  else if (mode == STATUS_RADIO_MODE_PKTL)
    return("PKT-L");
  else if (mode == STATUS_RADIO_MODE_FSKR)
    return("FSK-R");
  else if (mode == STATUS_RADIO_MODE_PKTFM)
    return("PKT-FM");
  else if (mode == STATUS_RADIO_MODE_PKTU)
    return("PKT-U");

  return(" ");
}

quint8 Misc::transformBaudrateToIndex(quint32 baudrate) {
  quint8 index;

  if (baudrate == 110)
    index = 0;
  else if(baudrate == 300)
    index = 1;
  else if(baudrate == 600)
    index = 2;
  else if(baudrate == 1200)
    index = 3;
  else if(baudrate == 2400)
    index = 4;
  else if(baudrate == 4800)
    index = 5;
  else if(baudrate == 9600)
    index = 6;
  else if(baudrate == 14400)
    index = 7;
  else if(baudrate == 19200)
    index = 8;
  else if(baudrate == 28800)
    index = 9;
  else if(baudrate == 38400)
    index = 10;
  else if(baudrate == 57600)
    index = 11;
  else if(baudrate == 115200)
    index = 12;
  else
    index = 0;

  return(index);
}

quint32 Misc::transformIndexToBaudrate(quint8 index) {
  quint32 baudrate;

  switch (index) {
    case 0: baudrate = 110;
      break;
    case 1: baudrate = 300;
      break;
    case 2: baudrate = 600;
      break;
    case 3: baudrate = 1200;
      break;
    case 4: baudrate = 2400;
      break;
    case 5: baudrate = 4800;
      break;
    case 6: baudrate = 9600;
      break;
    case 7: baudrate = 14400;
      break;
    case 8: baudrate = 19200;
      break;
    case 9: baudrate = 28800;
      break;
    case 10: baudrate = 38400;
      break;
    case 11: baudrate = 57600;
      break;
    case 12: baudrate = 115200;
    break;
    default: baudrate = 0;
      break;
  }

  return(baudrate);
}

bool Misc::transformPTTInputEnable(quint8 data, quint8 type) {
  if (data & (1<<type))
    return(true);

  return(false);
}

bool Misc::transformCWInputSource(quint8 data, quint8 type) {
  if (data & (1<<type))
    return(true);

  return(false);
}
