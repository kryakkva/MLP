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
enum testTrain {
  test_ = 0,
  train_
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
        bool ready_;
     private slots:
      // double NetworkTrain(std::vector<std::vector<double>> value);
      void NetworkTrain();

      std::vector<std::vector<double>> ReadData(std::string filename, testTrain v);
     signals:
      void updateBar(int i);
      void readMessage(std::string str);
      void trainMessage();
      void openTrainFile();
      void iAmReady();
     public:
      std::vector<std::vector<double>> _vector_train;
      std::vector<std::vector<double>> _vector_test;
      Network(QObject *parent = nullptr);

      void setBreak(bool b);
      ~Network();

      void reInitNet(int l);
      void SetInput(std::vector<double> values, int fl = 1);
      double NetworkTest(std::vector<std::vector<double>> value);
      int ForwardFeed();

        int SearchMaxIndex(double *value);
      void BackPropogation(double expect);

        void WeightsUpdater(double lr);
      void SaveWeights_M(std::string filename);

        void ReadWeights_M(std::string filename);
        void destroyNet();

        int getEpoch();
        void setTypeNet(int n);
        void setLayer(int n);
        void setEpoch(int n);
        double getMaxRa();
      bool getReady() {return ready_;}
        std::chrono::duration<double> getTime();
    };
} // s21

#endif //MLP_NETWORK_H
