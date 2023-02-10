#include "mainwindow.h"

#include "./ui_view.h"

namespace s21 {
static void printVector(std::vector<double> &_v) {
  std::cout << "vector contains:\n";
  std::vector<double>::iterator it;
  int i = 1;
  for (it = ++_v.begin(); it != _v.end(); ++it, i++) {
    std::cout << *it << " ";
    if (i == 28) {
      std::cout << std::endl;
      i = 0;
    }
  }
  std::cout << std::endl;
}

static data_Network ReadDataNetwork() {
  data_Network data{};
  std::string tmp;
  data.L = 4;
  data.size = new int[4];
  data.size[0] = 784;
  data.size[1] = 150;
  data.size[2] = 150;
  // data.size[3] = 150;
  data.size[3] = 26;
  return data;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::View) {
  ui->setupUi(this);
  drawArea = new DrawArea(ui->frame);
  NW_config = ReadDataNetwork();
  NW.Init(NW_config);
  NW.ReadWeights();
  connect(drawArea, SIGNAL(sendPicture(QImage *)), this,
          SLOT(GetPicture(QImage *)));
  connect(this, SIGNAL(sendChar(const QString &)), ui->char_is, SLOT(setText(const QString &)));
  drawArea->setGeometry(QRect(2, 2, 280, 280));
}

void MainWindow::GetPicture(QImage *img) {
  qInfo() << "test string";
  Converter conv(img);
  // conv.saveImage();
  std::vector<double> _v = conv.convertDrawImg();
  NW.SetInput(_v);
  // NW.ForwardFeed();
  // std::cout << (char)(NW.ForwardFeed() + 64) << std::endl;
  sendChar("I think it's the letter: " + QString(QChar(NW.ForwardFeed() + 64)));
  // printVector(_v);
  // img.save(QDir::homePath() + "/3.png", "png");
}

MainWindow::~MainWindow() {
  delete ui;
  delete drawArea;
  // delete convert;
}
}  // namespace s21
