//
// Created by apaege on 12.02.2023.
//

#include "../headers/Neuron.h"

namespace s21 {
Neuron::Neuron(int id_neuron, Layer* layer)
    : _id_neuron(id_neuron), _layer(layer) {}

Neuron::~Neuron() {
  for (Edge* e : _next) delete e;
}

void Neuron::trigger() {
  for (Edge* e : _next) {
    e->propagate(output());
  }
}

double Neuron::getBias() { return _bias; }

double Neuron::getError() { return _error; }

double Neuron::output() {
  if (_layer->getType() == layerType::INPUT) return _accumulated;
  return _func.useG(_accumulated + _bias);
}

double Neuron::outputDerivative() {
  return _func.useDerG(_accumulated + _bias);
}

void Neuron::clean() { setAccumulated(0); }

void Neuron::addAccumulated(double v) { setAccumulated(_accumulated + v); }

void Neuron::addNext(Neuron* n) {
  _next.push_back(new Edge(n, this, 0));
  n->addPrevious(_next.back());
}

void Neuron::addPrevious(Edge* e) { _previous.push_back(e); }

int Neuron::getNeuronId() const { return _id_neuron; }

void Neuron::setAccumulated(double v) { _accumulated = v; }

void Neuron::setBias(double v) { _bias = v; }

void Neuron::alterWeights(const std::vector<double>& weights) {
  for (size_t i_edge = 0; i_edge < weights.size(); ++i_edge)
    _next[i_edge]->alterWeight(weights[i_edge]);
}

std::vector<double> Neuron::getWeights() {
  std::vector<double> w;
  for (size_t i_edge = 0; i_edge < _next.size(); ++i_edge)
    w.push_back(_next[i_edge]->weight());
  return w;
}

void Neuron::randomizeAllWeights(double d) {
  for (Edge* e : _next) {
    e->alterWeight(_func.randomWeight(d));
  }
}

void Neuron::randomizeAllBias(double d) { setBias(_func.randomBias(d)); }

void Neuron::shiftBackWeights(double lr) {
  for (size_t i = 0; i < _previous.size(); i++)
    _previous[i]->shiftWeight(_previous[i]->neuronb()->output() * _error * lr);
  _bias += _error * lr;
}

void Neuron::getBackPropagationShifts(double expect) {
  if (_layer->getType() == layerType::OUTPUT) {
    double d1 = output();
    double d2 = outputDerivative();
    if (this->getNeuronId() != (int)(expect - 1))
      _error = -d1 * d2;
    else
      _error = (1 - d1) * d2;
  } else {
    for (size_t i = 0; i < _next.size(); i++)
      _error += _next[i]->backPropagationMemory() * _next[i]->weight();
    _error *= outputDerivative();
  }
}

std::vector<Edge*> Neuron::getNextEdge() { return _next; }

}  // namespace s21