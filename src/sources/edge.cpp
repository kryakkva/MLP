#include "../headers/edge.h"

namespace s21 {

Edge::Edge(Neuron* n, Neuron* nb, double w) : n_(n), nb_(nb), w_(w) {}

Neuron* Edge::neuron() const { return n_; }

Neuron* Edge::neuronB() const { return nb_; }

double Edge::weight() const { return w_; }

void Edge::propagate(double neuron_output) {
  neuron()->addAccumulated(neuron_output * weight());
}

void Edge::alterWeight(double w) { w_ = w; }

void Edge::shiftWeight(double d) { w_ += d; }

double Edge::backPropagationMemory() const { return neuron()->getError(); }

}  // namespace s21