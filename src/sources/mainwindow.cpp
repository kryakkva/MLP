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

void MainWindow::SignalSlotsConnect(){
  connect(this, SIGNAL(LetterIs(const QString &)), view_->letterLabel, SLOT(setText(const QString &)));
  connect(view_->layersDial, SIGNAL(valueChanged(int)),view_->lcdNumber, SLOT(display(int)));
  connect(view_->testScrollBar, SIGNAL(valueChanged(int)),draw_area_->getConv(), SLOT(intToString(int)));
  connect(draw_area_->getConv(), SIGNAL(sendStr(const QString &)),view_->testPartLabel, SLOT(setText(const QString &)));
  connect(view_->crossValidationRadio, SIGNAL(clicked(bool)),view_->kGroupsSpinBox, SLOT(setEnabled(bool)));
  connect(model_, SIGNAL(readMessage(std::string)), messages_, SLOT(readingFile(std::string)));
  connect(this, SIGNAL(readFile(std::string, testTrain)), model_, SLOT(ReadData(std::string, testTrain)));
  connect(model_, SIGNAL(updateBar(int, testTrain, int)), messages_, SLOT(updateBarVal(int, testTrain, int)));
  connect(view_->trainButton, SIGNAL(clicked(bool)), model_, SLOT(NetworkTrain(bool)));
  connect(view_->testButton, SIGNAL(clicked(bool)), model_, SLOT(NetworkTest(bool)));
  connect(model_, SIGNAL(actTrainBtn(bool)), view_->trainButton, SLOT(setEnabled(bool)));
  connect(model_, SIGNAL(actTestBtn(bool)), view_->testButton, SLOT(setEnabled(bool)));
  connect(model_, SIGNAL(actChartBtn(bool)), view_->chartButton, SLOT(setEnabled(bool)));
  connect(model_, SIGNAL(trainMessage()), messages_, SLOT(train()));
  connect(model_, SIGNAL(testMessage()), messages_, SLOT(test()));
  connect(model_, SIGNAL(iAmReady()), messages_, SLOT(modelReady()));
  connect(model_, SIGNAL(updateChart(double)), messages_, SLOT(updateChart(double)));
  connect(view_->testScrollBar, SIGNAL(valueChanged(int)), model_, SLOT(SetTestPart(int)));
  connect(view_->spinBox, SIGNAL(valueChanged(int)), model_, SLOT(setEpoch(int)));
  connect(view_->loadTestButton, SIGNAL(clicked(bool)), this, SLOT(openTestFile(bool)));
  connect(view_->LoadTrainButton, SIGNAL(clicked(bool)), this, SLOT(openTrainFile(bool)));
  connect(view_->chartButton, SIGNAL(clicked(bool)), messages_, SLOT(showChart(bool)));
  connect(model_, SIGNAL(wrongFile()), this, SLOT(wrongFileError()));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), view_(new Ui::View), model_(new Network()), messages_(new Messages(*model_ )) {
  view_->setupUi(this);
  SetDrawArea();
  SignalSlotsConnect();
  thr_model_ = new QThread(this);
  model_->moveToThread(thr_model_);
  thr_model_->start(QThread::NormalPriority);
}

MainWindow::~MainWindow() {
  thr_model_->quit();
  thr_model_->deleteLater();
  delete view_;
  delete draw_area_;
  delete messages_;
  delete model_;
}

void MainWindow::SetDrawArea() {
  draw_area_ = new DrawArea(view_->frame);
  connect(draw_area_, SIGNAL(SendLetter(std::vector<double>)),
          this, SLOT(PredictLetter(std::vector<double>)));
  connect(view_->loadImageButton, SIGNAL(pressed()),
          draw_area_, SLOT(LoadImage()));
}

void MainWindow::PredictLetter(std::vector<double> v) {
  model_->SetInput(v, 0);
  LetterIs(QString(QChar(model_->ForwardFeed() + 64)));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  event->accept();
}
/*
void MainWindow::on_load_btn_clicked()
{
  QString file_name = QFileDialog::getOpenFileName(this,
                                                             tr("OpenFile"),
                                                             QDir::homePath(),
                                                             tr("Train (*.csv)"));
  if (!file_name.isEmpty())
    emit readFile(file_name.toStdString(), train_);
  // emit readMessage();
  // msg.exec();
  // model_->ReadData(file_name.toStdString());
}
*/

void MainWindow::openTrainFile(bool b) {
  // std::thread th([&]() {});
  QString file_name = QFileDialog::getOpenFileName(this, "OpenFile", QDir::homePath(), "Train_file (*.csv)");
  if (!file_name.isEmpty())
      emit readFile(file_name.toStdString(), train_);
}

void MainWindow::openTestFile(bool b) {
  // std::thread th([&]() {});
  QString file_name = QFileDialog::getOpenFileName(this, "OpenFile", QDir::homePath(), "Test_file (*.csv)");
  if (!file_name.isEmpty())
      emit readFile(file_name.toStdString(), test_);
}


void MainWindow::on_saveWeightsButton_clicked()
{
  QString file_name = QFileDialog::getSaveFileName(this, "Open weights file", QDir::homePath(), "Weights_file (*.mlp)");
  if (!file_name.isEmpty())
    model_->SaveWeights_M(file_name.toStdString());
}


void MainWindow::on_LoadWeightsButton_clicked()
{
  QString file_name = QFileDialog::getOpenFileName(this, "OpenFile", QDir::homePath(), "Weights_file (*.mlp)");
  if (!file_name.isEmpty())
    model_->ReadWeights_M(file_name.toStdString());
}


// void MainWindow::on_spinBox_valueChanged(int arg1)
// {
//   model_->setEpoch(arg1);
// }


void MainWindow::on_layersDial_sliderReleased()
{
  model_->reInitNet(view_->lcdNumber->intValue());
}

void MainWindow::wrongFileError() {
  QMessageBox::information(this, "you open wrong file", "please open right file");
}

/*
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
      //                                                 QDir::homePath(), tr("Images (*.png *.jpg)"));
      // QString name = "/Users/yarik/MyProjects/MLP/src_data";
      // QString name = "../weights";
    }
  }
  _netConfig = ReadDataNetwork();
  model_->Init(_netConfig);
  QString str = dir.absolutePath() + "/weights";
  model_->ReadWeights(str.toStdString());
  // model_.ReadWeights(folderName.toStdString());
  delete [] _netConfig.size;
}
*/
}  // namespace s21
