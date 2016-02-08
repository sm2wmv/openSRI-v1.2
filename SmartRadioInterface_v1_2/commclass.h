/***************************************************************************
 *   Copyright (C) 2009 by Mikael Larsmark, SM2WMV   *
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

#ifndef __COMMCLASS_H__
#define __COMMCLASS_H__

#include <QMainWindow>
#include <QThread>
#include <QLinkedList>
#include <QString>
#include <QSerialPort>
#include <QDebug>

#include <stdint.h>

//Must be set to the same value in the uC source; VERY IMPORTANT!
#define	COMM_INTERFACE_DATA_LENGTH	255

//! The timeout limit for transmission (50 -> 500 ms)
#define COMM_INTERFACE_TX_TIMEOUT_LIMIT 50

#define COMM_INTERFACE_RX_TIMEOUT_LIMIT 50

#define COMM_INTERFACE_MAX_RESENDS  5

struct StructMessage {
  quint8 checksum;
  quint8 cmd;
  quint8 length;
  quint8 data[COMM_INTERFACE_DATA_LENGTH];
};

//! The serial acknowledge of the computer communication protocol
#define COMM_CLASS_ACK				0xFA
//! The serial NOT acknowledge of the computer communication protocol
#define COMM_CLASS_NACK			0xFB
//! The communication preamble
#define COMM_CLASS_PREAMBLE   0xFE
//! The communication postamble
#define COMM_CLASS_POSTAMBLE  0xFD

class CommClass {
  public:
    CommClass();
    qint32 openPort(QString deviceName, quint32 baudrate);
    qint32 closePort();
    void receiveMsg();
    void sendMessage(StructMessage message);
    void addTXMessage(quint8 cmd, quint8 length, quint8 *data);
    void addTXMessage(quint8 cmd, qint8 data);
    void addTXMessage(quint8 cmd, quint8 length, QString data);
    void addTXMessage(quint8 cmd, quint8 length, QByteArray *data);
    void addTXMessage(quint8 cmd);
    void addTXMessage(quint8 cmd, qint8 data1, qint8 data2);
    void addTXMessageSendStruct(quint8 cmd, quint8 length, quint16 offset, quint8 *data);
    bool isOpen();
    qint32 getRXQueueSize();
    StructMessage getRXQueueFirst();
    void processData();
  private:
    void pollTXQueue();
    void resetRXStatus();
    void checkTXQueue();
    void sendACK();
    void sendNACK();
    quint8 txMsgAcked;
    quint8 resendCount;
    quint16 txTimeout;
    quint16 rxTimeout;
    quint8 resendFlag;
  protected:
    QSerialPort *serialPort;
    QByteArray rxMessage;
    QLinkedList<StructMessage> txQueue;
    QLinkedList<StructMessage> rxQueue;
    void run();
};

#endif
