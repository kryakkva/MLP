//
// Created by Yarik on 25.02.2023.
//

#ifndef MLP_HEADERS_STATEMESSAGE_H_
#define MLP_HEADERS_STATEMESSAGE_H_

#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>

#include "messages.h"
#include "neural_network.h"

namespace s21 {
class StateMessages : public QObject {
  Q_OBJECT
 public:
  StateMessages(NeuralNetwork &model, QObject *parent = nullptr);
  ~StateMessages() noexcept;

 private:
  Messages *msg_;
  QDialog msgBox_;
  NeuralNetwork &model_;
  QProgressBar *bar_;
  QPushButton *break_btn_;
  QPushButton *ok_btn_;
 private slots:
  void readingFile();
  void updateBarVal(int i);
  void okBtnClicked(bool b);
  void breakBtnClicked(bool b);
  void proggressBarFull(int i);
};

}  // namespace s21

#endif  // MLP_HEADERS_STATEMESSAGE_H_
