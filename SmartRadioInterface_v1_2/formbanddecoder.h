#ifndef FORMBANDDECODER_H
#define FORMBANDDECODER_H

#include <QWidget>

namespace Ui {
class FormBandDecoder;
}

class FormBandDecoder : public QWidget
{
  Q_OBJECT

public:
  explicit FormBandDecoder(QWidget *parent = 0);
  ~FormBandDecoder();

private:
  Ui::FormBandDecoder *ui;
};

#endif // FORMBANDDECODER_H
