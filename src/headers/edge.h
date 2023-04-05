#ifndef EDGE_H
#define EDGE_H

#include "neuron.h"

namespace s21 {

class Layer;
class Neuron;
class NeuralNetwork;

class Edge {
 public:
  Edge(Neuron* n, Neuron* nb, double w);
  Neuron* neuron() const;
  Neuron* neuronB() const;
  double weight() const;
  void propagate(double neuron_output);
  void alterWeight(double w);
  void shiftWeight(double d);
  double backPropagationMemory() const;

 private:
  Neuron* n_ = nullptr;
  Neuron* nb_ = nullptr;
  double w_ = 0.0;
};

}  // namespace s21

#endif  // EDGE_H
