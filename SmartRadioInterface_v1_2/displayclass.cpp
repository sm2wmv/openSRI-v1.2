#include "displayclass.h"

displayItem::displayItem() {
}

void displayItem::add(QString str, QString displayStr, quint8 index) {
  text = str;
  displayText = displayStr;
  id = index;
}

QString displayItem::getText() {
  return(text);
}

QString displayItem::getDisplayText() {
  return(displayText);
}

quint8 displayItem::getID() {
  return(id);
}


DisplayClass::DisplayClass() {
  //Create the CW list
  quint8 index=0;
  displayItemArrayCW[index++].add("Empty","\0",DISPLAY_TYPE_ID_EMPTY);
  displayItemArrayCW[index++].add("Callsign","Call: %s\0",DISPLAY_TYPE_ID_CALL);
  displayItemArrayCW[index++].add("Frequency","Freq: %s\0",DISPLAY_TYPE_ID_FREQ);
  displayItemArrayCW[index++].add("Current mode","Mode: %s\0",DISPLAY_TYPE_ID_MODE);
  displayItemArrayCW[index++].add("Winkey CW Speed","CW Speed: %i\0",DISPLAY_TYPE_ID_CW_SPEED);

  index = 0;
  displayItemArrayPhone[index++].add("Empty","\0",DISPLAY_TYPE_ID_EMPTY);
  displayItemArrayPhone[index++].add("Callsign","Call: %s\0",DISPLAY_TYPE_ID_CALL);
  displayItemArrayPhone[index++].add("Frequency","Freq: %s\0",DISPLAY_TYPE_ID_FREQ);
  displayItemArrayPhone[index++].add("Current mode","Mode: %s\0",DISPLAY_TYPE_ID_MODE);

  index = 0;
  displayItemArrayDigital[index++].add("Empty","\0",DISPLAY_TYPE_ID_EMPTY);
  displayItemArrayDigital[index++].add("Callsign","Call: %s\0",DISPLAY_TYPE_ID_CALL);
  displayItemArrayDigital[index++].add("Frequency","Freq: %s\0",DISPLAY_TYPE_ID_FREQ);
  displayItemArrayDigital[index++].add("Current mode","Mode: %s\0",DISPLAY_TYPE_ID_MODE);
}

QStringList DisplayClass::getStringListCW() {
  QStringList strList;

  for (quint8 i=0;i<DISPLAY_TYPE_ID_COUNT_CW;i++)
    strList.append(displayItemArrayCW[i].getText());

  return(strList);
}

QStringList DisplayClass::getStringListPhone() {
  QStringList strList;

  for (quint8 i=0;i<DISPLAY_TYPE_ID_COUNT_PHONE;i++)
    strList.append(displayItemArrayPhone[i].getText());

  return(strList);
}

QStringList DisplayClass::getStringListDigital() {
  QStringList strList;

  for (quint8 i=0;i<DISPLAY_TYPE_ID_COUNT_DIGITAL;i++)
    strList.append(displayItemArrayDigital[i].getText());

  return(strList);
}

quint8 DisplayClass::getIDCW(quint8 rowID) {
  if (rowID < DISPLAY_TYPE_ID_COUNT_CW)
    return(displayItemArrayCW[rowID].getID());

  return(0);
}

quint8 DisplayClass::getIDPhone(quint8 rowID) {
  if (rowID < DISPLAY_TYPE_ID_COUNT_PHONE)
    return(displayItemArrayPhone[rowID].getID());

  return(0);
}

quint8 DisplayClass::getIDDigital(quint8 rowID) {
  if (rowID < DISPLAY_TYPE_ID_COUNT_DIGITAL)
    return(displayItemArrayDigital[rowID].getID());

  return(0);
}

QString DisplayClass::getStringCW(quint8 rowID) {
  if (rowID < DISPLAY_TYPE_ID_COUNT_CW)
    return(displayItemArrayCW[rowID].getDisplayText());

  return(0);
}

QString DisplayClass::getStringPhone(quint8 rowID) {
  if (rowID < DISPLAY_TYPE_ID_COUNT_PHONE)
    return(displayItemArrayPhone[rowID].getDisplayText());

  return(0);
}

QString DisplayClass::getStringDigital(quint8 rowID) {
  if (rowID < DISPLAY_TYPE_ID_COUNT_DIGITAL)
    return(displayItemArrayDigital[rowID].getDisplayText());

  return(0);
}
