#ifndef DIALOGSELECTPORT_H
#define DIALOGSELECTPORT_H

#include <QDialog>

namespace Ui {
class DialogSelectPort;
}

class DialogSelectPort : public QDialog
{
  Q_OBJECT

public:
  explicit DialogSelectPort(QWidget *parent = 0);
  ~DialogSelectPort();
  void refreshPorts();
  QString getPortName();
private slots:
  void on_pushButtonRefresh_clicked();
  void on_pushButtonCancel_clicked();
  void on_pushButtonOK_clicked();
private:
  Ui::DialogSelectPort *ui;
  QString currentPort;
  bool threadActive;
};

#endif // DIALOGSELECTPORT_H
