//
// Created by apaege on 12.02.2023.
//

#include "../headers/Function.h"

namespace s21 {
void Function::use(double *value, int n) {
  for (int i = 0; i < n; i++) value[i] = 1. / (1. + exp(-value[i]));
}

double Function::useDer(double value) { return value * (1. - value); }

double Function::useG(double value) { return 1. / (1. + exp(-value)); }

double Function::useDerG(double value) {
  return useG(value) * (1. - useG(value));
}

double Function::randomWeight(double d) {
  return (rand() % 100) * 0.03 / (d + 21);
}

double Function::randomBias(double d) {
  return (rand() % 50) * 0.06 / (d + 21);
}
}  // namespace s21