#include "formbanddecoder.h"
#include "ui_formbanddecoder.h"

FormBandDecoder::FormBandDecoder(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::FormBandDecoder)
{
  ui->setupUi(this);
}

FormBandDecoder::~FormBandDecoder()
{
  delete ui;
}
