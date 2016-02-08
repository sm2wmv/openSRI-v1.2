#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include "misc.h"
#include "commclass.h"
#include "dialogselectport.h"
#include "commands.h"
#include "settings.h"
#include "status.h"
#include "displayclass.h"

//! The time between each poll of the USB port in ms
#define TIMER_POLL_STATUS_INTERVAL  10

//! The interval between each PING is sent from both computer and SRI interface (in ms)
#define SRI_PING_INTERVAL     1000

//! SRI interface timeout limit (ms divided by SRI_PING_INTERVAL)
#define SRI_PING_TIMEOUT_LIMIT  5000/SRI_PING_INTERVAL

#define QLABEL_TX_STYLESHEET        "QLabel { background-color : red; border-style: outset;border-width: 1px;border-radius: 1px;border-color: black;}"
#define QLABEL_RX_STYLESHEET        "QLabel { background-color : green; border-style: outset;border-width: 1px;border-radius: 1px;border-color: black;}"
#define QLABEL_RX_MUTE_STYLESHEET   "QLabel { background-color : black; border-style: outset;border-width: 1px;border-radius: 1px;border-color: black;}"
#define QLABEL_TXRXNONE_STYLESHEET  ""

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  DialogSelectPort *dlgSelectPort;
  CommClass *getSerialPtr();
  void updateUIfromSettings();
  void updateUIfromStatus();
  void showMessageBox(QString title, QString text, enum QMessageBox::Icon icon);
private:
  Ui::MainWindow *ui;
  CommClass *serialPort;
  QString currentSRIPort;
  void parseCommData(StructMessage message);
  QTimer *timerPollStatus;
  QTimer *timerPingPongEvent;
  Settings *settings;
  Status *status;
  quint16 lastPongCounter;
  void updateBacklightColor(QColor color);
  DisplayClass display;
  quint8 rowCount;
private slots:
  void actionPTTInputClicked();
  void actionPTTInputInvertClicked();
  void actionSerSRIPortTriggered();
  void on_actionConnect_triggered();
  void on_actionDisconnect_triggered();
  void timerPollStatusUpdate();
  void timerPingPongEventUpdate();
  void on_actionQuit_triggered();
  void on_actionTestButton_triggered();
  void on_pushButtonCATSettingsApply_clicked();
  void on_actionGetVersion_triggered();
  void on_comboBoxPTTOutputPhone_currentIndexChanged(int index);
  void on_comboBoxPTTOutputCW_currentIndexChanged(int index);
  void on_comboBoxPTTOutputDigital_currentIndexChanged(int index);
  void on_checkBoxPTTOutputInhibit_toggled(bool checked);
  void on_checkBoxPTTOutputAmpflier_toggled(bool checked);
  void on_spinBoxPTTRadioPreDelay_valueChanged(int arg1);
  void on_spinBoxPTTRadioPostDelay_valueChanged(int arg1);
  void on_spinBoxPTTAmplifierPreDelay_valueChanged(int arg1);
  void on_spinBoxPTTAmplifierPostDelay_valueChanged(int arg1);
  void on_spinBoxPTTInhibitPreDelay_valueChanged(int arg1);
  void on_spinBoxPTTInhibitPostDelay_valueChanged(int arg1);
  void on_spinBoxPTTAntennaPreDelay_valueChanged(int arg1);
  void on_spinBoxPTTAntennaPostDelay_valueChanged(int arg1);
  void on_actionSaveToEEPROM_triggered();

  void disconnectSerialPort();
  void msgBoxClosed(QAbstractButton* button);
  void on_comboBoxAudioRoutingScenario_currentIndexChanged(int index);
  void on_comboBoxAudioRoutingLineInCW_currentIndexChanged(int index);
  void on_comboBoxAudioRoutingLineInPhone_currentIndexChanged(int index);
  void on_comboBoxAudioRoutingLineInDigital_currentIndexChanged(int index);
  void on_comboBoxAudioRoutingMicPath_currentIndexChanged(int index);
  void on_checkBoxAudioRoutingMuteMic_clicked(bool checked);
  void on_checkBoxAudioRoutingPreamp_clicked(bool checked);
  void on_checkBoxPTTOutputInhibitInverted_toggled(bool checked);

  void actionCWInputSourceToggled();
  void on_pushButtonDisplayBacklightApply_clicked();
  void on_comboBoxWinkeyMode_currentIndexChanged(int index);
  void on_checkBoxWinkeySwapPaddles_clicked(bool checked);
  void on_checkBoxWinkeyAutoSpace_clicked(bool checked);
  void on_checkBoxWinkeyCTSpace_clicked(bool checked);
  void on_spinBoxWinkeyPotLowSpeed_valueChanged(int arg1);
  void on_spinBoxWinkeyPotHighSpeed_valueChanged(int arg1);
  void on_spinBoxWinkeyFarnsworth_valueChanged(int arg1);
  void on_spinBoxWinkeyWeight_valueChanged(int arg1);
  void on_spinBoxWinkeyDihDahRatio_valueChanged(int arg1);
  void on_spinBoxWinkey1stExtension_valueChanged(int arg1);
  void on_spinBoxWinkeyKeyCompensation_valueChanged(int arg1);
  void on_groupBoxWinkeyPaddleMemory_clicked(bool checked);
  void on_radioButtonWinkeyPaddleMemoryPriorityDit_clicked(bool checked);
  void on_radioButtonWinkeyPaddleMemoryPriorityDah_clicked(bool checked);
  void on_spinBoxWinkeyPaddleMemorySetPoint_valueChanged(int arg1);
  void on_comboBoxWinkeyHangTime_currentIndexChanged(int index);

  void closeEvent (QCloseEvent *event);
  void killTheProgram();
  void on_checkBoxWinkeySerialEchoback_clicked(bool checked);
  void on_checkBoxWinkeyPaddleEchoback_clicked(bool checked);
  void on_spinBoxWinkeyDefaultSpeed_valueChanged(int arg1);
  void on_pushButtonCWMessageSave1_clicked();
  void on_pushButtonCWMessageSave2_clicked();
  void on_pushButtonCWMessageSave3_clicked();
  void on_pushButtonCWMessageSave4_clicked();
  void on_pushButtonCWMessageSave5_clicked();
  void on_pushButtonCWMessageSave6_clicked();
  void on_pushButtonCWMessageSave7_clicked();
  void on_pushButtonCWMessageSave8_clicked();
  void on_pushButtonCWMessagesSaveAll_clicked();
  void on_pushButtonCWMessageSend1_clicked();
  void on_pushButtonCWMessageSend2_clicked();
  void on_pushButtonCWMessageSend3_clicked();
  void on_pushButtonCWMessageSend4_clicked();
  void on_pushButtonCWMessageSend5_clicked();
  void on_pushButtonCWMessageSend6_clicked();
  void on_pushButtonCWMessageSend7_clicked();
  void on_pushButtonCWMessageSend8_clicked();
  void on_pushButtonCWMessagesStop_clicked();
  void on_pushButtonOtherBacklightColorDialog_clicked();
  void on_spinBoxDisplayBacklightRed_valueChanged(int arg1);
  void on_spinBoxDisplayBacklightGreen_valueChanged(int arg1);
  void on_spinBoxDisplayBacklightBlue_valueChanged(int arg1);
  void on_comboBoxDisplayTextCWLine1RX_currentIndexChanged(int index);
  void on_comboBoxDisplayTextCWLine2RX_currentIndexChanged(int index);
  void on_comboBoxDisplayTextCWLine1TX_currentIndexChanged(int index);
  void on_comboBoxDisplayTextCWLine2TX_currentIndexChanged(int index);
  void on_comboBoxDisplayTextPhoneLine1RX_currentIndexChanged(int index);
  void on_comboBoxDisplayTextPhoneLine2RX_currentIndexChanged(int index);
  void on_comboBoxDisplayTextPhoneLine1TX_currentIndexChanged(int index);
  void on_comboBoxDisplayTextPhoneLine2TX_currentIndexChanged(int index);
  void on_comboBoxDisplayTextDigitalLine1RX_currentIndexChanged(int index);
  void on_comboBoxDisplayTextDigitalLine2RX_currentIndexChanged(int index);
  void on_comboBoxDisplayTextDigitalLine1TX_currentIndexChanged(int index);
  void on_comboBoxDisplayTextDigitalLine2TX_currentIndexChanged(int index);
  void on_lineEditDisplayTextCallsign_textChanged(const QString &arg1);
  void on_spinBoxWinkeyLeadTime_valueChanged(int arg1);
  void on_spinBoxWinkeyTailTime_valueChanged(int arg1);
  void on_checkBoxAudioRoutingExtPreamp_clicked(bool checked);
  void checkBoxDebugClicked();
  void on_pushButtonDebugClearWindow_clicked();
  void on_checkBoxDebugLogToFile_clicked(bool checked);
};

#endif // MAINWINDOW_H
