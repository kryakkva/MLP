//
// Created by apaege on 12.02.2023.
//

#ifndef MLP_FUNCTION_H
#define MLP_FUNCTION_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace s21 {
class Function {
 public:
  void use(double *value, int n);
  double useDer(double value);
  double useG(double value);
  double useDerG(double value);
  double randomWeight(double d);
  double randomBias(double d);
};
}  // namespace s21
#endif  // MLP_FUNCTION_H
