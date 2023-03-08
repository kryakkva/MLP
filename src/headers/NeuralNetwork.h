//
// Created by apaege on 13.02.2023.
//

#ifndef MLP_NeuralNetwork_H
#define MLP_NeuralNetwork_H

#include "Layer.h"
#include "Matrix.h"

namespace s21 {
class NeuralNetwork;
class Neuron;
class Layer;

class NeuralNetwork {
 public:
  int _typeNetVal;
  int _hiddenVal;
  std::vector<std::vector<double>> _vector_train;
  std::vector<std::vector<double>> _vector_test;

  NeuralNetwork();
  ~NeuralNetwork();
  void reInitNet();
  void setInput(std::vector<double> values, int fl = 1);
  double networkTest(std::vector<std::vector<double>> value);
  double networkTrain(std::vector<std::vector<double>> value);
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
  std::vector<std::vector<std::vector<double *>>> getWeights();
  void alterWeights(
      const std::vector<std::vector<std::vector<double>>> &weights);
  void saveWeights(std::string filename);
  void readWeights(std::string filename);
  std::vector<std::vector<double>> readData(std::string filename);
  void destroyNet();
  int getEpoch();
  int getLayer();
  int getTypeNet();
  void setEpoch(int n);
  double getMaxRa();
  std::chrono::duration<double> getTime();

 private:
  int _typeNet;
  int _hidden;
  int _epoch;
  int _counter;
  double _maxRa;
  std::vector<int> _layerSize;
  Function _func;
  Matrix *_weights;
  double **_bias;
  double **_neurons_val, **_neurons_err;
  std::chrono::duration<double> _time;
  std::vector<Layer *> _layers;
  std::vector<std::unordered_map<std::string, double>> _configuration;

  void initNet();
};

}  // namespace s21

#endif  // MLP_NeuralNetwork_H
