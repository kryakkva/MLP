//
// Created by apaege on 13.02.2023.
//

#ifndef MLP_LAYER_H
#define MLP_LAYER_H

#include <unordered_map>

#include "Neuron.h"

namespace s21 {
class Layer;
class Neuron;
class NeuralNetwork;
class Edge;

enum layerType {
  STANDARD = 0,
  OUTPUT,
  INPUT,
};

class Layer {
 public:
  Layer(std::unordered_map<std::string, double> parameters);
  ~Layer();
  void initLayer();
  void clean();
  void trigger();
  void connectComplete(Layer* next);
  std::vector<double> output();
  const std::vector<Neuron*>& neurons() const;
  void alterWeights(const std::vector<std::vector<double> >& weights);
  void shiftBackWeights(double lr);
  std::vector<std::vector<double> > getWeights();
  void randomizeAllWeights(double d);
  void randomizeAllBias(double d);
  void getBackPropagationShifts(double expect);
  layerType getType() const;

 private:
  std::vector<Neuron*> _neurons;
  layerType _type;
  std::unordered_map<std::string, double> _parameters;
};

}  // namespace s21

#endif  // MLP_LAYER_H
