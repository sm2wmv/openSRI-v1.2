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
  void setIndex(int index);
  int getIndex();
  void setGroup(int group);
  int getGroup();

private:
  Ui::FormBandDecoder *ui;
  int currentGroup;
  int currentIndex;

};

#endif // FORMBANDDECODER_H
