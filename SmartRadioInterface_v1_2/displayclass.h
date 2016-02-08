#ifndef DISPLAY_H
#define DISPLAY_H

#include <QString>
#include <QStringList>
#include "commands.h"

class displayItem {
public:
  displayItem();
  void add(QString str, QString displayStr, quint8 index);
  QString getText();
  QString getDisplayText();
  quint8 getID();
private:
  QString text;
  QString displayText;
  quint8 id;
};

class DisplayClass {
public:
  DisplayClass();
  QStringList getStringListCW();
  QStringList getStringListPhone();
  QStringList getStringListDigital();
  quint8 getIDCW(quint8 rowID);
  quint8 getIDPhone(quint8 rowID);
  quint8 getIDDigital(quint8 rowID);
  QString getStringCW(quint8 rowID);
  QString getStringPhone(quint8 rowID);
  QString getStringDigital(quint8 rowID);

private:
  displayItem displayItemArrayCW[DISPLAY_TYPE_ID_COUNT_CW];
  displayItem displayItemArrayPhone[DISPLAY_TYPE_ID_COUNT_PHONE];
  displayItem displayItemArrayDigital[DISPLAY_TYPE_ID_COUNT_DIGITAL];
};

#endif // DISPLAY_H
