#include "messages.h"
#include "ui_messages.h"

namespace s21 {

Messages::Messages(Network &model, QWidget *parent) :
    QDialog(parent), model_(model), ui(new Ui::Messages)
{
  ui->setupUi(this);
  connect(ui->break_pushButton, SIGNAL(clicked(bool)), this, SLOT(breakBtnClicked(bool)));
  this->setModal(true);
}

Messages::~Messages()
{
    delete ui;
}

void Messages::readingFile(std::string str) {
  ui->file_name_label->setText(QString::fromStdString(str));
  // connect(ui->progressBar, SIGNAL(valueChanged(int)), this, SLOT(proggressBarFull(int)));
  this->exec();
}

void Messages::train() {
  // connect(ui->break_pushButton, SIGNAL(clicked(bool)), this, SLOT(breakBtnClicked(bool)));
  ui->file_name_label->setDisabled(true);
  // connect(ui->progressBar, SIGNAL(valueChanged(int)), this, SLOT(proggressBarFull(int)));
  this->exec();
}

void Messages::okBtnClicked(bool b) {
  qInfo() << "OK";
}

void Messages::breakBtnClicked(bool b) {
  qInfo() << "break";
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
