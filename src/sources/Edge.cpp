//
// Created by apaege on 13.02.2023.
//

#include "../headers/Edge.h"

namespace s21 {

Edge::Edge(Neuron* n, Neuron* nb, double w) : _n(n), _nb(nb), _w(w) {}

Neuron* Edge::neuron() const { return _n; }

Neuron* Edge::neuronb() const { return _nb; }

double Edge::weight() const { return _w; }

void Edge::propagate(double neuron_output) {
  neuron()->addAccumulated(neuron_output * weight());
}

void Edge::alterWeight(double w) { _w = w; }

void Edge::shiftWeight(double d) { _w += d; }

double Edge::backPropagationMemory() const { return neuron()->getError(); }

}  // namespace s21