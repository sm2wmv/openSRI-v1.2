#include "formbanddecoder.h"
#include "ui_formbanddecoder.h"

FormBandDecoder::FormBandDecoder(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::FormBandDecoder)
{
  ui->setupUi(this);
  segmentIndex = 1;
  segmentGroup = 1;
  segmentOutputStatus = 0;

  segmentFreqLow = 1800;
  segmentFreqHigh = 2000;
  segmentName = "160m dipole";
  segmentPTTLock = true;
  segmentRXAntenna = false;
}

FormBandDecoder::~FormBandDecoder() {
  delete ui;
}

void FormBandDecoder::setIndex(quint8 index) {
  segmentIndex = index;
  ui->labelIndex->setText("#" + QString::number(index));
}

quint8 FormBandDecoder::getIndex() {
  return(segmentIndex);
}

void FormBandDecoder::setGroup(quint8 group) {
  segmentGroup = group;

  ui->spinBoxGroupNr->setValue(group);
}

quint8 FormBandDecoder::getGroup() {
  return(segmentGroup);
}

void FormBandDecoder::setPTTLockState(bool state) {
  segmentPTTLock = state;

  ui->checkBoxPTTLock->blockSignals(true);
  ui->checkBoxPTTLock->setChecked(state);
  ui->checkBoxPTTLock->blockSignals(false);
}

bool FormBandDecoder::getPTTLockState() {
  return(segmentPTTLock);
}

void FormBandDecoder::setRXAntennaState(bool state) {
  segmentRXAntenna = state;

  ui->checkBoxRXAntenna->blockSignals(true);
  ui->checkBoxRXAntenna->setChecked(state);
  ui->checkBoxRXAntenna->blockSignals(false);
}

bool FormBandDecoder::getRXAntennaState() {
  return(segmentRXAntenna);
}

void FormBandDecoder::setOutputStatus(quint16 outputs) {
  segmentOutputStatus = outputs;

  ui->checkBoxOutput1->blockSignals(true);
  ui->checkBoxOutput2->blockSignals(true);
  ui->checkBoxOutput3->blockSignals(true);
  ui->checkBoxOutput4->blockSignals(true);
  ui->checkBoxOutput5->blockSignals(true);
  ui->checkBoxOutput6->blockSignals(true);
  ui->checkBoxOutput7->blockSignals(true);
  ui->checkBoxOutput8->blockSignals(true);
  ui->checkBoxOutput9->blockSignals(true);
  ui->checkBoxOutput10->blockSignals(true);

  ui->checkBoxOutput1->setChecked((outputs & (1<<0)));
  ui->checkBoxOutput2->setChecked((outputs & (1<<1)));
  ui->checkBoxOutput3->setChecked((outputs & (1<<2)));
  ui->checkBoxOutput4->setChecked((outputs & (1<<3)));
  ui->checkBoxOutput5->setChecked((outputs & (1<<4)));
  ui->checkBoxOutput6->setChecked((outputs & (1<<5)));
  ui->checkBoxOutput7->setChecked((outputs & (1<<6)));
  ui->checkBoxOutput8->setChecked((outputs & (1<<7)));
  ui->checkBoxOutput9->setChecked((outputs & (1<<8)));
  ui->checkBoxOutput10->setChecked((outputs & (1<<9)));

  ui->checkBoxOutput1->blockSignals(false);
  ui->checkBoxOutput2->blockSignals(false);
  ui->checkBoxOutput3->blockSignals(false);
  ui->checkBoxOutput4->blockSignals(false);
  ui->checkBoxOutput5->blockSignals(false);
  ui->checkBoxOutput6->blockSignals(false);
  ui->checkBoxOutput7->blockSignals(false);
  ui->checkBoxOutput8->blockSignals(false);
  ui->checkBoxOutput9->blockSignals(false);
  ui->checkBoxOutput10->blockSignals(false);
}

quint16 FormBandDecoder::getOutputStatus() {
  return(segmentOutputStatus);
}

quint32 FormBandDecoder::getFreqLow() {
  return(segmentFreqLow);
}

void FormBandDecoder::setFreqLow(quint32 freq) {
  segmentFreqLow = freq;

  ui->lineEditFreqStart->blockSignals(true);
  ui->lineEditFreqStart->setText(QString::number(freq));
  ui->lineEditFreqStart->blockSignals(false);
}

quint32 FormBandDecoder::getFreqHigh() {
  return(segmentFreqHigh);
}

void FormBandDecoder::setFreqHigh(quint32 freq) {
  segmentFreqHigh = freq;

  ui->lineEditFreqStop->blockSignals(true);
  ui->lineEditFreqStop->setText(QString::number(freq));
  ui->lineEditFreqStop->blockSignals(false);
}

QString FormBandDecoder::getName() {
  return(segmentName);
}

void FormBandDecoder::setName(QString name) {
  segmentName = name;

  ui->lineEditName->blockSignals(true);
  ui->lineEditName->setText(name);
  ui->lineEditName->blockSignals(false);
}

void FormBandDecoder::on_pushButtonDelete_clicked() {
    emit deleteSegment(this);
}


void FormBandDecoder::on_checkBoxOutput1_clicked(bool checked) {
    quint16 currStatus = getOutputStatus();

    if (checked)
      setOutputStatus(currStatus | (1<<0));
    else
      setOutputStatus(currStatus & ~(1<<0));
}

void FormBandDecoder::on_checkBoxOutput2_clicked(bool checked) {
  quint16 currStatus = getOutputStatus();

  if (checked)
    setOutputStatus(currStatus | (1<<1));
  else
    setOutputStatus(currStatus & ~(1<<1));
}

void FormBandDecoder::on_checkBoxOutput3_clicked(bool checked) {
  quint16 currStatus = getOutputStatus();

  if (checked)
    setOutputStatus(currStatus | (1<<2));
  else
    setOutputStatus(currStatus & ~(1<<2));
}

void FormBandDecoder::on_checkBoxOutput4_clicked(bool checked) {
  quint16 currStatus = getOutputStatus();

  if (checked)
    setOutputStatus(currStatus | (1<<3));
  else
    setOutputStatus(currStatus & ~(1<<3));
}

void FormBandDecoder::on_checkBoxOutput5_clicked(bool checked) {
  quint16 currStatus = getOutputStatus();

  if (checked)
    setOutputStatus(currStatus | (1<<4));
  else
    setOutputStatus(currStatus & ~(1<<4));
}

void FormBandDecoder::on_checkBoxOutput6_clicked(bool checked) {
  quint16 currStatus = getOutputStatus();

  if (checked)
    setOutputStatus(currStatus | (1<<5));
  else
    setOutputStatus(currStatus & ~(1<<5));
}

void FormBandDecoder::on_checkBoxOutput7_clicked(bool checked) {
  quint16 currStatus = getOutputStatus();

  if (checked)
    setOutputStatus(currStatus | (1<<6));
  else
    setOutputStatus(currStatus & ~(1<<6));
}

void FormBandDecoder::on_checkBoxOutput8_clicked(bool checked) {
  quint16 currStatus = getOutputStatus();

  if (checked)
    setOutputStatus(currStatus | (1<<7));
  else
    setOutputStatus(currStatus & ~(1<<7));
}

void FormBandDecoder::on_checkBoxOutput9_clicked(bool checked) {
  quint16 currStatus = getOutputStatus();

  if (checked)
    setOutputStatus(currStatus | (1<<8));
  else
    setOutputStatus(currStatus & ~(1<<8));
}

void FormBandDecoder::on_checkBoxOutput10_clicked(bool checked) {
  quint16 currStatus = getOutputStatus();

  if (checked)
    setOutputStatus(currStatus | (1<<9));
  else
    setOutputStatus(currStatus & ~(1<<9));
}
