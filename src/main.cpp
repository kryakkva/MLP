#include "./headers/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    s21::MainWindow w;
    w.show();
    w.initMlp();
    return a.exec();
}
