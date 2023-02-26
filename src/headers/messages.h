#ifndef S21_MESSAGES_H
#define S21_MESSAGES_H

#include <QDialog>
#include <QEvent>
#include "Network.h"

namespace s21 {

namespace Ui {
class Messages;
}

class Messages : public QDialog
{
    Q_OBJECT

public:
    explicit Messages(Network &model, QWidget *parent = nullptr);
    ~Messages();
    void reject();
private slots:
  void readingFile(std::string str);
  void okBtnClicked(bool b);
  void breakBtnClicked(bool b);
  void updateBarVal(int i);
  void train();
  void modelReady();
  // void proggressBarFull(int);

private:
    Ui::Messages *ui;
    Network &model_;
};


} // namespace s21
#endif // S21_MESSAGES_H
