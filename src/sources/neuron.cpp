#include "../headers/neuron.h"

namespace s21 {
Neuron::Neuron(int id_neuron, Layer* layer)
    : id_neuron_(id_neuron), layer_(layer) {}

Neuron::~Neuron() {
  for (Edge* e : next_) delete e;
}

void Neuron::trigger() {
  for (Edge* e : next_) {
    e->propagate(output());
  }
}

double Neuron::getBias() { return bias_; }

double Neuron::getError() { return error_; }

double Neuron::output() {
  if (layer_->getType() == layerType::INPUT) return accumulated_;
  return func_.useG(accumulated_ + bias_);
}

double Neuron::outputDerivative() {
  return func_.useDerG(accumulated_ + bias_);
}

void Neuron::clean() { setAccumulated(0); }

void Neuron::addAccumulated(double v) { setAccumulated(accumulated_ + v); }

void Neuron::addNext(Neuron* n) {
  next_.push_back(new Edge(n, this, 0));
  n->addPrevious(next_.back());
}

void Neuron::addPrevious(Edge* e) { previous_.push_back(e); }

int Neuron::getNeuronId() const { return id_neuron_; }

void Neuron::setAccumulated(double v) { accumulated_ = v; }

void Neuron::setBias(double v) { bias_ = v; }

void Neuron::alterWeights(const std::vector<double>& weights) {
  for (size_t i_edge = 0; i_edge < weights.size(); ++i_edge)
    next_[i_edge]->alterWeight(weights[i_edge]);
}

std::vector<double> Neuron::getWeights() {
  std::vector<double> w;
  for (size_t i_edge = 0; i_edge < next_.size(); ++i_edge)
    w.push_back(next_[i_edge]->weight());
  return w;
}

void Neuron::randomizeAllWeights(double d) {
  for (Edge* e : next_) {
    e->alterWeight(func_.randomWeight(d));
  }
}

void Neuron::randomizeAllBias(double d) { setBias(func_.randomBias(d)); }

void Neuron::shiftBackWeights(double lr) {
  for (size_t i = 0; i < previous_.size(); i++)
    previous_[i]->shiftWeight(previous_[i]->neuronB()->output() * error_ * lr);
  bias_ += error_ * lr;
}

void Neuron::getBackPropagationShifts(double expect) {
  if (layer_->getType() == layerType::OUTPUT) {
    double d1 = output();
    double d2 = outputDerivative();
    if (this->getNeuronId() != (int)(expect - 1))
      error_ = -d1 * d2;
    else
      error_ = (1 - d1) * d2;
  } else {
    for (size_t i = 0; i < next_.size(); i++)
      error_ += next_[i]->backPropagationMemory() * next_[i]->weight();
    error_ *= outputDerivative();
  }
}

std::vector<Edge*> Neuron::getNextEdge() { return next_; }

void Neuron::clearAll() {
  next_.clear();
  previous_.clear();
}
}  // namespace s21