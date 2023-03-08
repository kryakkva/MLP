//
// Created by apaege on 13.02.2023.
//

#include "../headers/Layer.h"

namespace s21 {
Layer::Layer(std::unordered_map<std::string, double> parameters) {
  _parameters = parameters;
  _type = static_cast<layerType>(static_cast<int>(parameters["type"]));
  initLayer();
}

Layer::~Layer() {
  for (Neuron* n : _neurons) delete n;
  _neurons.clear();
}

void Layer::initLayer() {
  _neurons.clear();
  for (size_t i_neuron = 0; i_neuron < _parameters["size"]; ++i_neuron)
    _neurons.push_back(new Neuron(i_neuron, this));
}

void Layer::clean() {
  for (Neuron* n : _neurons) n->clean();
}

void Layer::trigger() {
  for (Neuron* n : _neurons) n->trigger();
}

void Layer::connectComplete(Layer* next) {
  for (Neuron* n1 : _neurons)
    for (Neuron* n2 : next->_neurons) n1->addNext(n2);
}

std::vector<double> Layer::output() {
  std::vector<double> outputs;
  for (Neuron* n : _neurons) outputs.push_back(n->output());
  return outputs;
}

const std::vector<Neuron*>& Layer::neurons() const { return _neurons; }

void Layer::alterWeights(const std::vector<std::vector<double>>& weights) {
  for (size_t i_neuron = 0; i_neuron < weights.size(); ++i_neuron)
    _neurons[i_neuron]->alterWeights(weights[i_neuron]);
}

void Layer::shiftBackWeights(double lr) {
  for (size_t i_neuron = 0; i_neuron < _neurons.size(); ++i_neuron)
    _neurons[i_neuron]->shiftBackWeights(lr);
}

std::vector<std::vector<double*>> Layer::getWeights() {
  std::vector<std::vector<double*>> w;
  for (size_t i_neuron = 0; i_neuron < _neurons.size(); ++i_neuron)
    w.push_back(_neurons[i_neuron]->getWeights());
  return w;
}

void Layer::randomizeAllWeights(double d) {
  for (Neuron* neuron : _neurons) neuron->randomizeAllWeights(d);
}

void Layer::randomizeAllBias(double d) {
  for (Neuron* neuron : _neurons) neuron->randomizeAllBias(d);
}

void Layer::getBackPropagationShifts(double expect) {
  for (size_t i_neuron = 0; i_neuron < _neurons.size(); i_neuron++) {
    Neuron* n = _neurons[i_neuron];
    n->getBackPropagationShifts(expect);
  }
}

layerType Layer::getType() const { return _type; }

}  // namespace s21