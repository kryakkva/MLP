//
// Created by apaege on 13.02.2023.
//

#ifndef MLP_EDGE_H
#define MLP_EDGE_H

#include "Neuron.h"

extern double LEARNING_RATE;

namespace s21 {

class Layer;
class Neuron;
class NeuralNetwork;

class Edge {
 public:
  Edge(Neuron* n, Neuron* nb, double w);
  Neuron* neuron() const;
  Neuron* neuronb() const;
  double weight() const;
  void propagate(double neuron_output);
  void alterWeight(double w);
  void shiftWeight(double d);
  double backPropagationMemory() const;

 private:
  Neuron* _n = nullptr;
  Neuron* _nb = nullptr;
  double _w = 0.0;
};

}  // namespace s21

#endif  // MLP_EDGE_H
