//
// Created by apaege on 08.01.2023.
//

#ifndef MLP_NETWORK_H
#define MLP_NETWORK_H

#include <QWidget>
#include <QMessageBox>
#include "Layer.h"
#include "Matrix.h"
#include <fstream>
#include <sstream>
#include <vector>

namespace s21 {
class NeuralNetwork;
class Neuron;
class Layer;

enum mStatus {
  test_ = 0,
  train_,
  read_,
};
    class NeuralNetwork : public QObject {
      Q_OBJECT
    private:
      bool _typeNet;
      int _hidden;
      int _epoch;
      int _counter;
      int _crossVal = 0;
      double _maxRa;
      std::vector<int> _layerSize;
      Function _func;
      Matrix  *_weights;
      double **_bias;
      double  **_neurons_val, **_neurons_err;
      std::chrono::duration<double> _time;
      std::vector<Layer *> _layers;
      std::vector<std::unordered_map<std::string, double>> _configuration;
      bool break_;
      double test_part_;
      std::vector<std::vector<std::vector<double>>> _w_temp;
      std::vector<std::vector<double>> _b_temp;
      bool _reWrite = false;
      void crossVal(int e);
      void setCrossVal(int c);
      void initNet();
     private slots:
      void networkTrain(bool b);
      void networkTest(bool is_auto);
      void SetTestPart(int i);
      // void setLayer(int n);
      void setEpoch(int n);
      void readData(std::string filename, mStatus v);
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
      std::vector<std::vector<double>> _vector_train_cross;
      std::vector<std::vector<double>> _vector_test_cross;
      NeuralNetwork(QObject *parent = nullptr);

      ~NeuralNetwork();
      void setBreak(bool b);
      void reInitNet(int l, bool b);
      void reSaveStudy();
      void reLoadStudy();
      void setInput(std::vector<double> values, int fl = 1);
      int forwardFeed();
      int searchMaxIndex(double *value);
      void backPropagation(double expect);
      void weightsUpdater(double lr);
      void trigger();
      int searchMaxIndexGraph(std::vector<double> values);
      void getBackPropagationShifts(double expect);
      void shiftBackWeights(double lr);
      void autogenerate();
      void connectComplete();
      void randomizeAllWeightsBias();
      void addLayer(std::unordered_map<std::string, double> parameters);
      void clean();
      std::vector<double> output();
      std::vector<std::vector<std::vector<double>>> getWeights();
      void alterWeights(
          const std::vector<std::vector<std::vector<double>>> &weights);
      void saveWeights(std::string filename);
      void setTypeNet(bool n);
      void readWeights(std::string filename);
      void destroyNet();
      int getEpoch();
      bool getTypeNet();
      double getMaxRa();
      std::chrono::duration<double> getTime();
      int getLayer();
    };
} // s21

#endif //MLP_NETWORK_H
