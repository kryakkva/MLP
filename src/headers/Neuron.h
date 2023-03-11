//
// Created by apaege on 12.02.2023.
//

#ifndef MLP_NEURON_H
#define MLP_NEURON_H

#include "Edge.h"
#include "Function.h"
#include "Layer.h"

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

 private:
  int _id_neuron = 0;
  double _accumulated = 0.0;
  double _error = 0.0;
  double _bias = 0.0;
  Layer* _layer = NULL;
  Function _func;
  std::vector<Edge*> _next;
  std::vector<Edge*> _previous;
};
}  // namespace s21

#endif  // MLP_NEURON_H
