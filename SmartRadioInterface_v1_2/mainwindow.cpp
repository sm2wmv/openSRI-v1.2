#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QColor>
#include <QColorDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "version.h"
#include "../sri_v1_2/SRI_v1/include/status.h"

void MainWindow::actionSerSRIPortTriggered() {
  dlgSelectPort->refreshPorts();
  dlgSelectPort->show();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->tabWidget->setCurrentIndex(0);

  dlgSelectPort = new DialogSelectPort();
  dlgSelectPort->hide();

  QString str;
  str.append("Smart Radio Interface - ");
  str.append(SOFTWARE_VERSION);

  this->setWindowTitle(str);

  settings = new Settings();
  status = new Status();

  serialPort = new CommClass();

  timerPollStatus = new QTimer(this);
  timerPingPongEvent = new QTimer(this);

  QString freqVFOA = QString("14.195.00");
  ui->lcdNumberFreqVFOA->display(freqVFOA);

  QString freqVFOB = QString("14.025.34");
  ui->lcdNumberFreqVFOB->display(freqVFOB);

  ui->labelVFOATX->setStyleSheet(QLABEL_TX_STYLESHEET);
  ui->labelVFOARX->setStyleSheet(QLABEL_RX_STYLESHEET);

  QColor color;
  color.setRed(ui->spinBoxDisplayBacklightRed->value());
  color.setGreen(ui->spinBoxDisplayBacklightGreen->value());
  color.setBlue(ui->spinBoxDisplayBacklightBlue->value());
  color.setAlpha(255);
  updateBacklightColor(color);

  settings->setSerialPtr(serialPort);
  status->setSerialPtr(serialPort);

  connect(timerPollStatus, SIGNAL(timeout()), this, SLOT(timerPollStatusUpdate()));
  connect(timerPingPongEvent, SIGNAL(timeout()), this, SLOT(timerPingPongEventUpdate()));
  connect(ui->actionSetSRIPort,SIGNAL(triggered()),this,SLOT(actionSerSRIPortTriggered()));

  connect(ui->checkBoxPTTInputFootswitch,SIGNAL(clicked()),this,SLOT(actionPTTInputClicked()));
  connect(ui->checkBoxPTTInputFSKCWRTS,SIGNAL(clicked()),this,SLOT(actionPTTInputClicked()));
  connect(ui->checkBoxPTTInputVOXSense,SIGNAL(clicked()),this,SLOT(actionPTTInputClicked()));
  connect(ui->checkBoxPTTInputWinKey,SIGNAL(clicked()),this,SLOT(actionPTTInputClicked()));
  connect(ui->checkBoxPTTInputWinKeyDTR,SIGNAL(clicked()),this,SLOT(actionPTTInputClicked()));
  connect(ui->checkBoxPTTInputWinKeyRTS,SIGNAL(clicked()),this,SLOT(actionPTTInputClicked()));

  connect(ui->checkBoxPTTInputFSKCWRTSInvert,SIGNAL(clicked()),this,SLOT(actionPTTInputInvertClicked()));
  connect(ui->checkBoxPTTInputWinKeyDTRInvert,SIGNAL(clicked()),this,SLOT(actionPTTInputInvertClicked()));
  connect(ui->checkBoxPTTInputWinKeyRTSInvert,SIGNAL(clicked()),this,SLOT(actionPTTInputInvertClicked()));
  connect(ui->checkBoxPTTInputVOXInvert,SIGNAL(clicked()),this,SLOT(actionPTTInputInvertClicked()));

  connect(ui->checkBoxCWSourceWinkey, SIGNAL(clicked()), this, SLOT(actionCWInputSourceToggled()));
  connect(ui->checkBoxCWSourceWinkeyUSBDTR, SIGNAL(clicked()), this, SLOT(actionCWInputSourceToggled()));
  connect(ui->checkBoxCWSourceWinkeyUSBRTS, SIGNAL(clicked()), this, SLOT(actionCWInputSourceToggled()));
  connect(ui->checkBoxCWSourceUSBDTR, SIGNAL(clicked()), this, SLOT(actionCWInputSourceToggled()));

  connect(ui->pushButtonEnableDebug, SIGNAL(clicked()), this, SLOT(checkBoxDebugClicked()));
  connect(ui->checkBoxDebugWinkey, SIGNAL(clicked()), this, SLOT(checkBoxDebugClicked()));
  connect(ui->checkBoxDebugCAT, SIGNAL(clicked()), this, SLOT(checkBoxDebugClicked()));
  connect(ui->checkBoxDebugSystem, SIGNAL(clicked()), this, SLOT(checkBoxDebugClicked()));
  connect(ui->checkBoxDebugSequencer, SIGNAL(clicked()), this, SLOT(checkBoxDebugClicked()));

  ui->comboBoxDisplayTextCWLine1RX->addItems(display.getStringListCW());
  ui->comboBoxDisplayTextCWLine2RX->addItems(display.getStringListCW());
  ui->comboBoxDisplayTextCWLine1TX->addItems(display.getStringListCW());
  ui->comboBoxDisplayTextCWLine2TX->addItems(display.getStringListCW());

  ui->comboBoxDisplayTextPhoneLine1RX->addItems(display.getStringListPhone());
  ui->comboBoxDisplayTextPhoneLine2RX->addItems(display.getStringListPhone());
  ui->comboBoxDisplayTextPhoneLine1TX->addItems(display.getStringListPhone());
  ui->comboBoxDisplayTextPhoneLine2TX->addItems(display.getStringListPhone());

  ui->comboBoxDisplayTextDigitalLine1RX->addItems(display.getStringListDigital());
  ui->comboBoxDisplayTextDigitalLine2RX->addItems(display.getStringListDigital());
  ui->comboBoxDisplayTextDigitalLine1TX->addItems(display.getStringListDigital());
  ui->comboBoxDisplayTextDigitalLine2TX->addItems(display.getStringListDigital());

  QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
  QRegExp ipRegex ("^" + ipRange
                   + "\\." + ipRange
                   + "\\." + ipRange
                   + "\\." + ipRange + "$");
  QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
  ui->lineEditEthernetIP->setValidator(ipValidator);
  ui->lineEditEthernetSubnet->setValidator(ipValidator);
  ui->lineEditEthernetGateway->setValidator(ipValidator);

  QWidget *listSegmentView = new QWidget;
  ui->scrollAreaBandDecoder->setWidget(listSegmentView);
  QBoxLayout *segmentLayout = new QBoxLayout(QBoxLayout::TopToBottom, listSegmentView);

  segmentLayout->addWidget(new FormBandDecoderTitle());
  for (int i=0;i<listBandDecoderInput.length();i++) {
    segmentLayout->addWidget(listBandDecoderInput.at(i), 0, 0);
    listBandDecoderInput.at(i)->setIndex(i+1);
  }

  segmentLayout->addStretch();
}

void MainWindow::closeEvent (QCloseEvent *event) {
  QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Smart Radio Interface",
                                        tr("Are you sure you wish to quit?\n"),
                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::Yes);
  if (resBtn != QMessageBox::Yes) {
    event->ignore();
  } else {
    on_actionDisconnect_triggered();

    //We ignore the event and close the application one second later
    //that way the communication to the SRI can send the disconnect command
    event->ignore();
    QTimer::singleShot(1000, this, SLOT(killTheProgram()));
  }
}

MainWindow::~MainWindow() {
  if (serialPort->isOpen())
    serialPort->closePort();

  delete ui;
}

CommClass* MainWindow::getSerialPtr() {
  return(serialPort);
}

void MainWindow::updateUIfromStatus() {
  if (settings->getPTTInputEnabled(SETTING_PTT_INPUT_FSKCW_RTS)) {
    if (status->getPTTInputState(STATUS_PTT_INPUT_FSKCW_RTS))
      ui->labelPTTInputIndicatorFSKCWRTS->setStyleSheet(QLABEL_TX_STYLESHEET);
    else
      ui->labelPTTInputIndicatorFSKCWRTS->setStyleSheet(QLABEL_RX_STYLESHEET);
  }
  else
    ui->labelPTTInputIndicatorFSKCWRTS->setStyleSheet(QLABEL_TXRXNONE_STYLESHEET);

  if (settings->getPTTInputEnabled(SETTING_PTT_INPUT_FOOTSWITCH)) {
    if (status->getPTTInputState(STATUS_PTT_INPUT_FOOTSWITCH))
      ui->labelPTTInputIndicatorFootswitch->setStyleSheet(QLABEL_TX_STYLESHEET);
    else
      ui->labelPTTInputIndicatorFootswitch->setStyleSheet(QLABEL_RX_STYLESHEET);
  }
  else
    ui->labelPTTInputIndicatorFootswitch->setStyleSheet(QLABEL_TXRXNONE_STYLESHEET);

  if (settings->getPTTInputEnabled(SETTING_PTT_INPUT_WK)) {
    if (status->getPTTInputState(STATUS_PTT_INPUT_WK))
      ui->labelPTTInputIndicatorWinKey->setStyleSheet(QLABEL_TX_STYLESHEET);
    else
      ui->labelPTTInputIndicatorWinKey->setStyleSheet(QLABEL_RX_STYLESHEET);
  }
  else
    ui->labelPTTInputIndicatorWinKey->setStyleSheet(QLABEL_TXRXNONE_STYLESHEET);

  if (settings->getPTTInputEnabled(SETTING_PTT_INPUT_VOX_SENSE)) {
    if (status->getPTTInputState(STATUS_PTT_INPUT_VOX_SENSE))
      ui->labelPTTInputIndicatorVOXSense->setStyleSheet(QLABEL_TX_STYLESHEET);
    else
      ui->labelPTTInputIndicatorVOXSense->setStyleSheet(QLABEL_RX_STYLESHEET);
  }
  else
    ui->labelPTTInputIndicatorVOXSense->setStyleSheet(QLABEL_TXRXNONE_STYLESHEET);

  if (settings->getPTTInputEnabled(SETTING_PTT_INPUT_WK_DTR)) {
    if (status->getPTTInputState(STATUS_PTT_INPUT_WK_DTR))
      ui->labelPTTInputIndicatorWinKeyDTR->setStyleSheet(QLABEL_TX_STYLESHEET);
    else
      ui->labelPTTInputIndicatorWinKeyDTR->setStyleSheet(QLABEL_RX_STYLESHEET);
  }
  else
    ui->labelPTTInputIndicatorWinKeyDTR->setStyleSheet(QLABEL_TXRXNONE_STYLESHEET);

  if (settings->getPTTInputEnabled(SETTING_PTT_INPUT_WK_RTS)) {
    if (status->getPTTInputState(STATUS_PTT_INPUT_WK_RTS))
      ui->labelPTTInputIndicatorWinKeyRTS->setStyleSheet(QLABEL_TX_STYLESHEET);
    else
      ui->labelPTTInputIndicatorWinKeyRTS->setStyleSheet(QLABEL_RX_STYLESHEET);
  }
  else
    ui->labelPTTInputIndicatorWinKeyRTS->setStyleSheet(QLABEL_TXRXNONE_STYLESHEET);

  ui->lcdNumberFreqVFOA->display(Misc::transformFreqToString(status->getVFOAFreq()));
  ui->lcdNumberFreqVFOB->display(Misc::transformFreqToString(status->getVFOBFreq()));

  ui->labelVFOAMode->setText(Misc::transformModeToString(status->getVFOAMode()));
  ui->labelVFOBMode->setText(Misc::transformModeToString(status->getVFOBMode()));

  if (status->getVFOAModeType() == STATUS_RADIO_MODE_TYPE_CW)
    ui->labelModeTypeData->setText("CW");
  else if (status->getVFOAModeType() == STATUS_RADIO_MODE_TYPE_PHONE)
    ui->labelModeTypeData->setText("PHONE");
  else if (status->getVFOAModeType() == STATUS_RADIO_MODE_TYPE_DIGITAL)
    ui->labelModeTypeData->setText("DIGITAL");
  else
    ui->labelModeTypeData->setText("UNKNOWN");

  quint8 txrx_state = status->getVFOABTXRXState();

  if (txrx_state & (1<<STATUS_VFO_TXRX_VFOA_TX))
    ui->labelVFOATX->setStyleSheet(QLABEL_TX_STYLESHEET);
  else
    ui->labelVFOATX->setStyleSheet(QLABEL_TXRXNONE_STYLESHEET);

  if (txrx_state & (1<<STATUS_VFO_TXRX_VFOB_TX))
    ui->labelVFOBTX->setStyleSheet(QLABEL_TX_STYLESHEET);
  else
    ui->labelVFOBTX->setStyleSheet(QLABEL_TXRXNONE_STYLESHEET);

  if (txrx_state & (1<<STATUS_VFO_TXRX_VFOA_RX))
    ui->labelVFOARX->setStyleSheet(QLABEL_RX_STYLESHEET);
  else if (txrx_state & (1<<STATUS_VFO_TXRX_VFOA_MUTE))
    ui->labelVFOARX->setStyleSheet(QLABEL_RX_MUTE_STYLESHEET);
  else
    ui->labelVFOARX->setStyleSheet(QLABEL_TXRXNONE_STYLESHEET);

  if (txrx_state & (1<<STATUS_VFO_TXRX_VFOB_RX))
    ui->labelVFOBRX->setStyleSheet(QLABEL_RX_STYLESHEET);
  else if (txrx_state & (1<<STATUS_VFO_TXRX_VFOB_MUTE))
    ui->labelVFOBRX->setStyleSheet(QLABEL_RX_MUTE_STYLESHEET);
  else
    ui->labelVFOBRX->setStyleSheet(QLABEL_TXRXNONE_STYLESHEET);

  QString str;
  str.append("Pot speed: ");
  str.append(QString::number(status->getWinkeyPotSpeed()));
  str.append(" WPM");
  ui->labelWinkeyPotSpeedStatus->setText(str);
}

void MainWindow::updateUIfromSettings() {
  bool state = true;

  //********* START RADIO SETTINGS *********//
  ui->groupBoxCATInterface->setChecked(settings->getRadioCATInterfaceEnabled());

  ui->comboBoxCATBaudrate->setCurrentIndex(Misc::transformBaudrateToIndex( settings->getRadioCATBaudrate()));
  ui->comboBoxCATParity->setCurrentIndex(settings->getRadioCATParity());
  ui->comboBoxCATStopbits->setCurrentIndex(settings->getRadioCATStopbits());
  ui->comboBoxRadioModel->setCurrentIndex(settings->getRadioModel());
  ui->spinBoxCATCIVAddress->setValue(settings->getRadioCATICOMCIV());
  ui->checkBoxCATFlowControl->setChecked(settings->getRadioCATFlowControl());
  ui->checkBoxCATJumperRTSCTS->setChecked(settings->getRadioCATJumperRTSCTS());

  ui->comboBoxCATRetrieveType->setCurrentIndex(settings->getRadioCATRetrieveType());
  ui->spinBoxCATPollingRate->setValue(settings->getRadioCATPollRate());
  //********* END RADIO SETTINGS *********//

  //********* START PTT SETTINGS *********//
  state = true;
  ui->checkBoxPTTInputFootswitch->blockSignals(state);
  ui->checkBoxPTTInputWinKeyRTS->blockSignals(state);
  ui->checkBoxPTTInputWinKeyDTR->blockSignals(state);
  ui->checkBoxPTTInputFSKCWRTS->blockSignals(state);
  ui->checkBoxPTTInputVOXSense->blockSignals(state);
  ui->checkBoxPTTInputWinKeyRTSInvert->blockSignals(state);
  ui->checkBoxPTTInputWinKeyDTRInvert->blockSignals(state);
  ui->checkBoxPTTInputFSKCWRTSInvert->blockSignals(state);
  ui->checkBoxPTTInputVOXInvert->blockSignals(state);
  ui->checkBoxPTTInputWinKey->blockSignals(state);

  ui->checkBoxPTTInputFootswitch->setChecked(Misc::transformPTTInputEnable(settings->getPTTInputEnabled(),SETTING_PTT_INPUT_FOOTSWITCH));
  ui->checkBoxPTTInputWinKeyRTS->setChecked(Misc::transformPTTInputEnable(settings->getPTTInputEnabled(),SETTING_PTT_INPUT_WK_RTS));
  ui->checkBoxPTTInputWinKeyDTR->setChecked(Misc::transformPTTInputEnable(settings->getPTTInputEnabled(),SETTING_PTT_INPUT_WK_DTR));
  ui->checkBoxPTTInputFSKCWRTS->setChecked(Misc::transformPTTInputEnable(settings->getPTTInputEnabled(),SETTING_PTT_INPUT_FSKCW_RTS));
  ui->checkBoxPTTInputVOXSense->setChecked(Misc::transformPTTInputEnable(settings->getPTTInputEnabled(),SETTING_PTT_INPUT_VOX_SENSE));
  ui->checkBoxPTTInputWinKey->setChecked(Misc::transformPTTInputEnable(settings->getPTTInputEnabled(),SETTING_PTT_INPUT_WK));

  ui->checkBoxPTTInputWinKeyRTSInvert->setChecked(Misc::transformPTTInputEnable(settings->getPTTInputInvert(),SETTING_PTT_INPUT_WK_RTS));
  ui->checkBoxPTTInputWinKeyDTRInvert->setChecked(Misc::transformPTTInputEnable(settings->getPTTInputInvert(),SETTING_PTT_INPUT_WK_DTR));
  ui->checkBoxPTTInputFSKCWRTSInvert->setChecked(Misc::transformPTTInputEnable(settings->getPTTInputInvert(),SETTING_PTT_INPUT_FSKCW_RTS));
  ui->checkBoxPTTInputVOXInvert->setChecked(Misc::transformPTTInputEnable(settings->getPTTInputInvert(),SETTING_PTT_INPUT_VOX_SENSE));

  state = false;
  ui->checkBoxPTTInputFootswitch->blockSignals(state);
  ui->checkBoxPTTInputWinKeyRTS->blockSignals(state);
  ui->checkBoxPTTInputWinKeyDTR->blockSignals(state);
  ui->checkBoxPTTInputFSKCWRTS->blockSignals(state);
  ui->checkBoxPTTInputVOXSense->blockSignals(state);
  ui->checkBoxPTTInputWinKeyRTSInvert->blockSignals(state);
  ui->checkBoxPTTInputWinKeyDTRInvert->blockSignals(state);
  ui->checkBoxPTTInputFSKCWRTSInvert->blockSignals(state);
  ui->checkBoxPTTInputVOXInvert->blockSignals(state);
  ui->checkBoxPTTInputWinKey->blockSignals(state);

  state = true;
  ui->comboBoxPTTOutputPhone->blockSignals(state);
  ui->comboBoxPTTOutputCW->blockSignals(state);
  ui->comboBoxPTTOutputDigital->blockSignals(state);
  ui->checkBoxPTTOutputAmpflier->blockSignals(state);
  ui->checkBoxPTTOutputInhibit->blockSignals(state);
  ui->checkBoxPTTOutputInhibitInverted->blockSignals(state);
  ui->spinBoxPTTAmplifierPostDelay->blockSignals(state);
  ui->spinBoxPTTAmplifierPostDelay->blockSignals(state);
  ui->spinBoxPTTAmplifierPreDelay->blockSignals(state);
  ui->spinBoxPTTAntennaPostDelay->blockSignals(state);
  ui->spinBoxPTTAntennaPreDelay->blockSignals(state);
  ui->spinBoxPTTInhibitPostDelay->blockSignals(state);
  ui->spinBoxPTTInhibitPreDelay->blockSignals(state);
  ui->spinBoxPTTRadioPostDelay->blockSignals(state);
  ui->spinBoxPTTRadioPreDelay->blockSignals(state);

  ui->comboBoxPTTOutputPhone->setCurrentIndex(settings->getPTTOutputPhoneConf());
  ui->comboBoxPTTOutputCW->setCurrentIndex(settings->getPTTOutputCWConf());
  ui->comboBoxPTTOutputDigital->setCurrentIndex(settings->getPTTOutputDigitalConf());
  ui->checkBoxPTTOutputAmpflier->setChecked(settings->getPTTOutputAmplifierEnabled());
  ui->checkBoxPTTOutputInhibit->setChecked(settings->getPTTOutputInhibitEnabled());
  ui->checkBoxPTTOutputInhibitInverted->setChecked(settings->getPTTOutputInhibitInverted());
  ui->spinBoxPTTAmplifierPostDelay->setValue(settings->getPTTAmpPostDelay());
  ui->spinBoxPTTAmplifierPreDelay->setValue(settings->getPTTAmpPreDelay());
  ui->spinBoxPTTAntennaPostDelay->setValue(settings->getPTTAntennaPostDelay());
  ui->spinBoxPTTAntennaPreDelay->setValue(settings->getPTTAntennaPreDelay());
  ui->spinBoxPTTInhibitPostDelay->setValue(settings->getPTTInhibitPostDelay());
  ui->spinBoxPTTInhibitPreDelay->setValue(settings->getPTTInhibitPreDelay());
  ui->spinBoxPTTRadioPostDelay->setValue(settings->getPTTRadioPostDelay());
  ui->spinBoxPTTRadioPreDelay->setValue(settings->getPTTRadioPreDelay());


  state = false;
  ui->comboBoxPTTOutputPhone->blockSignals(state);
  ui->comboBoxPTTOutputCW->blockSignals(state);
  ui->comboBoxPTTOutputDigital->blockSignals(state);
  ui->checkBoxPTTOutputAmpflier->blockSignals(state);
  ui->checkBoxPTTOutputInhibit->blockSignals(state);
  ui->checkBoxPTTOutputInhibitInverted->blockSignals(state);
  ui->spinBoxPTTAmplifierPostDelay->blockSignals(state);
  ui->spinBoxPTTAmplifierPostDelay->blockSignals(state);
  ui->spinBoxPTTAmplifierPreDelay->blockSignals(state);
  ui->spinBoxPTTAntennaPostDelay->blockSignals(state);
  ui->spinBoxPTTAntennaPreDelay->blockSignals(state);
  ui->spinBoxPTTInhibitPostDelay->blockSignals(state);
  ui->spinBoxPTTInhibitPreDelay->blockSignals(state);
  ui->spinBoxPTTRadioPostDelay->blockSignals(state);
  ui->spinBoxPTTRadioPreDelay->blockSignals(state);
  //********* END PTT SETTINGS *********//

  //********* START CW SETTINGS *********//
  state = true;
  ui->checkBoxCWSourceWinkey->blockSignals(state);
  ui->checkBoxCWSourceWinkeyUSBDTR->blockSignals(state);
  ui->checkBoxCWSourceWinkeyUSBRTS->blockSignals(state);
  ui->checkBoxCWSourceUSBDTR->blockSignals(state);

  ui->checkBoxCWSourceWinkey->setChecked(Misc::transformCWInputSource(settings->getCWInputSource(),SETTING_CW_INPUT_SOURCE_WINKEY));
  ui->checkBoxCWSourceWinkeyUSBDTR->setChecked(Misc::transformCWInputSource(settings->getCWInputSource(),SETTING_CW_INPUT_SOURCE_WINKEY_DTR));
  ui->checkBoxCWSourceWinkeyUSBRTS->setChecked(Misc::transformCWInputSource(settings->getCWInputSource(),SETTING_CW_INPUT_SOURCE_WINKEY_RTS));
  ui->checkBoxCWSourceUSBDTR->setChecked(Misc::transformCWInputSource(settings->getCWInputSource(),SETTING_CW_INPUT_SOURCE_USB_DTR));

  state = false;
  ui->checkBoxCWSourceWinkey->blockSignals(state);
  ui->checkBoxCWSourceWinkeyUSBDTR->blockSignals(state);
  ui->checkBoxCWSourceWinkeyUSBRTS->blockSignals(state);
  ui->checkBoxCWSourceUSBDTR->blockSignals(state);
  //********* END CW SETTINGS *********//


  //********* START MISC SETTINGS *********//
  state = true;
  ui->spinBoxDisplayBacklightRed->blockSignals(state);
  ui->spinBoxDisplayBacklightGreen->blockSignals(state);
  ui->spinBoxDisplayBacklightBlue->blockSignals(state);
  ui->lineEditDisplayTextCallsign->blockSignals(state);

  ui->spinBoxDisplayBacklightRed->setValue(settings->getBacklightRGBColorRed());
  ui->spinBoxDisplayBacklightGreen->setValue(settings->getBacklightRGBColorGreen());
  ui->spinBoxDisplayBacklightBlue->setValue(settings->getBacklightRGBColorBlue());
  ui->lineEditDisplayTextCallsign->setText(settings->getCallsign());

  state = false;
  ui->spinBoxDisplayBacklightRed->blockSignals(state);
  ui->spinBoxDisplayBacklightGreen->blockSignals(state);
  ui->spinBoxDisplayBacklightBlue->blockSignals(state);
  ui->lineEditDisplayTextCallsign->blockSignals(state);
  //********* END MISC SETTINGS *********//

  //********* START DIGITAL SETTINGS *********//
  state = true;

  ui->comboBoxDigitalFSKBaudrate->blockSignals(state);
  ui->comboBoxDigitalFSKBitLength->blockSignals(state);
  ui->comboBoxDigitalFSKParity->blockSignals(state);
  ui->comboBoxDigitalFSKStopbits->blockSignals(state);

  ui->comboBoxDigitalFSKBaudrate->setCurrentIndex(settings->getDigitalFSKBaudrate());
  ui->comboBoxDigitalFSKBitLength->setCurrentIndex(settings->getDigitalFSKBitLength());
  ui->comboBoxDigitalFSKParity->setCurrentIndex(settings->getDigitalFSKParity());
  ui->comboBoxDigitalFSKStopbits->setCurrentIndex(settings->getDigitalFSKStopbits());

  state = false;
  ui->comboBoxDigitalFSKBaudrate->blockSignals(state);
  ui->comboBoxDigitalFSKBitLength->blockSignals(state);
  ui->comboBoxDigitalFSKParity->blockSignals(state);
  ui->comboBoxDigitalFSKStopbits->blockSignals(state);
  //********* END DIGITAL SETTINGS *********//

  //********* START WINKEY SETTINGS *********//
  state = true;
  ui->comboBoxWinkeyMode->blockSignals(state);
  ui->checkBoxWinkeySwapPaddles->blockSignals(state);
  ui->checkBoxWinkeyAutoSpace->blockSignals(state);
  ui->checkBoxWinkeyCTSpace->blockSignals(state);
  ui->comboBoxWinkeyHangTime->blockSignals(state);
  ui->spinBoxWinkeyPotLowSpeed->blockSignals(state);
  ui->spinBoxWinkeyPotHighSpeed->blockSignals(state);
  ui->spinBoxWinkeyFarnsworth->blockSignals(state);
  ui->spinBoxWinkeyWeight->blockSignals(state);
  ui->spinBoxWinkeyDihDahRatio->blockSignals(state);
  ui->spinBoxWinkey1stExtension->blockSignals(state);
  ui->spinBoxWinkeyKeyCompensation->blockSignals(state);
  ui->groupBoxWinkeyPaddleMemory->blockSignals(state);
  ui->radioButtonWinkeyPaddleMemoryPriorityDit->blockSignals(state);
  ui->radioButtonWinkeyPaddleMemoryPriorityDah->blockSignals(state);
  ui->spinBoxWinkeyPaddleMemorySetPoint->blockSignals(state);
  ui->checkBoxWinkeyPaddleEchoback->blockSignals(state);
  ui->checkBoxWinkeySerialEchoback->blockSignals(state);
  ui->spinBoxWinkeyDefaultSpeed->blockSignals(state);
  ui->spinBoxWinkeyLeadTime->blockSignals(state);
  ui->spinBoxWinkeyTailTime->blockSignals(state);

  ui->comboBoxWinkeyMode->setCurrentIndex(settings->getWinkeyKeyerMode());
  ui->checkBoxWinkeySwapPaddles->setChecked(settings->getWinkeySwapPaddles());
  ui->checkBoxWinkeyAutoSpace->setChecked(settings->getWinkeyAutoSpace());
  ui->checkBoxWinkeyCTSpace->setChecked(settings->getWinkeyCTSpace());
  ui->comboBoxWinkeyHangTime->setCurrentIndex(settings->getWinkeyHangtime());
  ui->spinBoxWinkeyPotLowSpeed->setValue(settings->getWinkeySpeedpotMin());
  ui->spinBoxWinkeyPotHighSpeed->setValue(settings->getWinkeySpeedpotMax());
  ui->spinBoxWinkeyFarnsworth->setValue(settings->getWinkeyFarnsworth());
  ui->spinBoxWinkeyWeight->setValue(settings->getWinkeyWeight());
  ui->spinBoxWinkeyDihDahRatio->setValue(settings->getWinkeyDihDahRatio());
  ui->spinBoxWinkey1stExtension->setValue(settings->getWinkeyFirstExtension());
  ui->spinBoxWinkeyKeyCompensation->setValue(settings->getWinkeyKeyCompensation());
  ui->groupBoxWinkeyPaddleMemory->setChecked(settings->getWinkeyPaddleMemory());
  ui->spinBoxWinkeyLeadTime->setValue(settings->getWinkeyLeadTime());
  ui->spinBoxWinkeyTailTime->setValue(settings->getWinkeyTailTime());

  if (settings->getWinkeyPriority()) {
    ui->radioButtonWinkeyPaddleMemoryPriorityDit->setChecked(true);
    ui->radioButtonWinkeyPaddleMemoryPriorityDah->setChecked(false);
  }
  else {
    ui->radioButtonWinkeyPaddleMemoryPriorityDit->setChecked(false);
    ui->radioButtonWinkeyPaddleMemoryPriorityDah->setChecked(true);
  }

  ui->spinBoxWinkeyPaddleMemorySetPoint->setValue(settings->getWinkeyPaddleMemorySwPoint());
  ui->checkBoxWinkeyPaddleEchoback->setChecked(settings->getWinkeyPaddleEchoBack());
  ui->checkBoxWinkeySerialEchoback->setChecked(settings->getWinkeySerialEchoBack());
  ui->spinBoxWinkeyDefaultSpeed->setValue(settings->getWinkeyDefaultSpeed());

  state = false;
  ui->comboBoxWinkeyMode->blockSignals(state);
  ui->checkBoxWinkeySwapPaddles->blockSignals(state);
  ui->checkBoxWinkeyAutoSpace->blockSignals(state);
  ui->checkBoxWinkeyCTSpace->blockSignals(state);
  ui->comboBoxWinkeyHangTime->blockSignals(state);
  ui->spinBoxWinkeyPotLowSpeed->blockSignals(state);
  ui->spinBoxWinkeyPotHighSpeed->blockSignals(state);
  ui->spinBoxWinkeyFarnsworth->blockSignals(state);
  ui->spinBoxWinkeyWeight->blockSignals(state);
  ui->spinBoxWinkeyDihDahRatio->blockSignals(state);
  ui->spinBoxWinkey1stExtension->blockSignals(state);
  ui->spinBoxWinkeyKeyCompensation->blockSignals(state);
  ui->groupBoxWinkeyPaddleMemory->blockSignals(state);
  ui->radioButtonWinkeyPaddleMemoryPriorityDit->blockSignals(state);
  ui->radioButtonWinkeyPaddleMemoryPriorityDah->blockSignals(state);
  ui->spinBoxWinkeyPaddleMemorySetPoint->blockSignals(state);
  ui->checkBoxWinkeyPaddleEchoback->blockSignals(state);
  ui->checkBoxWinkeySerialEchoback->blockSignals(state);
  ui->spinBoxWinkeyDefaultSpeed->blockSignals(state);
  ui->spinBoxWinkeyLeadTime->blockSignals(state);
  ui->spinBoxWinkeyTailTime->blockSignals(state);
  //********* END WINKEY SETTINGS *********//

  //********* START DISPLAY SETTINGS *********//
  state = true;
  ui->comboBoxDisplayTextCWLine1RX->blockSignals(state);
  ui->comboBoxDisplayTextCWLine2RX->blockSignals(state);
  ui->comboBoxDisplayTextCWLine1TX->blockSignals(state);
  ui->comboBoxDisplayTextCWLine2TX->blockSignals(state);
  ui->comboBoxDisplayTextPhoneLine1RX->blockSignals(state);
  ui->comboBoxDisplayTextPhoneLine2RX->blockSignals(state);
  ui->comboBoxDisplayTextPhoneLine1TX->blockSignals(state);
  ui->comboBoxDisplayTextPhoneLine2TX->blockSignals(state);
  ui->comboBoxDisplayTextDigitalLine1RX->blockSignals(state);
  ui->comboBoxDisplayTextDigitalLine2RX->blockSignals(state);
  ui->comboBoxDisplayTextDigitalLine1TX->blockSignals(state);
  ui->comboBoxDisplayTextDigitalLine2TX->blockSignals(state);


  ui->comboBoxDisplayTextCWLine1RX->setCurrentIndex(settings->getDisplay(DISPLAY_LINE_ID_LINE1_RX,DISPLAY_MODE_CW));
  ui->comboBoxDisplayTextCWLine2RX->setCurrentIndex(settings->getDisplay(DISPLAY_LINE_ID_LINE2_RX,DISPLAY_MODE_CW));
  ui->comboBoxDisplayTextCWLine1TX->setCurrentIndex(settings->getDisplay(DISPLAY_LINE_ID_LINE1_TX,DISPLAY_MODE_CW));
  ui->comboBoxDisplayTextCWLine2TX->setCurrentIndex(settings->getDisplay(DISPLAY_LINE_ID_LINE2_TX,DISPLAY_MODE_CW));
  ui->comboBoxDisplayTextPhoneLine1RX->setCurrentIndex(settings->getDisplay(DISPLAY_LINE_ID_LINE1_RX,DISPLAY_MODE_PHONE));
  ui->comboBoxDisplayTextPhoneLine2RX->setCurrentIndex(settings->getDisplay(DISPLAY_LINE_ID_LINE2_RX,DISPLAY_MODE_PHONE));
  ui->comboBoxDisplayTextPhoneLine1TX->setCurrentIndex(settings->getDisplay(DISPLAY_LINE_ID_LINE1_TX,DISPLAY_MODE_PHONE));
  ui->comboBoxDisplayTextPhoneLine2TX->setCurrentIndex(settings->getDisplay(DISPLAY_LINE_ID_LINE2_TX,DISPLAY_MODE_PHONE));

  ui->comboBoxDisplayTextDigitalLine1RX->setCurrentIndex(settings->getDisplay(DISPLAY_LINE_ID_LINE1_RX,DISPLAY_MODE_DIGITAL));
  ui->comboBoxDisplayTextDigitalLine2RX->setCurrentIndex(settings->getDisplay(DISPLAY_LINE_ID_LINE2_RX,DISPLAY_MODE_DIGITAL));
  ui->comboBoxDisplayTextDigitalLine1TX->setCurrentIndex(settings->getDisplay(DISPLAY_LINE_ID_LINE1_TX,DISPLAY_MODE_DIGITAL));
  ui->comboBoxDisplayTextDigitalLine2TX->setCurrentIndex(settings->getDisplay(DISPLAY_LINE_ID_LINE2_TX,DISPLAY_MODE_DIGITAL));


  state = false;
  ui->comboBoxDisplayTextCWLine1RX->blockSignals(state);
  ui->comboBoxDisplayTextCWLine2RX->blockSignals(state);
  ui->comboBoxDisplayTextCWLine1TX->blockSignals(state);
  ui->comboBoxDisplayTextCWLine2TX->blockSignals(state);
  ui->comboBoxDisplayTextPhoneLine1RX->blockSignals(state);
  ui->comboBoxDisplayTextPhoneLine2RX->blockSignals(state);
  ui->comboBoxDisplayTextPhoneLine1TX->blockSignals(state);
  ui->comboBoxDisplayTextPhoneLine2TX->blockSignals(state);
  ui->comboBoxDisplayTextDigitalLine1RX->blockSignals(state);
  ui->comboBoxDisplayTextDigitalLine2RX->blockSignals(state);
  ui->comboBoxDisplayTextDigitalLine1TX->blockSignals(state);
  ui->comboBoxDisplayTextDigitalLine2TX->blockSignals(state);

  //********* END DISPLAY SETTINGS *********//

  QColor color;
  color.setRed(settings->getBacklightRGBColorRed());
  color.setGreen(settings->getBacklightRGBColorGreen());
  color.setBlue(settings->getBacklightRGBColorBlue());
  color.setAlpha(255);
  updateBacklightColor(color);
}

void MainWindow::msgBoxClosed(QAbstractButton* button) {

}


void MainWindow::showMessageBox(QString title, QString text, enum QMessageBox::Icon icon) {
   QMessageBox* msgBox = new QMessageBox(this);
   msgBox->setIcon(icon);
   msgBox->setAttribute(Qt::WA_DeleteOnClose);
   msgBox->setStandardButtons(QMessageBox::Ok);
   msgBox->setWindowTitle(title);
   msgBox->setText(text);
   msgBox->setModal(false);
   msgBox->open(this, SLOT(msgBoxClosed(QAbstractButton*)));
}

void MainWindow::parseCommData(StructMessage message) {
  if (message.cmd == SRI_CMD_CONNECT) {
    QString msg;
    msg.append("Connected to: ");
    msg.append(currentSRIPort);
    msg.append(" - Firmware rev: ");

    for (unsigned char i=0;i<message.length;i++)
      msg.append(message.data[i]);

    statusBar()->showMessage(msg, 5000);

    lastPongCounter = 0;
  }
  else if (message.cmd == SRI_CMD_SEND_SETTINGS) {
    //qDebug() << "LENGTH: " << message.length;
    //qDebug() << "Offset: " << (quint16)((message.data[0] << 8) + message.data[1]);
    settings->newDataToStruct(message.data+2, (quint16)((message.data[0] << 8) + message.data[1]), message.length-2);

    lastPongCounter = 0;
  }
  else if (message.cmd == SRI_CMD_GET_FIRMWARE_REV) {
    QString msg;
    msg.append("openSRI current\nfirmware version: ");

    for (unsigned char i=0;i<message.length;i++)
      msg.append(message.data[i]);

    showMessageBox("Firmware version",msg,QMessageBox::Information);

    lastPongCounter = 0;
  }
  else if (message.cmd == SRI_CMD_EEPROM_SAVE) {
    QString msg;
    msg.append("Settings were successfully saved to\nthe SRI interface EEPROM");

    for (unsigned char i=0;i<message.length;i++)
      msg.append(message.data[i]);

    showMessageBox("Saved settings",msg,QMessageBox::Information);

    lastPongCounter = 0;
  }
  else if (message.cmd == SRI_CMD_UPDATE_UI) {
    //qDebug("UPDATE UI");
    updateUIfromSettings();

    lastPongCounter = 0;
  }
  else if (message.cmd == SRI_CMD_SEND_STATUS) {
    status->newDataToStruct(message.data+2, (quint16)((message.data[0] << 8) + message.data[1]), message.length-2);

    updateUIfromStatus();

    lastPongCounter = 0;
  }
  else if (message.cmd == SRI_CMD_SEND_PING) {
    if (serialPort->isOpen()) {
      //Send a PONG back to the SRI interface to know we are alive
      serialPort->addTXMessage(SRI_CMD_SEND_PONG);
      //qDebug("PING CMD");
      lastPongCounter = 0;
    }
  }
  else if (message.cmd == SRI_CMD_SEND_PONG) {
    //qDebug("PONG CMD");
    lastPongCounter = 0;
  }
  else if (message.cmd == SRI_CMD_WINKEY) {
    if (message.data[0] == SRI_CMD_SUB_WINKEY_CHARS_SENT) {
      ui->lineEditWinkeyText->setText(ui->lineEditWinkeyText->text() + message.data[1]);

      lastPongCounter = 0;
    }
  }
  else if (message.cmd == SRI_CMD_DEBUG_TEXT) {
    QString text;
    for (quint8 i=0; i<message.length;i++)
      text.append(message.data[i]);

    if (rowCount == 0) {
      ui->textEditDebug->setTextColor(QColor("blue"));
      ui->textEditDebug->insertPlainText(text);
      rowCount = 1;
    }
    else {
      ui->textEditDebug->setTextColor(QColor("black"));
      ui->textEditDebug->insertPlainText(text);
      rowCount = 0;
    }

    ui->textEditDebug->moveCursor (QTextCursor::End);
  }
  else if (message.cmd == SRI_CMD_CW_MESSAGE) {
    if (message.data[0] == SRI_CMD_SUB_SET_CW_MESSAGE) {
      QString str;

      for (quint8 i=2;i<message.length;i++)
        str.append(message.data[i]);

      settings->setCWMessageNoUpdate(message.data[1],str);

      switch(message.data[1]) {
        case 0: ui->lineEditCWMessage1->setText(str);
                break;
        case 1: ui->lineEditCWMessage2->setText(str);
                break;
        case 2: ui->lineEditCWMessage3->setText(str);
                break;
        case 3: ui->lineEditCWMessage4->setText(str);
                break;
        case 4: ui->lineEditCWMessage5->setText(str);
                break;
        case 5: ui->lineEditCWMessage6->setText(str);
                break;
        case 6: ui->lineEditCWMessage7->setText(str);
                break;
        case 7: ui->lineEditCWMessage8->setText(str);
                break;
        default:
                break;
      }
    }
  }
}

void MainWindow::timerPollStatusUpdate() {
  serialPort->processData();

  if (serialPort->getRXQueueSize()) {
    parseCommData(serialPort->getRXQueueFirst());
  }
}

void MainWindow::timerPingPongEventUpdate() {
  if (serialPort->isOpen()) {
    serialPort->addTXMessage(SRI_CMD_SEND_PING);

    lastPongCounter++;

    if (lastPongCounter >= SRI_PING_TIMEOUT_LIMIT) {
      lastPongCounter = 0;
      on_actionDisconnect_triggered();

      showMessageBox("Warning","The SRI interface is not responding",QMessageBox::Warning);

    }
  }
}

void MainWindow::on_actionConnect_triggered() {
  currentSRIPort = dlgSelectPort->getPortName();

  if (currentSRIPort != "Undefined") {
    if (serialPort->openPort(currentSRIPort,250000)) {
      ui->actionConnect->setEnabled(false);
      ui->actionDisconnect->setEnabled(true);

      timerPollStatus->setInterval(TIMER_POLL_STATUS_INTERVAL);
      timerPollStatus->start();

      timerPingPongEvent->setInterval(SRI_PING_INTERVAL);
      timerPingPongEvent->start();

      serialPort->addTXMessage(SRI_CMD_CONNECT);
    }
  }
  else {
    QMessageBox::warning(this, tr("Warning"),tr("No port has been defined to which\nthe SRI interface is connected to"));

    dlgSelectPort->refreshPorts();
    dlgSelectPort->show();
  }
}

void MainWindow::disconnectSerialPort() {
  if (serialPort->isOpen()) {
    timerPollStatus->stop();

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);

    QString msg;
    msg.append("Disconnected from: ");
    msg.append(currentSRIPort);
    statusBar()->showMessage(msg, 5000);

    serialPort->closePort();
  }
}

void MainWindow::on_actionDisconnect_triggered() {
  if (serialPort->isOpen()) {
    serialPort->addTXMessage(SRI_CMD_DISCONNECT);

    //Wait two seconds before we disconnect the serial port, so that
    //the message has time to be sent to the uC
    QTimer::singleShot(1000, this, SLOT(disconnectSerialPort()));
  }
}

void MainWindow::killTheProgram() {
  if (serialPort->isOpen())
    serialPort->closePort();

  exit(0);
}

void MainWindow::on_actionQuit_triggered() {
  QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Smart Radio Interface",
                                        tr("Are you sure you wish to quit?\n"),
                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::Yes);
if (resBtn != QMessageBox::Yes) {

  } else {
    on_actionDisconnect_triggered();
    QTimer::singleShot(1000, this, SLOT(killTheProgram()));
  }
}

void MainWindow::on_actionTestButton_triggered() {
  serialPort->addTXMessage(SRI_CMD_TEST_COMMAND);
}

void MainWindow::on_pushButtonCATSettingsApply_clicked() {
  quint32 baudrate = 0;

  baudrate = Misc::transformIndexToBaudrate(ui->comboBoxCATBaudrate->currentIndex());

  if (ui->groupBoxCATInterface->isChecked())
    settings->setRadioCATInterfaceEnabled(1);
  else
    settings->setRadioCATInterfaceEnabled(0);

  settings->setRadioCATBaudrate(baudrate);
  settings->setRadioModel(ui->comboBoxRadioModel->currentIndex());
  settings->setRadioCATParity(ui->comboBoxCATParity->currentIndex());
  settings->setRadioCATStopbits(ui->comboBoxCATStopbits->currentIndex());

  if (ui->checkBoxCATJumperRTSCTS->isChecked())
    settings->setRadioCATJumperRTSCTS(1);
  else
    settings->setRadioCATJumperRTSCTS(0);

  if (ui->checkBoxCATFlowControl->isChecked())
    settings->setRadioCATFlowControl(1);
  else
    settings->setRadioCATFlowControl(0);

  settings->setRadioCATICOMCIV(ui->spinBoxCATCIVAddress->value());

  settings->setRadioCATRetrieveType(ui->comboBoxCATRetrieveType->currentIndex());
  settings->setRadioCATPollRate(ui->spinBoxCATPollingRate->value());

  serialPort->addTXMessage(SRI_CMD_CAT_SETTINGS_CHANGED);
}

void MainWindow::on_actionGetVersion_triggered() {
  serialPort->addTXMessage(SRI_CMD_GET_FIRMWARE_REV);
}

void MainWindow::actionPTTInputClicked() {
  quint8 state = 0;

  if (ui->checkBoxPTTInputFootswitch->isChecked())
    state |= (1<<SETTING_PTT_INPUT_FOOTSWITCH);
  if (ui->checkBoxPTTInputFSKCWRTS->isChecked())
    state |= (1<<SETTING_PTT_INPUT_FSKCW_RTS);
  if (ui->checkBoxPTTInputVOXSense->isChecked())
    state |= (1<<SETTING_PTT_INPUT_VOX_SENSE);
  if (ui->checkBoxPTTInputWinKeyDTR->isChecked())
    state |= (1<<SETTING_PTT_INPUT_WK_DTR);
  if (ui->checkBoxPTTInputWinKeyRTS->isChecked())
    state |= (1<<SETTING_PTT_INPUT_WK_RTS);
  if (ui->checkBoxPTTInputWinKey->isChecked())
    state |= (1<<SETTING_PTT_INPUT_WK);

  settings->setPTTInputEnabled(state);
}

void MainWindow::actionPTTInputInvertClicked() {
  quint8 state = 0;

  if (ui->checkBoxPTTInputFSKCWRTSInvert->isChecked())
    state |= (1<<SETTING_PTT_INPUT_FSKCW_RTS);
  if (ui->checkBoxPTTInputVOXInvert->isChecked())
    state |= (1<<SETTING_PTT_INPUT_VOX_SENSE);
  if (ui->checkBoxPTTInputWinKeyDTRInvert->isChecked())
    state |= (1<<SETTING_PTT_INPUT_WK_DTR);
  if (ui->checkBoxPTTInputWinKeyRTSInvert->isChecked())
    state |= (1<<SETTING_PTT_INPUT_WK_RTS);

  settings->setPTTInputInvert(state);
}

void MainWindow::on_comboBoxPTTOutputPhone_currentIndexChanged(int index) {
    settings->setPTTOutputPhoneConf(index);
}

void MainWindow::on_comboBoxPTTOutputCW_currentIndexChanged(int index) {
  settings->setPTTOutputCWConf(index);
}


void MainWindow::on_comboBoxPTTOutputDigital_currentIndexChanged(int index){
  settings->setPTTOutputDigitalConf(index);
}

void MainWindow::on_checkBoxPTTOutputInhibit_toggled(bool checked) {
  if (checked)
    settings->setPTTOutputInhibitEnabled(1);
  else
    settings->setPTTOutputInhibitEnabled(0);
}

void MainWindow::on_checkBoxPTTOutputAmpflier_toggled(bool checked) {
  if (checked)
    settings->setPTTOutputAmplifierEnabled(1);
  else
    settings->setPTTOutputAmplifierEnabled(0);
}

void MainWindow::on_spinBoxPTTRadioPreDelay_valueChanged(int arg1) {
  settings->setPTTRadioPreDelay(arg1);
  serialPort->addTXMessage(SRI_CMD_SEQ_SETTINGS_CHANGED, SRI_CMD_SUB_SEQ_RADIO_DELAY_CHANGED);
}

void MainWindow::on_spinBoxPTTRadioPostDelay_valueChanged(int arg1) {
  settings->setPTTRadioPostDelay(arg1);
  serialPort->addTXMessage(SRI_CMD_SEQ_SETTINGS_CHANGED, SRI_CMD_SUB_SEQ_RADIO_DELAY_CHANGED);
}

void MainWindow::on_spinBoxPTTAmplifierPreDelay_valueChanged(int arg1) {
  settings->setPTTAmpPreDelay(arg1);
  serialPort->addTXMessage(SRI_CMD_SEQ_SETTINGS_CHANGED);
}

void MainWindow::on_spinBoxPTTAmplifierPostDelay_valueChanged(int arg1) {
  settings->setPTTAmpPostDelay(arg1);
  serialPort->addTXMessage(SRI_CMD_SEQ_SETTINGS_CHANGED);
}

void MainWindow::on_spinBoxPTTInhibitPreDelay_valueChanged(int arg1) {
  settings->setPTTInhibitPreDelay(arg1);
  serialPort->addTXMessage(SRI_CMD_SEQ_SETTINGS_CHANGED);
}

void MainWindow::on_spinBoxPTTInhibitPostDelay_valueChanged(int arg1) {
  settings->setPTTInhibitPostDelay(arg1);
  serialPort->addTXMessage(SRI_CMD_SEQ_SETTINGS_CHANGED);
}

void MainWindow::on_spinBoxPTTAntennaPreDelay_valueChanged(int arg1) {
  settings->setPTTAntennaPreDelay(arg1);
  serialPort->addTXMessage(SRI_CMD_SEQ_SETTINGS_CHANGED);
}

void MainWindow::on_spinBoxPTTAntennaPostDelay_valueChanged(int arg1) {
  settings->setPTTAntennaPostDelay(arg1);
  serialPort->addTXMessage(SRI_CMD_SEQ_SETTINGS_CHANGED);
}

void MainWindow::on_actionSaveToEEPROM_triggered() {
  settings->saveToEEPROM();
}

void MainWindow::on_comboBoxAudioRoutingScenario_currentIndexChanged(int index) {
  bool blockState = true;
  ui->comboBoxAudioRoutingLineInCW->blockSignals(blockState);
  ui->comboBoxAudioRoutingLineInPhone->blockSignals(blockState);
  ui->comboBoxAudioRoutingLineInDigital->blockSignals(blockState);
  ui->comboBoxAudioRoutingMicPath->blockSignals(blockState);
  ui->checkBoxAudioRoutingMuteMic->blockSignals(blockState);
  ui->checkBoxAudioRoutingPreamp->blockSignals(blockState);
  ui->checkBoxAudioRoutingExtPreamp->blockSignals(blockState);

  ui->comboBoxAudioRoutingLineInCW->setCurrentIndex(settings->getAudioRoutingLineInSourceCW(index));
  ui->comboBoxAudioRoutingLineInPhone->setCurrentIndex(settings->getAudioRoutingLineInSourcePhone(index));
  ui->comboBoxAudioRoutingLineInDigital->setCurrentIndex(settings->getAudioRoutingLineInSourceDigital(index));
  ui->comboBoxAudioRoutingMicPath->setCurrentIndex(settings->getAudioRoutingMicPath(index));
  ui->checkBoxAudioRoutingMuteMic->setChecked(settings->getAudioRoutingMicMute(index));
  ui->checkBoxAudioRoutingPreamp->setChecked(settings->getAudioRoutingMicPreamp(index));
  ui->checkBoxAudioRoutingExtPreamp->setChecked(settings->getAudioRoutingMicExtPreamp(index));

  blockState = false;
  ui->comboBoxAudioRoutingLineInCW->blockSignals(blockState);
  ui->comboBoxAudioRoutingLineInPhone->blockSignals(blockState);
  ui->comboBoxAudioRoutingLineInDigital->blockSignals(blockState);
  ui->comboBoxAudioRoutingMicPath->blockSignals(blockState);
  ui->checkBoxAudioRoutingMuteMic->blockSignals(blockState);
  ui->checkBoxAudioRoutingPreamp->blockSignals(blockState);
  ui->checkBoxAudioRoutingExtPreamp->blockSignals(blockState);
}

void MainWindow::on_comboBoxAudioRoutingLineInCW_currentIndexChanged(int index) {
  settings->setAudioRoutingLineInSourceCW(ui->comboBoxAudioRoutingScenario->currentIndex(),index);
  serialPort->addTXMessage(SRI_CMD_AUDIO_SETTINGS_CHANGED);
}

void MainWindow::on_comboBoxAudioRoutingLineInPhone_currentIndexChanged(int index) {
  settings->setAudioRoutingLineInSourcePhone(ui->comboBoxAudioRoutingScenario->currentIndex(),index);
  serialPort->addTXMessage(SRI_CMD_AUDIO_SETTINGS_CHANGED);
}


void MainWindow::on_comboBoxAudioRoutingLineInDigital_currentIndexChanged(int index) {
  settings->setAudioRoutingLineInSourceDigital(ui->comboBoxAudioRoutingScenario->currentIndex(),index);
  serialPort->addTXMessage(SRI_CMD_AUDIO_SETTINGS_CHANGED);
}

void MainWindow::on_comboBoxAudioRoutingMicPath_currentIndexChanged(int index) {
  settings->setAudioRoutingMicPath(ui->comboBoxAudioRoutingScenario->currentIndex(),index);
  serialPort->addTXMessage(SRI_CMD_AUDIO_SETTINGS_CHANGED);
}

void MainWindow::on_checkBoxAudioRoutingMuteMic_clicked(bool checked) {
  settings->setAudioRoutingMicMute(ui->comboBoxAudioRoutingScenario->currentIndex(),checked);
  serialPort->addTXMessage(SRI_CMD_AUDIO_SETTINGS_CHANGED);
}


void MainWindow::on_checkBoxAudioRoutingPreamp_clicked(bool checked) {
  settings->setAudioRoutingMicPreamp(ui->comboBoxAudioRoutingScenario->currentIndex(),checked);
  serialPort->addTXMessage(SRI_CMD_AUDIO_SETTINGS_CHANGED);
}

void MainWindow::on_checkBoxAudioRoutingExtPreamp_clicked(bool checked) {
  settings->setAudioRoutingMicExtPreamp(ui->comboBoxAudioRoutingScenario->currentIndex(), checked);
  serialPort->addTXMessage(SRI_CMD_AUDIO_SETTINGS_CHANGED);
}

void MainWindow::on_checkBoxPTTOutputInhibitInverted_toggled(bool checked) {
  if (checked)
    settings->setPTTOutputInhibitInverted(1);
  else
    settings->setPTTOutputInhibitInverted(0);
}

void MainWindow::actionCWInputSourceToggled() {
  quint8 source = 0;

    if (ui->checkBoxCWSourceWinkey->isChecked())
      source |= (1<<SETTING_CW_INPUT_SOURCE_WINKEY);
    if (ui->checkBoxCWSourceWinkeyUSBRTS->isChecked())
      source |= (1<<SETTING_CW_INPUT_SOURCE_WINKEY_RTS);
    if (ui->checkBoxCWSourceWinkeyUSBDTR->isChecked())
      source |= (1<<SETTING_CW_INPUT_SOURCE_WINKEY_DTR);
    if (ui->checkBoxCWSourceUSBDTR->isChecked())
      source |= (1<<SETTING_CW_INPUT_SOURCE_USB_DTR);

    settings->setCWInputSource(source);
}

void MainWindow::on_pushButtonDisplayBacklightApply_clicked() {
  settings->setBacklightRGBColor(ui->spinBoxDisplayBacklightRed->value(),ui->spinBoxDisplayBacklightGreen->value(),ui->spinBoxDisplayBacklightBlue->value());
  serialPort->addTXMessage(SRI_CMD_DISPLAY_RGB_CHANGED);
}

void MainWindow::on_comboBoxWinkeyMode_currentIndexChanged(int index) {
  settings->setWinkeyKeyerMode(index);
}

void MainWindow::on_checkBoxWinkeySwapPaddles_clicked(bool checked) {
  settings->setWinkeySwapPaddles(checked);
}

void MainWindow::on_checkBoxWinkeyAutoSpace_clicked(bool checked) {
  settings->setWinkeyAutoSpace(checked);
}

void MainWindow::on_checkBoxWinkeyCTSpace_clicked(bool checked) {
  settings->setWinkeyCTSpace(checked);
}

void MainWindow::on_spinBoxWinkeyPotLowSpeed_valueChanged(int arg1) {
  settings->setWinkeySpeedpotMin(arg1);
}

void MainWindow::on_spinBoxWinkeyPotHighSpeed_valueChanged(int arg1) {
  settings->setWinkeySpeedpotMax(arg1);
}

void MainWindow::on_spinBoxWinkeyFarnsworth_valueChanged(int arg1) {
  settings->setWinkeyFarnsworth(arg1);
}

void MainWindow::on_spinBoxWinkeyWeight_valueChanged(int arg1) {
  settings->setWinkeyWeight(arg1);
}

void MainWindow::on_spinBoxWinkeyDihDahRatio_valueChanged(int arg1) {
  settings->setWinkeyDihDahRatio(arg1);
}

void MainWindow::on_spinBoxWinkey1stExtension_valueChanged(int arg1) {
  settings->setWinkeyFirstExtension(arg1);
}

void MainWindow::on_spinBoxWinkeyKeyCompensation_valueChanged(int arg1) {
  settings->setWinkeyKeyCompensation(arg1);
}

void MainWindow::on_groupBoxWinkeyPaddleMemory_clicked(bool checked) {
  settings->setWinkeyPaddleMemory(checked, ui->radioButtonWinkeyPaddleMemoryPriorityDit->isChecked());
}

void MainWindow::on_radioButtonWinkeyPaddleMemoryPriorityDit_clicked(bool checked) {
  settings->setWinkeyPriority(checked);
}

void MainWindow::on_radioButtonWinkeyPaddleMemoryPriorityDah_clicked(bool checked) {
  if (checked)
    settings->setWinkeyPriority(false);
  else
    settings->setWinkeyPriority(true);
}

void MainWindow::on_spinBoxWinkeyPaddleMemorySetPoint_valueChanged(int arg1) {
  settings->setWinkeyPaddleMemorySwPoint(arg1);
}

void MainWindow::on_comboBoxWinkeyHangTime_currentIndexChanged(int index) {
  settings->setWinkeyHangtime(index);
}

void MainWindow::on_checkBoxWinkeySerialEchoback_clicked(bool checked) {
  settings->setWinkeySerialEchoBack(checked);
}

void MainWindow::on_checkBoxWinkeyPaddleEchoback_clicked(bool checked) {
  settings->setWinkeyPaddleEchoBack(checked);
}

void MainWindow::on_spinBoxWinkeyDefaultSpeed_valueChanged(int arg1) {
  settings->setWinkeyDefaultSpeed(arg1);
}

void MainWindow::on_pushButtonCWMessageSave1_clicked() {
  settings->setCWMessage(0, ui->lineEditCWMessage1->text());
}

void MainWindow::on_pushButtonCWMessageSave2_clicked() {
  settings->setCWMessage(1, ui->lineEditCWMessage2->text());
}

void MainWindow::on_pushButtonCWMessageSave3_clicked() {
  settings->setCWMessage(2, ui->lineEditCWMessage3->text());
}

void MainWindow::on_pushButtonCWMessageSave4_clicked() {
  settings->setCWMessage(3, ui->lineEditCWMessage4->text());
}

void MainWindow::on_pushButtonCWMessageSave5_clicked() {
  settings->setCWMessage(4, ui->lineEditCWMessage5->text());
}

void MainWindow::on_pushButtonCWMessageSave6_clicked() {
  settings->setCWMessage(5, ui->lineEditCWMessage6->text());
}

void MainWindow::on_pushButtonCWMessageSave7_clicked() {
  settings->setCWMessage(6, ui->lineEditCWMessage7->text());
}

void MainWindow::on_pushButtonCWMessageSave8_clicked(){
  settings->setCWMessage(7, ui->lineEditCWMessage8->text());
}

void MainWindow::on_pushButtonCWMessagesSaveAll_clicked() {
  settings->setCWMessage(0, ui->lineEditCWMessage1->text());
  settings->setCWMessage(1, ui->lineEditCWMessage2->text());
  settings->setCWMessage(2, ui->lineEditCWMessage3->text());
  settings->setCWMessage(3, ui->lineEditCWMessage4->text());
  settings->setCWMessage(4, ui->lineEditCWMessage5->text());
  settings->setCWMessage(5, ui->lineEditCWMessage6->text());
  settings->setCWMessage(6, ui->lineEditCWMessage7->text());
  settings->setCWMessage(7, ui->lineEditCWMessage8->text());
}

void MainWindow::on_pushButtonCWMessageSend1_clicked() {
  if (serialPort->isOpen()) {
    serialPort->addTXMessage(SRI_CMD_CW_MESSAGE, SRI_CMD_SUB_PLAY_CW_MESSAGE, (qint8)0);
  }
}

void MainWindow::on_pushButtonCWMessageSend2_clicked() {
  if (serialPort->isOpen()) {
    serialPort->addTXMessage(SRI_CMD_CW_MESSAGE, SRI_CMD_SUB_PLAY_CW_MESSAGE, (qint8)1);
  }
}

void MainWindow::on_pushButtonCWMessageSend3_clicked(){
  if (serialPort->isOpen()) {
    serialPort->addTXMessage(SRI_CMD_CW_MESSAGE, SRI_CMD_SUB_PLAY_CW_MESSAGE, (qint8)2);
  }
}

void MainWindow::on_pushButtonCWMessageSend4_clicked() {
  if (serialPort->isOpen()) {
    serialPort->addTXMessage(SRI_CMD_CW_MESSAGE, SRI_CMD_SUB_PLAY_CW_MESSAGE, (qint8)3);
  }
}

void MainWindow::on_pushButtonCWMessageSend5_clicked() {
  if (serialPort->isOpen()) {
    serialPort->addTXMessage(SRI_CMD_CW_MESSAGE, SRI_CMD_SUB_PLAY_CW_MESSAGE, (qint8)4);
  }
}

void MainWindow::on_pushButtonCWMessageSend6_clicked() {
  if (serialPort->isOpen()) {
    serialPort->addTXMessage(SRI_CMD_CW_MESSAGE, SRI_CMD_SUB_PLAY_CW_MESSAGE, (qint8)5);
  }
}

void MainWindow::on_pushButtonCWMessageSend7_clicked() {
  if (serialPort->isOpen()) {
    serialPort->addTXMessage(SRI_CMD_CW_MESSAGE, SRI_CMD_SUB_PLAY_CW_MESSAGE, (qint8)6);
  }
}

void MainWindow::on_pushButtonCWMessageSend8_clicked() {
  if (serialPort->isOpen()) {
    serialPort->addTXMessage(SRI_CMD_CW_MESSAGE, SRI_CMD_SUB_PLAY_CW_MESSAGE, (qint8)7);
  }
}

void MainWindow::on_pushButtonCWMessagesStop_clicked() {
  if (serialPort->isOpen()) {
    serialPort->addTXMessage(SRI_CMD_CW_MESSAGE, SRI_CMD_SUB_STOP_CW_MESSAGE);
  }

}

void MainWindow::on_pushButtonOtherBacklightColorDialog_clicked()
{
  QColor currColor;
  currColor.setRgb(ui->spinBoxDisplayBacklightRed->value(),ui->spinBoxDisplayBacklightGreen->value(),ui->spinBoxDisplayBacklightBlue->value(),255);
  QColor color = QColorDialog::getColor(currColor);

  updateBacklightColor(color);
}

void MainWindow::updateBacklightColor(QColor color) {
  if(color.isValid()) {
    ui->spinBoxDisplayBacklightRed->setValue(color.red());
    ui->spinBoxDisplayBacklightGreen->setValue(color.green());
    ui->spinBoxDisplayBacklightBlue->setValue(color.blue());

    QPalette palette = ui->labelBacklightColorCurrent->palette();
    palette.setColor(ui->labelBacklightColorCurrent->backgroundRole(),color);
    ui->labelBacklightColorCurrent->setPalette(palette);
    ui->labelBacklightColorCurrent->setAutoFillBackground(true);
  }
}

void MainWindow::on_spinBoxDisplayBacklightRed_valueChanged(int arg1) {
  ui->spinBoxDisplayBacklightRed->blockSignals(true);
  QColor color;
  color.setRed(arg1);
  color.setGreen(ui->spinBoxDisplayBacklightGreen->value());
  color.setBlue(ui->spinBoxDisplayBacklightBlue->value());
  color.setAlpha(255);
  updateBacklightColor(color);
  ui->spinBoxDisplayBacklightRed->blockSignals(false);
}

void MainWindow::on_spinBoxDisplayBacklightGreen_valueChanged(int arg1) {
  ui->spinBoxDisplayBacklightGreen->blockSignals(true);
  QColor color;
  color.setRed(ui->spinBoxDisplayBacklightRed->value());
  color.setGreen(arg1);
  color.setBlue(ui->spinBoxDisplayBacklightBlue->value());
  color.setAlpha(255);
  updateBacklightColor(color);
  ui->spinBoxDisplayBacklightGreen->blockSignals(false);
}

void MainWindow::on_spinBoxDisplayBacklightBlue_valueChanged(int arg1) {
  ui->spinBoxDisplayBacklightBlue->blockSignals(true);
  QColor color;
  color.setRed(ui->spinBoxDisplayBacklightRed->value());
  color.setGreen(ui->spinBoxDisplayBacklightGreen->value());
  color.setBlue(arg1);
  color.setAlpha(255);
  updateBacklightColor(color);
  ui->spinBoxDisplayBacklightBlue->blockSignals(false);
}

void MainWindow::on_comboBoxDisplayTextCWLine1RX_currentIndexChanged(int index) {
 settings->setDisplay(DISPLAY_LINE_ID_LINE1_RX, DISPLAY_MODE_CW, display.getIDCW(index),display.getStringCW(index));
}

void MainWindow::on_comboBoxDisplayTextCWLine2RX_currentIndexChanged(int index) {
  settings->setDisplay(DISPLAY_LINE_ID_LINE2_RX, DISPLAY_MODE_CW, display.getIDCW(index),display.getStringCW(index));
}

void MainWindow::on_comboBoxDisplayTextCWLine1TX_currentIndexChanged(int index) {
  settings->setDisplay(DISPLAY_LINE_ID_LINE1_TX, DISPLAY_MODE_CW, display.getIDCW(index),display.getStringCW(index));
}

void MainWindow::on_comboBoxDisplayTextCWLine2TX_currentIndexChanged(int index) {
  settings->setDisplay(DISPLAY_LINE_ID_LINE2_TX, DISPLAY_MODE_CW, display.getIDCW(index),display.getStringCW(index));
}

void MainWindow::on_comboBoxDisplayTextPhoneLine1RX_currentIndexChanged(int index) {
  settings->setDisplay(DISPLAY_LINE_ID_LINE1_RX, DISPLAY_MODE_PHONE, display.getIDPhone(index),display.getStringPhone(index));
}

void MainWindow::on_comboBoxDisplayTextPhoneLine2RX_currentIndexChanged(int index) {
  settings->setDisplay(DISPLAY_LINE_ID_LINE2_RX, DISPLAY_MODE_PHONE, display.getIDPhone(index),display.getStringPhone(index));
}

void MainWindow::on_comboBoxDisplayTextPhoneLine1TX_currentIndexChanged(int index) {
  settings->setDisplay(DISPLAY_LINE_ID_LINE1_TX, DISPLAY_MODE_PHONE, display.getIDPhone(index),display.getStringPhone(index));
}

void MainWindow::on_comboBoxDisplayTextPhoneLine2TX_currentIndexChanged(int index) {
  settings->setDisplay(DISPLAY_LINE_ID_LINE2_TX, DISPLAY_MODE_PHONE, display.getIDPhone(index),display.getStringPhone(index));
}

void MainWindow::on_comboBoxDisplayTextDigitalLine1RX_currentIndexChanged(int index) {
  settings->setDisplay(DISPLAY_LINE_ID_LINE1_RX, DISPLAY_MODE_DIGITAL, display.getIDDigital(index),display.getStringDigital(index));
}

void MainWindow::on_comboBoxDisplayTextDigitalLine2RX_currentIndexChanged(int index) {
  settings->setDisplay(DISPLAY_LINE_ID_LINE2_RX, DISPLAY_MODE_DIGITAL, display.getIDDigital(index),display.getStringDigital(index));
}

void MainWindow::on_comboBoxDisplayTextDigitalLine1TX_currentIndexChanged(int index) {
  settings->setDisplay(DISPLAY_LINE_ID_LINE1_TX, DISPLAY_MODE_DIGITAL, display.getIDDigital(index),display.getStringDigital(index));
}

void MainWindow::on_comboBoxDisplayTextDigitalLine2TX_currentIndexChanged(int index) {
  settings->setDisplay(DISPLAY_LINE_ID_LINE2_TX, DISPLAY_MODE_DIGITAL, display.getIDDigital(index),display.getStringDigital(index));
}

void MainWindow::on_lineEditDisplayTextCallsign_textChanged(const QString &arg1) {
  settings->setCallsign(arg1);
}

void MainWindow::on_spinBoxWinkeyLeadTime_valueChanged(int arg1)
{
  settings->setWinkeyLeadTime(arg1);
}

void MainWindow::on_spinBoxWinkeyTailTime_valueChanged(int arg1)
{
    settings->setWinkeyTailTime(arg1);
}

void MainWindow::checkBoxDebugClicked() {
  quint8 debug_state = 0;
  if (ui->pushButtonEnableDebug->isChecked())
    debug_state |= (1<<0);

  if (ui->checkBoxDebugSystem->isChecked())
    debug_state |= (1<<1);

  if (ui->checkBoxDebugCAT->isChecked())
    debug_state |= (1<<2);

  if (ui->checkBoxDebugWinkey->isChecked())
    debug_state |= (1<<3);

  if (ui->checkBoxDebugSequencer->isChecked())
    debug_state |= (1<<4);

  serialPort->addTXMessage(SRI_CMD_DEBUG_TEXT, debug_state);
}

void MainWindow::on_pushButtonDebugClearWindow_clicked() {
    ui->textEditDebug->clear();
}

void MainWindow::on_checkBoxDebugLogToFile_clicked(bool checked) {

}

void MainWindow::on_comboBoxDigitalFSKBaudrate_currentIndexChanged(int index) {
  settings->setDigitalFSKBaudrate(index);
}

void MainWindow::on_comboBoxDigitalFSKBitLength_currentIndexChanged(int index) {
  settings->setDigitalFSKBitLength(index);
}

void MainWindow::on_comboBoxDigitalFSKStopbits_currentIndexChanged(int index) {
  settings->setDigitalFSKStopbits(index);
}

void MainWindow::on_comboBoxDigitalFSKParity_currentIndexChanged(int index) {
  settings->setDigitalFSKParity(index);
}
