#ifndef S21_MAINWINDOW_H
#define S21_MAINWINDOW_H

#include <QMainWindow>

#include "Network.h"
#include "drawarea.h"
// #include "StateMessage.h"
#include "messages.h"

namespace s21 {

namespace Ui {
class View;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void closeEvent(QCloseEvent *event);

 private:
  void SignalSlotsConnect();
  void SetDrawArea();

 private slots:
  void PredictLetter(std::vector<double>);
  void openTrainFile();
  void openTestFile();
  // void on_load_btn_clicked();

  void on_spinBox_valueChanged(int arg1);
  void on_layersDial_sliderReleased();
  void on_saveWeightsButton_clicked();
  void on_LoadWeightsButton_clicked();

signals:
  void LetterIs(const QString &);

  void readFile(std::string, testTrain);
  // void setInputLetter(std::vector<double>);

 private:
  Ui::View *view_;
  DrawArea *draw_area_;
  Network *model_;
  Messages *messages_;

  QThread *thr_model_ = nullptr;
  // QThread *thr_state_ = nullptr;

};
}  // namespace s21
#endif  // S21_MAINWINDOW_H
