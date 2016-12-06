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
  void setIndex(int index);
  int getIndex();
  void setGroup(int group);
  int getGroup();

private slots:
  void on_pushButtonDelete_clicked();
signals:
  void deleteSegment(QWidget *currWidget);
private:
  Ui::FormBandDecoder *ui;
  int currentGroup;
  int currentIndex;

};

#endif // FORMBANDDECODER_H
