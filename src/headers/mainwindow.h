#ifndef S21_MAINWINDOW_H
#define S21_MAINWINDOW_H

#include <QMainWindow>

#include "Network.h"
#include "converter.h"
#include "drawarea.h"
#include "statecontroller.h"

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
  void initMlp();

 private:
  void signalSlotsConnect();
 private slots:
  void GetPicture(QImage *);

 signals:
  void sendChar(const QString &);

 private:
  Ui::View *_ui;
  DrawArea *_drawArea;
  Network _net{};
  data_Network _netConfig;
  StateController _state;
  // Converter *convert;
};
}  // namespace s21
#endif  // S21_MAINWINDOW_H
