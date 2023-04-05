#ifndef NEURON_H
#define NEURON_H

#include "edge.h"
#include "function.h"
#include "layer.h"

namespace s21 {
class Layer;
class Neuron;
class NeuralNetwork;

class Neuron {
 public:
  Neuron(int id_neuron, Layer* layer);
  ~Neuron();

  void trigger();
  double getBias();
  double getError();
  double output();
  double outputDerivative();
  void clean();
  void addAccumulated(double v);
  void addNext(Neuron* n);
  void addPrevious(Edge* e);
  int getNeuronId() const;
  void setAccumulated(double v);
  void setBias(double v);
  void alterWeights(const std::vector<double>& weights);
  std::vector<double> getWeights();
  void randomizeAllWeights(double d);
  void randomizeAllBias(double d);
  void shiftBackWeights(double lr);
  void getBackPropagationShifts(double expect);
  std::vector<Edge*> getNextEdge();
  void clearAll();

 private:
  int id_neuron_ = 0;
  double accumulated_ = 0.0;
  double error_ = 0.0;
  double bias_ = 0.0;
  Layer* layer_ = NULL;
  Function func_;
  std::vector<Edge*> next_;
  std::vector<Edge*> previous_;
};
}  // namespace s21

#endif  // NEURON_H
