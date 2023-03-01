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
  series_->append(set_);
  chart_->addSeries(series_);
  ui->chartArea->setChart(chart_);
  connect(ui->break_pushButton, SIGNAL(clicked(bool)), this, SLOT(breakBtnClicked(bool)));
  connect(ui->chart_pushButton, SIGNAL(clicked(bool)), this, SLOT(chartBtnClicked(bool)));
  this->setModal(true);
}

Messages::~Messages()
{
    delete ui;
    delete set_;
    delete series_;
    delete chart_;
}

void Messages::readingFile(std::string str) {
  this->resize(600, 200);
  ui->chart_pushButton->setVisible(false);
  // ui->file_name_label->setVisible(true);
  ui->file_name_label->setText(QString::fromStdString(str));
  this->show();
  this->exec();
}

void Messages::train() {
  this->resize(600, 200);
  set_->selectAllBars();
  set_->remove(0, set_->selectedBars().size());
  ui->chart_pushButton->setVisible(true);
  ui->file_name_label->setVisible(false);
  this->show();
  this->exec();
}

void Messages::chartBtnClicked(bool b) {
  // qInfo() << "Chart";
  if (ui->chart_pushButton->text() == "Show chart"){
    ui->chart_pushButton->setText("Hide chart");
    this->resize(600, 600);
  }
  else if (ui->chart_pushButton->text() == "Hide chart"){
    ui->chart_pushButton->setText("Show chart");
    this->resize(600, 200);
  }
}

void Messages::updateChart(double d) {
  // qInfo() << d;
  chart_->removeSeries(series_);
  set_->append(d);
  chart_->addSeries(series_);
}

void Messages::breakBtnClicked(bool b) {
  // qInfo() << "break";
    this->close();
}

void Messages::updateBarVal(int i) {
    ui->progressBar->setValue(i);
    // qInfo() << i;
    // if (i == 100)
    //   QDialog::reject();
}

void Messages::modelReady() {
  QDialog::reject();
}

void Messages::reject() {
  int ret = QMessageBox::question(this,
                                   "APP_NAME",
                                   tr("Are you sure?\n"),
                                   QMessageBox::No | QMessageBox::Yes,
                                   QMessageBox::No);
    if (ret == (int)QMessageBox::Yes) {
      model_.setBreak(true);
      QDialog::reject();
    }
  }
} // namespace s21
