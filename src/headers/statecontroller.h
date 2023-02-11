//
// Created by Yarik on 10.02.2023.
//

#ifndef MLP__STATECONTROLLER_H_
#define MLP__STATECONTROLLER_H_

#include <QMessageBox>
#include <QObject>

namespace s21 {

class StateController : public QObject {
  Q_OBJECT

 public:
  StateController();
  ~StateController() noexcept;

 private:
  int _layers;
  int _epoch;
  int _testP;
  bool _isMatrix;
  bool _train;
  bool _wait;

 private slots:
  // void setLayers(int);
  // void setEpoch(int);

 signals:


};

} // s21

#endif //MLP__STATECONTROLLER_H_
