/***************************************************************************
 *   Copyright (C) 2014 by Mikael Larsmark, SM2WMV   *
 *   mike@sm3wmv.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "commclass.h"

qint32 ackCount = 0;

CommClass::CommClass() {
  serialPort = new QSerialPort();
}

int CommClass::openPort(QString deviceName, quint32 baudrate) {
  serialPort->setPortName(deviceName);
  serialPort->setBaudRate(baudrate);
  serialPort->setDataBits(QSerialPort::Data8);
  serialPort->setFlowControl(QSerialPort::NoFlowControl);
  serialPort->setParity(QSerialPort::NoParity);
  serialPort->setStopBits(QSerialPort::OneStop);

  txTimeout = 0;
  rxTimeout = 0;
  resendCount = 0;
  resendFlag = 0;
  txMsgAcked = 1;

  if (serialPort->open(QIODevice::ReadWrite)) {
    serialPort->flush();

    return(1);
  }
  else {
    return(0);
  }
}

void CommClass::processData() {
  if (serialPort->isOpen()) {
    receiveMsg();

    checkTXQueue();

    rxTimeout++;

    if (rxMessage.size() > 0) {
      if (rxTimeout > COMM_INTERFACE_RX_TIMEOUT_LIMIT) {
        rxMessage.clear();

        rxTimeout = 0;
      }
    }

    if (txMsgAcked == 0) {
      if ((txTimeout > COMM_INTERFACE_TX_TIMEOUT_LIMIT) && (resendFlag == 0)) {
        resendFlag = 1;
      }
      else
        txTimeout++;
    }
  }
}

int CommClass::closePort() {
  if (serialPort->isOpen())
    serialPort->close();

  return(0);
}

void CommClass::checkTXQueue() {
  if (txQueue.size() > 0) {
    if (txMsgAcked) {
      sendMessage(txQueue.first());

      resendFlag = 0;
      txMsgAcked = 0;
      resendCount = 0;
      txTimeout = 0;
    }
    else if (resendFlag) {
      if(resendCount < COMM_INTERFACE_MAX_RESENDS) {
        if (txTimeout > COMM_INTERFACE_TX_TIMEOUT_LIMIT) {
          //qDebug() << "RESEND";
          if (txQueue.size() > 0)
            sendMessage(txQueue.first());

          resendCount++;
          txTimeout = 0;
        }
      }
      else {
        if (txQueue.size() > 0)
          txQueue.removeFirst();

        txMsgAcked = 1;
        resendCount = 0;
        txTimeout = 0;
      }
    }
  }
}

int CommClass::getRXQueueSize() {
  return(rxQueue.size());
}

StructMessage CommClass::getRXQueueFirst() {
  StructMessage message = rxQueue.first();
  rxQueue.removeFirst();

  return(message);
}

void CommClass::resetRXStatus() {

}

void CommClass::sendACK() {
  //qDebug("SENDING ACK");

  StructMessage message;

  message.checksum = COMM_CLASS_ACK;
  message.cmd = COMM_CLASS_ACK;
  message.length = 0;

  sendMessage(message);
}

void CommClass::sendNACK() {
  StructMessage message;

  message.checksum = COMM_CLASS_NACK;
  message.cmd = COMM_CLASS_NACK;
  message.length = 0;

  sendMessage(message);
}

void CommClass::receiveMsg() {
  char buff[1024];
  quint8 prev_char = 0;
  quint8 curr_char = 0;
  quint8 char_count = 0;
  quint8 checksum = 0;
  qint32 lastFoundIndex = 0;
  quint8 preamble_found = 0;
  int numBytes;

  StructMessage message;

  numBytes = serialPort->bytesAvailable();

  if(numBytes > 0) {
    rxTimeout = 0;

    if(numBytes > 1024)
      numBytes = 1024;

    int i = serialPort->read(buff, numBytes);

    rxMessage.append(buff,i);
  }

  for (int i=0;i<rxMessage.size();i++) {
    curr_char = rxMessage.at(i);

    if (preamble_found) {
      if ((curr_char == COMM_CLASS_POSTAMBLE) && (message.length == (char_count - 3))){
        //qDebug("FOUND POSTAMBLE");

        if (checksum == message.checksum) {
          if (message.cmd == COMM_CLASS_ACK) {
            if (txQueue.size() > 0)
              txQueue.removeFirst();  //Remove the first message in the queue

            txMsgAcked = 1;
            resendFlag = 0;
            resendCount = 0;
          }
          else if (message.cmd == COMM_CLASS_NACK) {
            resendFlag = 1;
          }
          else {
            rxQueue.append(message);
            //qDebug("SEND ACK");
            sendACK();

            rxMessage.clear();
          }

          checksum = 0;
          char_count = 0;
        }
        else {
          resendFlag = 1;
          resendCount = 0;
          lastFoundIndex = i;
          //qDebug("CHECK FAILED");
        }

        lastFoundIndex = i;
      }
      else {
        if (char_count < (COMM_INTERFACE_DATA_LENGTH+3)) {
          switch(char_count) {
            case 0:
              message.checksum = curr_char;
              break;
          case 1:
              message.cmd = curr_char;
              checksum += curr_char;
              break;
          case 2:
              message.length = curr_char;
              checksum += curr_char;
              break;
          default:
              message.data[char_count-3] = curr_char;
              checksum += curr_char;
              break;
          }

          char_count++;
        }
        else {
          char_count = 0;
          checksum = 0;
          curr_char = 0;
        }
      }
    }
    else if ((prev_char == COMM_CLASS_PREAMBLE) && (curr_char == COMM_CLASS_PREAMBLE)) {
      preamble_found = 1;
      checksum = 0;
      char_count = 0;
    }

    prev_char = curr_char;
  }

  if (lastFoundIndex != 0)
    rxMessage.remove(0,lastFoundIndex+1);
}

bool CommClass::isOpen() {
  return(serialPort->isOpen());
}

void CommClass::sendMessage(StructMessage message) {
  QByteArray txArray;

  txArray.append(COMM_CLASS_PREAMBLE);
  txArray.append(COMM_CLASS_PREAMBLE);
  txArray.append(message.checksum);
  txArray.append(message.cmd);
  txArray.append(message.length);

  for (quint8 i=0;i<message.length;i++)
    txArray.append(message.data[i]);

  txArray.append(COMM_CLASS_POSTAMBLE);

  serialPort->write(txArray);
}

void CommClass::addTXMessage(quint8 cmd, quint8 length, quint8 *data) {
  StructMessage message;

  message.checksum = 0;
  message.cmd = cmd;
  message.checksum += cmd;
  message.length = length;
  message.checksum += length;

  for (quint8 i=0;i<length;i++) {
    message.data[i] = data[i];
    message.checksum += data[i];
  }

  txQueue.append(message);
}

void CommClass::addTXMessageSendStruct(quint8 cmd, quint8 length, quint16 offset, quint8 *data) {
  StructMessage message;

  //qDebug() << data[0] << " " << data[1];

  message.checksum = 0;
  message.cmd = cmd;
  message.checksum += cmd;
  message.length = length+2;
  message.checksum += message.length;

  message.data[0] = (offset & 0xFF00) >> 8;
  message.checksum += message.data[0];
  message.data[1] = offset & 0x00FF;
  message.checksum += message.data[1];

  for (quint8 i=0;i<length;i++) {
    message.data[i+2] = data[i];
    message.checksum += data[i];
  }

  txQueue.append(message);
}

void CommClass::addTXMessage(quint8 cmd, qint8 data1, qint8 data2) {
  StructMessage message;

  message.checksum = 0;
  message.cmd = cmd;
  message.checksum += cmd;
  message.length = 2;
  message.checksum += 2;

  message.data[0] = data1;
  message.data[1] = data2;
  message.checksum += data1;
  message.checksum += data2;

  txQueue.append(message);
}

void CommClass::addTXMessage(quint8 cmd, qint8 data) {
  StructMessage message;

  message.checksum = 0;
  message.cmd = cmd;
  message.checksum += cmd;
  message.length = 1;
  message.checksum += 1;

  message.data[0] = data;
  message.checksum += data;

  txQueue.append(message);
}

void CommClass::addTXMessage(quint8 cmd, quint8 length, QByteArray *data) {
  StructMessage message;

  message.checksum = 0;
  message.cmd = cmd;
  message.checksum += cmd;
  message.length = length;
  message.checksum += length;

  for (quint8 i=0;i<length;i++) {
    message.data[i] = data->at(i);
    message.checksum += data->at(i);
  }

  txQueue.append(message);
}

void CommClass::addTXMessage(quint8 cmd, quint8 length, QString data) {
  StructMessage message;

  message.checksum = 0;
  message.cmd = cmd;
  message.checksum += cmd;
  message.length = length;
  message.checksum += length;

  for (quint8 i=0;i<length;i++) {
    message.data[i] = data.at(i).toLatin1();
    message.checksum += data.at(i).toLatin1();
  }

  txQueue.append(message);
}

void CommClass::addTXMessage(quint8 cmd) {
  StructMessage message;

  message.checksum = 0;
  message.cmd = cmd;
  message.checksum += cmd;
  message.length = 0;

  txQueue.append(message);
}
