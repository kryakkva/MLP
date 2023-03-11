//
// Created by Yarik on 25.02.2023.
//

#include "StateMessage.h"

namespace s21 {
StateMessages::StateMessages(NeuralNetwork &model, QObject *parent) : QObject(parent), model_(model) {}

StateMessages::~StateMessages() noexcept {
}

void StateMessages::readingFile() {
  msgBox_.setModal(true);
  msgBox_.resize(400, 300);
  // ok_btn_ = new QPushButton("OK", &msgBox_);
  break_btn_ = new QPushButton("Break", &msgBox_);
  bar_ = new QProgressBar(&msgBox_);
  bar_->setGeometry(50, 50, 300, 30);
  // ok_btn_->setGeometry(QRect(50, 70, 50, 30));
  break_btn_->setGeometry(QRect(50, 20, 50, 30));
  // connect(ok_btn_, SIGNAL(clicked(bool)), this, SLOT(okBtnClicked(bool)));
  connect(break_btn_, SIGNAL(clicked(bool)), this, SLOT(breakBtnClicked(bool)));
  connect(bar_, SIGNAL(valueChanged(int)), this, SLOT(proggressBarFull(int)));
  msgBox_.exec();
  delete bar_;
  bar_ = nullptr;
  delete break_btn_;
}

void StateMessages::okBtnClicked(bool b) {
  qInfo() << "OK";
}

void StateMessages::breakBtnClicked(bool b) {
  qInfo() << "break";
  int ret = QMessageBox::warning(&msgBox_ ,tr("My Application"),
                                   tr("The file is oppening.\n"
                                      "are you shure break oppen?"),
                                   QMessageBox::Yes | QMessageBox::Cancel,
                                   QMessageBox::Cancel);
  if (ret == (int)QMessageBox::Yes) {
    model_.setBreak(true);
    msgBox_.close();
  }
}

void StateMessages::updateBarVal(int i) {
  if (bar_ != nullptr)
    bar_->setValue(i);
}

void StateMessages::proggressBarFull(int i) {
  if ( i == 100){
    // break_btn_->setText("OK");
    msgBox_.close();
  }
}

} // s21