#include "messages.h"

#include "ui_messages.h"
namespace s21 {

Messages::Messages(NeuralNetwork &model, QWidget *parent)
    : QDialog(parent), model_(model), ui(new Ui::Messages), isChart_(false) {
  ui->setupUi(this);
  chart_ = new QChart();
  set_ = new QBarSet("Epoch Error Percentage");
  series_ = new QBarSeries();
  series_->setLabelsVisible();
  series_->setLabelsPrecision(4);
  series_->setLabelsPosition(QAbstractBarSeries::LabelsCenter);
  series_->setLabelsAngle(-90);
  series_->setLabelsFormat("@value %");
  series_->append(set_);
  chart_->legend()->setVisible(false);
  chart_->setBackgroundVisible(false);
  chart_->addSeries(series_);
  ui->chartArea->setChart(chart_);
  chart_->createDefaultAxes();
  connect(ui->break_pushButton, SIGNAL(clicked(bool)), this,
          SLOT(breakBtnClicked(bool)));
  connect(ui->chart_pushButton, SIGNAL(clicked(bool)), this,
          SLOT(chartBtnClicked(bool)));
}

Messages::~Messages() {
  //    delete axisY_;
  //    delete set_;
  //    delete series_;
  //    delete chart_;
  // ui->chartArea->deleteLater();
  delete ui;
}

void Messages::showDialogMsg(mStatus status, std::string str) {
  ready_ = false;
  ui->progressBar->setValue(0);
  ui->break_pushButton->setText("Break");
  ui->label->setVisible(true);
  ui->progressBar->setVisible(true);
  ui->saveChartPushButton->setVisible(false);
  this->setFixedSize(600, 200);
  switch (status) {
    case test_:
      this->setWindowTitle("Testing");
      ui->label->setText("Testing...");
      ui->break_pushButton->setVisible(true);
      ui->file_name_label->setVisible(false);
      ui->chart_pushButton->setVisible(false);
      ui->chartArea->setVisible(false);
      break;
    case train_:
      this->setWindowTitle("Training");
      ui->label->setText("Training...");
      ui->chart_pushButton->setText("Show chart");
      ui->file_name_label->setVisible(true);
      ui->break_pushButton->setVisible(true);
      ui->chart_pushButton->setVisible(true);
      ui->chartArea->setVisible(true);
      ui->chartArea->setGeometry(20, 190, 560, 390);
      clearChart();
      break;
    case read_:
      this->setWindowTitle("Reading");
      ui->label->setText("Reading file: ");
      ui->file_name_label->setVisible(true);
      ui->break_pushButton->setVisible(true);
      ui->chartArea->setVisible(false);
      ui->chart_pushButton->setVisible(false);
      ui->file_name_label->setText(QString::fromStdString(str));
      break;
    default:
      break;
  }
  this->exec();
}

void Messages::clearChart() {
  set_->selectAllBars();
  if (!set_->selectedBars().empty())
    set_->remove(0, set_->selectedBars().size());
  if (!chart_->series().empty()) chart_->removeSeries(series_);
}

void Messages::chartBtnClicked(bool b) {
  if (ui->chart_pushButton->text() == "Show chart") {
    ui->chart_pushButton->setText("Hide chart");
    this->setFixedSize(600, 600);
  } else if (ui->chart_pushButton->text() == "Hide chart") {
    ui->chart_pushButton->setText("Show chart");
    this->setFixedSize(600, 200);
  }
}

void Messages::updateChart(double d) {
  set_->append(d);
  set_->selectAllBars();
  if (set_->selectedBars().size() == 1) {
    chart_->addSeries(series_);
  }
  set_->deselectAllBars();
  chart_->createDefaultAxes();
}

void Messages::breakBtnClicked(bool b) { this->close(); }

void Messages::updateBarVal(int i, mStatus stat, int e) {
  ui->progressBar->setValue(i);
  if (stat == train_) {
    if (!model_.getCrossVal())
      ui->file_name_label->setText(
          QString::number(e) + " from " + QString::number(model_.getEpoch()) +
          " epoch" + " | error control value = " +
          QString::number(model_.getErrorTrain()) + "%");
    else
      ui->file_name_label->setText(QString::number(e) + " from " +
                                   QString::number(model_.getCrossVal()) +
                                   " groups" + " | error control value = " +
                                   QString::number(model_.getErrorTrain()) +
                                   "%");
  }
}

void Messages::modelReady(mStatus status) {
  ready_ = true;
  switch (status) {
    case test_:
      ui->label->setText("Testing completed");
      break;
    case train_:
      ui->label->setText("Training completed");
      ui->saveChartPushButton->setVisible(true);
      break;
    case read_:
      ui->label->setText("Reading completed");
      break;
    default:
      break;
  }
  ui->break_pushButton->setText("OK");
}

void Messages::reject() {
  if (isChart_ || ready_) {
    isChart_ = false;
    QDialog::reject();
  } else {
    int ret = QMessageBox::question(this, "APP_NAME", tr("Are you sure?\n"),
                                    QMessageBox::No | QMessageBox::Yes,
                                    QMessageBox::No);
    if (ret == (int)QMessageBox::Yes) {
      if (!ready_) model_.setBreak(true);
      QDialog::reject();
    }
  }
}

void Messages::showChart(bool) {
  this->setWindowTitle("Error change");
  isChart_ = true;
  this->setFixedSize(600, 600);
  ui->chart_pushButton->setVisible(false);
  ui->file_name_label->setVisible(false);
  ui->label->setVisible(false);
  ui->progressBar->setVisible(false);
  ui->break_pushButton->setVisible(false);
  ui->chartArea->setVisible(true);
  ui->chartArea->setGeometry(0, 0, 600, 600);
  this->exec();
}

void Messages::on_saveChartPushButton_clicked() {
  ui->chartArea->grab().save(
      QDir::homePath() + "/chart_" + QTime::currentTime().toString() + ".png",
      "png");
}

// void Messages::train() {
//     ui->break_pushButton->setText("Break");
//     ready_ = false;
//   this->setWindowTitle("Training");
//   this->setFixedSize(600,200);
//   clearChart();
//   ui->chart_pushButton->setText("Show chart");
//   ui->label->setText("Training...");
//   ui->file_name_label->setVisible(true);
//   ui->label->setVisible(true);
//   ui->progressBar->setVisible(true);
//   ui->break_pushButton->setVisible(true);
//   ui->chart_pushButton->setVisible(true);
//   ui->chartArea->setVisible(true);
//   ui->chartArea->setGeometry(20,190,560,390);
//   // this->show();
//   this->exec();
// }

// void Messages::test() {
//     ui->break_pushButton->setText("Break");
//     ready_ = false;
//   this->setWindowTitle("Testing");
//   this->setFixedSize(600,200);
//   ui->label->setText("Testing...");
//   ui->file_name_label->setVisible(false);
//   ui->label->setVisible(true);
//   ui->progressBar->setVisible(true);
//   ui->break_pushButton->setVisible(true);
//   ui->chart_pushButton->setVisible(false);
//   ui->chartArea->setVisible(false);
//   // this->show();
//   this->exec();
// }

// void Messages::readingFile(std::string str) {
//     ready_ = false;
//     ui->break_pushButton->setText("Break");
//   this->setWindowTitle("Reading");
//   this->setFixedSize(600,200);
//   ui->file_name_label->setVisible(true);
//   ui->label->setVisible(true);
//   ui->progressBar->setVisible(true);
//   ui->break_pushButton->setVisible(true);
//   ui->chart_pushButton->setVisible(false);
//   ui->chartArea->setVisible(false);
//   ui->label->setText("Reading file: ");
//   ui->file_name_label->setText(QString::fromStdString(str));
//   // this->show();
//   this->exec();
// }
}  // namespace s21
