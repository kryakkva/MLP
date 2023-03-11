//
// Created by apaege on 25.12.2022.
//

#ifndef MLP_MATRIX_H
#define MLP_MATRIX_H

#include "Function.h"

namespace s21 {
class Matrix {
 public:
  Matrix();
  ~Matrix();
  void matrixInit(int row, int col);
  static void multi(const Matrix &m, const double *neuron, int n,
                    double *neuron_new);
  static void multi_T(const Matrix &m, const double *neuron, int n,
                      double *neuron_new);
  static void sumVector(double *a, const double *b, int n);
  double &operator()(int i, int j);
  friend std::ostream &operator<<(std::ostream &os, const Matrix &m);
  friend std::istream &operator>>(std::istream &is, Matrix &m);
  int getCol();
  int getRow();
  void clear(int row);

 private:
  double **_matrix;
  int _row, _col;
};
}  // namespace s21

#endif  // MLP_MATRIX_H
