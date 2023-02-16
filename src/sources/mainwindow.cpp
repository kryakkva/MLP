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
  connect(this, SIGNAL(letterIs(const QString &)),
          _ui->letterLabel, SLOT(setText(const QString &)));
  connect(this, SIGNAL(setInputLetter(std::vector<double>)),
          _net, SLOT(SetInput(std::vector<double>)));
  connect(_ui->layersDial, SIGNAL(valueChanged(int)),
          _ui->lcdNumber, SLOT(display(int)));
  connect(_ui->testScrollBar, SIGNAL(valueChanged(int)),
          _drawArea->getConv(), SLOT(intToString(int)));
  connect(_drawArea->getConv(), SIGNAL(sendStr(const QString &)),
          _ui->testPartLabel,SLOT(setText(const QString &)));
  connect(_ui->crossValidationCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(crossValidKgroup(int)));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _ui(new Ui::View), _net(new Network()) {
  _ui->setupUi(this);
  setDrawArea();
  signalSlotsConnect();
}

MainWindow::~MainWindow() {
  delete _drawArea;
  delete _ui;
  delete _net;
  // delete convert;
}

void MainWindow::setDrawArea() {
  _drawArea = new DrawArea(_ui->frame);
  connect(_drawArea, SIGNAL(sendLetter(std::vector<double>)),
          this, SLOT(predictLetter(std::vector<double>)));
  connect(_ui->loadImageButton, SIGNAL(pressed()),
          _drawArea, SLOT(loadImage()));
}

void MainWindow::initMlp() {
  // QDialog dialog(this);
  // dialog.resize(300, 200);
  // QProgressDialog progress("Loading...", "Break", 0, 100);
  // progress.setWindowModality(Qt::WindowModal);
  // progress.setModal(true);
  // for (double i = 0.; i < 9999999999999999999; i += 0.00001) {
  //   progress.setValue(i);
  //   QApplication::processEvents();
  //
  //   if (progress.wasCanceled())
  //     break;
  //   //... copy one file
  // }
  // progress.setValue(9999999999999999999);

  QString folderName;
  QString fileName;
  QDir dir(qApp->applicationDirPath());
  qInfo() << qApp->applicationPid();
  dir.cd("../");
  // qInfo() << "current " << QDir::currentPath();
  // qInfo() << "current " << QDir::currentPath();

  if (QDir(dir.absolutePath() + "/weights").isEmpty()) {
    if (!QDir(dir.absolutePath() + "/weights").exists()) {
      qInfo() << "no dir";
      qInfo() << QDir().mkdir(dir.absolutePath() + "/weights");
    }
    QMessageBox dialog;
    dialog.setIcon(QMessageBox::Warning);
    QPushButton train("Train");
    QPushButton open("Open");
    dialog.addButton(&train, QMessageBox::ActionRole);
    dialog.addButton(&open, QMessageBox::ActionRole);
    // btn->setText("LOAD");
    // dialog.setBaseSize(300, 200);
    dialog.setText("The neural network is not trained.");
    dialog.setInformativeText(
        "Please start the tutorial or select the weight folder");
    // QProgressBar bar(&dialog);
    while (fileName.isEmpty()) {
      switch (dialog.exec()) {
        case 0:break;
        case 1:
          // folderName = QFileDialog::getExistingDirectory(this,tr("OpenFile"),QDir::homePath());
          fileName = QFileDialog::getOpenFileName(this,
                                                  tr("OpenFile"),
                                                  QDir::homePath(),
                                                  tr("Weights (*.mlp)"));
          qInfo() << fileName;
          QFile::copy(fileName, dir.absolutePath() + "/weights/Weights_2.mlp");
      }
      // QDir().mkdir("../MyFolder");
      // QString fileName = QFileDialog::getOpenFileName(this, tr("OpenFile"),
      //                                                 QDir::homePath()/*, tr("Images (*.png *.jpg)")*/);
      // QString name = "/Users/yarik/MyProjects/MLP/src_data";
      // QString name = "../weights";
    }
  }
  _netConfig = ReadDataNetwork();
  _net->Init(_netConfig);
  QString str = dir.absolutePath() + "/weights";
  _net->ReadWeights(str.toStdString());
  // _net.ReadWeights(folderName.toStdString());
  delete [] _netConfig.size;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  _net->ClearLeaks();
  event->accept();
  // } else {
  //   event->ignore();
  // }
}

void MainWindow::predictLetter(std::vector<double> _v) {
  emit setInputLetter(_v);
  letterIs(QString(QChar(_net->ForwardFeed() + 64)));
  // printVector(_v);
  // img.save(QDir::homePath() + "/3.png", "png");
}

void MainWindow::crossValidKgroup(int i) {
  _ui->kGroupsSpinBox->setEnabled(i);
}
}  // namespace s21
