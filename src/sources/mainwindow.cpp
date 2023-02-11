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

void MainWindow::signalSlotsConnect(){
  connect(_drawArea, SIGNAL(sendPicture(QImage *)),
          this, SLOT(GetPicture(QImage *)));
  connect(this, SIGNAL(sendChar(const QString &)),
          _ui->char_is, SLOT(setText(const QString &)));
  connect(_ui->layerNumber, SIGNAL(valueChanged(int)),
          _ui->lcdNumber, SLOT(display(int)));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _ui(new Ui::View) {
  _ui->setupUi(this);
  _drawArea = new DrawArea(_ui->frame);
  _drawArea->setGeometry(QRect(2, 2, 280, 280));
  signalSlotsConnect();
}

MainWindow::~MainWindow() {
  delete _ui;
  delete _drawArea;
  // delete convert;
}

void MainWindow::initMlp() {
  QMessageBox dialog;
  dialog.setBaseSize(400, 150);
  dialog.setText("The neural network is not trained.");
  dialog.setInformativeText("Please start the tutorial or select the weight folder");
  dialog.exec();
  // QString fileName = QFileDialog::getOpenFileName(this, tr("OpenFile"),
  //                                                 QDir::homePath()/*, tr("Images (*.png *.jpg)")*/);
  // QString folderName = QFileDialog::getExistingDirectory(this, tr("OpenFile"), QDir::homePath());
  // qInfo() << folderName;
  QString name = "/Users/yarik/MyProjects/MLP/src_data";
  _netConfig = ReadDataNetwork();
  _net.Init(_netConfig);
  _net.ReadWeights(name.toStdString());
  delete [] _netConfig.size;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  _net.ClearLeaks();
  event->accept();
  // } else {
  //   event->ignore();
  // }
}

void MainWindow::GetPicture(QImage *img) {
  qInfo() << "test string";
  Converter conv(img);
  // conv.saveImage();
  std::vector<double> _v = conv.convertDrawImg();
  _net.SetInput(_v);
  // _net.ForwardFeed();
  // std::cout << (char)(_net.ForwardFeed() + 64) << std::endl;
  sendChar("I think it's the letter: " + QString(QChar(_net.ForwardFeed() + 64)));
  // printVector(_v);
  // img.save(QDir::homePath() + "/3.png", "png");
}
}  // namespace s21
