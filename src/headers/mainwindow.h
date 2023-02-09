#ifndef S21_MAINWINDOW_H
#define S21_MAINWINDOW_H

#include <QMainWindow>

#include "Network.h"
#include "drawarea.h"

namespace s21 {

namespace Ui { class View; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
 void GetPicture(QImage);

private:
    Ui::View *ui;
    DrawArea *drawArea;
};

} // namespace s21
#endif // S21_MAINWINDOW_H
