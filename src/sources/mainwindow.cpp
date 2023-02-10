#include "mainwindow.h"

#include "./ui_view.h"

namespace s21 {
static void printVector(std::vector<double> &_v) {
  std::cout << "vector contains:\n";
  std::vector<double>::iterator it;
  int i = 1;
  for (it = _v.begin(); it != _v.end(); ++it, i++) {
    std::cout << *it << " ";
    if (i == 28) {
      std::cout << std::endl;
      i = 0;
    }
  }
  std::cout << std::endl;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::View) {
  ui->setupUi(this);
  drawArea = new DrawArea(ui->frame);
  connect(drawArea, SIGNAL(sendPicture(QImage *)), this,
          SLOT(GetPicture(QImage *)));
  drawArea->setGeometry(QRect(2, 2, 280, 280));
}

void MainWindow::GetPicture(QImage *img) {
  qInfo() << "test string";
  Converter conv(img);
  // conv.saveImage();
  std::vector<double> _v = conv.convertDrawImg();
  printVector(_v);
  // img.save(QDir::homePath() + "/3.png", "png");
}

MainWindow::~MainWindow() {
  delete ui;
  delete drawArea;
  // delete convert;
}
}  // namespace s21
