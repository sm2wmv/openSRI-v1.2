#include "formbanddecodertitle.h"
#include "ui_formbanddecodertitle.h"

FormBandDecoderTitle::FormBandDecoderTitle(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::FormBandDecoderTitle)
{
  ui->setupUi(this);
}

FormBandDecoderTitle::~FormBandDecoderTitle()
{
  delete ui;
}
