#ifndef FORMBANDDECODERTITLE_H
#define FORMBANDDECODERTITLE_H

#include <QWidget>

namespace Ui {
class FormBandDecoderTitle;
}

class FormBandDecoderTitle : public QWidget
{
  Q_OBJECT

public:
  explicit FormBandDecoderTitle(QWidget *parent = 0);
  ~FormBandDecoderTitle();

private:
  Ui::FormBandDecoderTitle *ui;
};

#endif // FORMBANDDECODERTITLE_H
