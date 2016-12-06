#include "formbanddecoder.h"
#include "ui_formbanddecoder.h"

FormBandDecoder::FormBandDecoder(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::FormBandDecoder)
{
  ui->setupUi(this);
  currentIndex = 1;
  currentGroup = 1;
}

FormBandDecoder::~FormBandDecoder()
{
  delete ui;
}

void FormBandDecoder::setIndex(int index) {
  currentIndex = index;

  ui->labelIndex->setText("#" + QString::number(index));
}

int FormBandDecoder::getIndex() {
  return(currentIndex);
}

void FormBandDecoder::setGroup(int group) {
  currentGroup = group;

  ui->spinBoxGroupNr->setValue(group);
}

int FormBandDecoder::getGroup() {
  return(currentGroup);
}
