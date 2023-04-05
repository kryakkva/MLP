#ifndef MATRIX_H
#define MATRIX_H

#include "function.h"

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
  double **matrix_;
  int row_, col_;
};
}  // namespace s21

#endif  // MATRIX_H
