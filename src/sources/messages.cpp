#include "messages.h"
#include "ui_messages.h"
#include <unistd.h>
namespace s21 {

Messages::Messages(Network &model, QWidget *parent) :
    QDialog(parent), model_(model), ui(new Ui::Messages), isChart_(false) {
  ui->setupUi(this);
  chart_ = new QChart();
  set_ = new QBarSet("Epoch Error Percentage");
    series_ = new QBarSeries();
//    axisY_ = new QValueAxis;
//    qInfo() << ui << ui->chartArea << chart_ << set_ <<  series_ ;
//    chart_->addAxis(axisY_, Qt::AlignLeft);
  // qInfo() << series_->isLabelsVisible();
  series_->setLabelsVisible();
  // qInfo() << series_->isLabelsVisible();
  series_->setLabelsPrecision(4);
  series_->setLabelsPosition(QAbstractBarSeries::LabelsCenter);
  series_->setLabelsAngle(-90);
  series_->setLabelsFormat("@value %");
  series_->append(set_);
  chart_->legend()->setVisible(false);
  chart_->setBackgroundVisible(false);
    chart_->addSeries(series_);
//    chart_->createDefaultAxes();
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
    chart_->createDefaultAxes();
    connect(ui->break_pushButton, SIGNAL(clicked(bool)), this, SLOT(breakBtnClicked(bool)));
  connect(ui->chart_pushButton, SIGNAL(clicked(bool)), this, SLOT(chartBtnClicked(bool)));
}

Messages::~Messages()
{
//    delete axisY_;
//    delete set_;
//    delete series_;
//    delete chart_;
//ui->chartArea->deleteLater();
    delete ui;
}

void Messages::readingFile(std::string str) {
    ready_ = false;
    ui->break_pushButton->setText("Break");
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

void Messages::clearChart(){
    set_->selectAllBars();
    if (!set_->selectedBars().empty()){
        qInfo() << "selected bars not empty" << set_->selectedBars().size();
        set_->remove(0, set_->selectedBars().size());
    }
    if (!chart_->series().empty()){
        qInfo() << "series not empty" << chart_->series().size();
        chart_->removeSeries(series_);
    }
//    if (!chart_->axes().empty()){
////        int a = chart_->axes().size();
//        qInfo() << "axes not empty" << chart_->axes().size();
//        while (chart_->axes().size()){
//            qInfo() << chart_->axes()[0];
//            chart_->removeAxis(chart_->axes()[0]);
//        }
//        qInfo() << "axes not empty" << chart_->axes().size();
//    }
//    chart_->createDefaultAxes();
    qInfo() << "Cleared";
}

void Messages::train() {
    ui->break_pushButton->setText("Break");
    ready_ = false;
  this->setWindowTitle("Training");
  this->setFixedSize(600,200);
  clearChart();
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
    ui->break_pushButton->setText("Break");
    ready_ = false;
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
    set_->append(d);
    set_->selectAllBars();
    if (set_->selectedBars().size() == 1) {
      chart_->addSeries(series_);
  }
    chart_->createDefaultAxes();
//  chart_->update();
//  ui->chartArea->update();
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
    ready_ = true;
    ui->break_pushButton->setText("OK");
}

void Messages::reject() {
  if (isChart_ || ready_){
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
        if (!ready_)
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
