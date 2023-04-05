#include "../headers/layer.h"

namespace s21 {
Layer::Layer(std::unordered_map<std::string, double> parameters) {
  parameters_ = parameters;
  type_ = static_cast<layerType>(static_cast<int>(parameters["type"]));
  initLayer();
}

Layer::~Layer() {
  for (Neuron* n : neurons_) delete n;
  neurons_.clear();
}

void Layer::initLayer() {
  neurons_.clear();
  for (size_t i_neuron = 0; i_neuron < parameters_["size"]; ++i_neuron)
    neurons_.push_back(new Neuron(i_neuron, this));
}

void Layer::clean() {
  for (Neuron* n : neurons_) n->clean();
}

void Layer::trigger() {
  for (Neuron* n : neurons_) n->trigger();
}

void Layer::connectComplete(Layer* next) {
  for (Neuron* n1 : neurons_) {
    for (Neuron* n2 : next->neurons_) n1->addNext(n2);
  }
}

std::vector<double> Layer::output() {
  std::vector<double> outputs;
  for (Neuron* n : neurons_) outputs.push_back(n->output());
  return outputs;
}

const std::vector<Neuron*>& Layer::neurons() const { return neurons_; }

void Layer::alterWeights(const std::vector<std::vector<double>>& weights) {
  for (size_t i_neuron = 0; i_neuron < weights.size(); ++i_neuron)
    neurons_[i_neuron]->alterWeights(weights[i_neuron]);
}

void Layer::shiftBackWeights(double lr) {
  for (size_t i_neuron = 0; i_neuron < neurons_.size(); ++i_neuron)
    neurons_[i_neuron]->shiftBackWeights(lr);
}

std::vector<std::vector<double>> Layer::getWeights() {
  std::vector<std::vector<double>> w;
  for (size_t i_neuron = 0; i_neuron < neurons_.size(); ++i_neuron)
    w.push_back(neurons_[i_neuron]->getWeights());
  return w;
}

void Layer::randomizeAllWeights(double d) {
  for (Neuron* neuron : neurons_) neuron->randomizeAllWeights(d);
}

void Layer::randomizeAllBias(double d) {
  for (Neuron* neuron : neurons_) neuron->randomizeAllBias(d);
}

void Layer::getBackPropagationShifts(double expect) {
  for (size_t i_neuron = 0; i_neuron < neurons_.size(); i_neuron++) {
    Neuron* n = neurons_[i_neuron];
    n->getBackPropagationShifts(expect);
  }
}

layerType Layer::getType() const { return type_; }

void Layer::clearAll() {
  for (Neuron* n : neurons_) n->clearAll();
  neurons_.clear();
}

}  // namespace s21