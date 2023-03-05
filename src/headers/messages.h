#ifndef S21_MESSAGES_H
#define S21_MESSAGES_H

#include <QDialog>
#include <QEvent>
#include <QtCharts/QtCharts>
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
  void chartBtnClicked(bool b);
  void breakBtnClicked(bool b);
  void updateBarVal(int i, testTrain stat, int e = 0);
  void train();
  void test();
  void modelReady();
  // void proggressBarFull(int);
  void updateChart(double d);
  void showChart(bool);

private:
  Ui::Messages *ui;
  Network &model_;
  QBarSet *set_;
  QBarSeries *series_;
  QChart *chart_;
  QValueAxis *axisY_;
  bool isChart_;
};


} // namespace s21
#endif // S21_MESSAGES_H
