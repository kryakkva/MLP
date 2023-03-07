//
// Created by apaege on 08.01.2023.
//

#ifndef MLP_NETWORK_H
#define MLP_NETWORK_H

#include <QWidget>
#include <QMessageBox>
#include "../headers/Matrix.h"
#include <fstream>
#include <sstream>
#include <vector>

namespace s21 {
enum mStatus {
  test_ = 0,
  train_,
  read_,
};
    class Network : public QObject {
      Q_OBJECT
    private:
        int _typeNet;
        int _hidden;
        int _epoch;
        int _counter;
        double _maxRa;
        std::vector<int> _layerSize;
        ActivateFunction    _actFunc;
        Matrix  *_weights;
        double **_bias;
        double  **_neurons_val, **_neurons_err;
        std::chrono::duration<double> _time;
        void initNet();
        bool break_;
        // std::string content_dir_;
        double test_part_;
     private slots:
      // double NetworkTrain(std::vector<std::vector<double>> value);
      void NetworkTrain(bool b);
      void NetworkTest(bool is_auto);
      void SetTestPart(int i);
      void setLayer(int n);
      void setEpoch(int n);
      void ReadData(std::string filename, mStatus v);
     signals:
      void updateBar(int i, mStatus stat, int e = 0);
      void dialogMsg(mStatus status, std::string str = "");
      void actTrainBtn(bool);
      void actTestBtn(bool);
      void actChartBtn(bool);
      void iAmReady(mStatus status);
      void updateChart(double);
      void wrongFile();
      void isTrained(bool, int e = 0);
     public:
      std::vector<std::vector<double>> _vector_train;
      std::vector<std::vector<double>> _vector_test;
      Network(QObject *parent = nullptr);
      void setBreak(bool b);
      ~Network();
      void reInitNet(int l);
      void SetInput(std::vector<double> values, int fl = 1);
      int ForwardFeed();
      int SearchMaxIndex(double *value);
      void BackPropogation(double expect);
      void WeightsUpdater(double lr);
      void SaveWeights_M(std::string filename);
      void ReadWeights_M(std::string filename);
      void destroyNet();
      int getEpoch();
      void setTypeNet(int n);
      double getMaxRa();
      std::chrono::duration<double> getTime();
      int getLayers();
    };
} // s21

#endif //MLP_NETWORK_H
