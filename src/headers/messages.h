#ifndef S21_MESSAGES_H
#define S21_MESSAGES_H

#include <QDialog>
#include <QEvent>
#include <QtCharts/QtCharts>
#include "NeuralNetwork.h"

namespace s21 {

namespace Ui {
class Messages;
}

class Messages : public QDialog
{
    Q_OBJECT

public:
  explicit Messages(NeuralNetwork &model, QWidget *parent = nullptr);
  ~Messages();
private slots:
  // void readingFile(std::string str);
  void chartBtnClicked(bool b);
  void breakBtnClicked(bool b);
  void updateBarVal(int i, mStatus stat, int e = 0);
  // void train();
  // void test();
  void modelReady(mStatus status);
  void updateChart(double d);
  void showChart(bool);
  void showDialogMsg(mStatus status, std::string str = "");

  void on_saveChartPushButton_clicked();

private:
  Ui::Messages *ui;
  NeuralNetwork &model_;
  QBarSet *set_;
  QBarSeries *series_;
  QChart *chart_;
  bool ready_;
  bool isChart_;

 private:
  void reject();
  void clearChart();
};


} // namespace s21
#endif // S21_MESSAGES_H
