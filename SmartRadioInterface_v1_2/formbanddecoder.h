#ifndef FORMBANDDECODER_H
#define FORMBANDDECODER_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class FormBandDecoder;
}

class FormBandDecoder : public QWidget
{
  Q_OBJECT
public:

  explicit FormBandDecoder(QWidget *parent = 0);
  ~FormBandDecoder();
  void setIndex(quint8 index);
  quint8 getIndex();
  void setGroup(quint8 group);
  quint8 getGroup();
  void setOutputStatus(quint16 outputs);
  quint16 getOutputStatus();
  quint32 getFreqLow();
  void setFreqLow(quint32 freq);
  quint32 getFreqHigh();
  void setFreqHigh(quint32 freq);
  QString getName();
  void setName(QString name);
  void setPTTLockState(bool state);
  bool getPTTLockState();
  void setRXAntennaState(bool state);
  bool getRXAntennaState();
private slots:
  void on_pushButtonDelete_clicked();
  void on_checkBoxOutput1_clicked(bool checked);

  void on_checkBoxOutput2_clicked(bool checked);

  void on_checkBoxOutput3_clicked(bool checked);

  void on_checkBoxOutput4_clicked(bool checked);

  void on_checkBoxOutput5_clicked(bool checked);

  void on_checkBoxOutput6_clicked(bool checked);

  void on_checkBoxOutput7_clicked(bool checked);

  void on_checkBoxOutput8_clicked(bool checked);

  void on_checkBoxOutput9_clicked(bool checked);

  void on_checkBoxOutput10_clicked(bool checked);

signals:
  void deleteSegment(QWidget *currWidget);
private:
  Ui::FormBandDecoder *ui;
  quint8 segmentGroup;
  quint8 segmentIndex;
  quint32 segmentFreqHigh;
  quint32 segmentFreqLow;
  quint16 segmentOutputStatus;
  QString segmentName;
  bool segmentPTTLock;
  bool segmentRXAntenna;
};

#endif // FORMBANDDECODER_H
