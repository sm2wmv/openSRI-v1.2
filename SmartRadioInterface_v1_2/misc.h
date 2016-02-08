#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <QDebug>

class Misc {
public:
  Misc();
  static quint8 transformBaudrateToIndex(quint32 baudrate);
    static quint32 transformIndexToBaudrate(quint8 index);
  static bool transformPTTInputEnable(quint8 data, quint8 type);
  static QString transformFreqToString(quint32 freq);
  static QString transformModeToString(quint8 mode);
  static bool transformCWInputSource(quint8 data, quint8 type);
};

#endif // MISC_H
