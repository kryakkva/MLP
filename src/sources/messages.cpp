#include "messages.h"
#include "ui_messages.h"
#include <unistd.h>
namespace s21 {

Messages::Messages(Network &model, QWidget *parent) :
    QDialog(parent), model_(model), ui(new Ui::Messages)
{
  ui->setupUi(this);
  chart_ = new QChart();
  set_ = new QBarSet("ERROR");
  series_ = new QBarSeries();
  axisY_ = new QValueAxis;
  chart_->addAxis(axisY_, Qt::AlignLeft);
  // qInfo() << series_->isLabelsVisible();
  series_->setLabelsVisible();
  // qInfo() << series_->isLabelsVisible();
  series_->setLabelsPrecision(4);
  series_->setLabelsPosition(QAbstractBarSeries::LabelsCenter);
  series_->setLabelsAngle(-90);
  series_->setLabelsFormat("@value %");
  series_->append(set_);
  chart_->addSeries(series_);
  // QValueAxis *axisX = new QValueAxis();
  // axisX->setRange(0,model_.getEpoch());
  // axisX->setTickCount(1);
  // chart_->addAxis(axisX, Qt::AlignBottom);
  // series_->attachAxis(axisX);
  // QValueAxis *axisY = new QValueAxis();
  // axisY->setRange(0,100);
  // chart_->addAxis(axisY, Qt::AlignLeft);
  // series_->attachAxis(axisY);
  // chart_->createDefaultAxes();
  // chart_->legend()->setVisible(true);
  // chart_->legend()->setAlignment(Qt::AlignBottom);

  ui->chartArea->setChart(chart_);
  connect(ui->break_pushButton, SIGNAL(clicked(bool)), this, SLOT(breakBtnClicked(bool)));
  connect(ui->chart_pushButton, SIGNAL(clicked(bool)), this, SLOT(chartBtnClicked(bool)));
}

Messages::~Messages()
{
    delete ui;
    delete axisY_;
    delete set_;
    delete series_;
    delete chart_;
}

void Messages::readingFile(std::string str) {
  this->setWindowTitle("Reading");
  this->setFixedSize(600,200);
  ui->file_name_label->setVisible(true);
  ui->label->setVisible(true);
  ui->progressBar->setVisible(true);
  ui->break_pushButton->setVisible(true);
  ui->chart_pushButton->setVisible(false);
  ui->chartArea->setVisible(false);
  ui->label->setText("Reading file: ");
  ui->file_name_label->setText(QString::fromStdString(str));
  // this->show();
  this->exec();
}

void Messages::train() {
  this->setWindowTitle("Training");
  this->setFixedSize(600,200);
  set_->selectAllBars();
  set_->remove(0, set_->selectedBars().size());
  ui->chart_pushButton->setText("Show chart");
  ui->label->setText("Training...");
  ui->file_name_label->setVisible(true);
  ui->label->setVisible(true);
  ui->progressBar->setVisible(true);
  ui->break_pushButton->setVisible(true);
  ui->chart_pushButton->setVisible(true);
  ui->chartArea->setVisible(true);
  ui->chartArea->setGeometry(20,190,560,390);
  // this->show();
  this->exec();
}

void Messages::test() {
  this->setWindowTitle("Testing");
  this->setFixedSize(600,200);
  ui->label->setText("Testing...");
  ui->file_name_label->setVisible(false);
  ui->label->setVisible(true);
  ui->progressBar->setVisible(true);
  ui->break_pushButton->setVisible(true);
  ui->chart_pushButton->setVisible(false);
  ui->chartArea->setVisible(false);
  // this->show();
  this->exec();
}

void Messages::chartBtnClicked(bool b) {
  if (ui->chart_pushButton->text() == "Show chart"){
    ui->chart_pushButton->setText("Hide chart");
    this->setFixedSize(600,600);
  }
  else if (ui->chart_pushButton->text() == "Hide chart"){
    ui->chart_pushButton->setText("Show chart");
    this->setFixedSize(600,200);
  }
}

void Messages::updateChart(double d) {
  // qInfo() << d;
  // chart_->createDefaultAxes();
  set_->selectAllBars();
  if (set_->selectedBars().size() == 0) {
    axisY_->setMax(d);
  }
  chart_->removeSeries(series_);
  set_->append(d);
  chart_->addSeries(series_);
}

void Messages::breakBtnClicked(bool b) {
  // qInfo() << "break";
    this->close();
}

void Messages::updateBarVal(int i, testTrain stat, int e) {
    ui->progressBar->setValue(i);
    // qInfo() << (QString::number(e) + " from " + QString::number(model_.getEpoch()) + " epoch");
    if (stat == train_)
      ui->file_name_label->setText(QString::number(e) + " from " + QString::number(model_.getEpoch()) + " epoch");
    // qInfo() << i;
    // if (i == 100)
    //   QDialog::reject();
}

void Messages::modelReady() {
  QDialog::reject();
}

void Messages::reject() {
  if (isChart_){
    isChart_ = false;
    QDialog::reject();
  }
  else {
    int ret = QMessageBox::question(this,
                                    "APP_NAME",
                                    tr("Are you sure?\n"),
                                    QMessageBox::No | QMessageBox::Yes,
                                    QMessageBox::No);
    if (ret == (int) QMessageBox::Yes) {
      model_.setBreak(true);
      QDialog::reject();
    }
  }
  }

void Messages::showChart(bool) {
  this->setWindowTitle("Error change");
  isChart_ = true;
  this->setFixedSize(600,600);
  ui->chart_pushButton->setVisible(false);
  ui->file_name_label->setVisible(false);
  ui->label->setVisible(false);
  ui->progressBar->setVisible(false);
  ui->break_pushButton->setVisible(false);
  ui->chartArea->setVisible(true);
  ui->chartArea->setGeometry(0,0,600,600);
  this->exec();
}
} // namespace s21
