//
// Created by apaege on 25.12.2022.
//

#include "../headers/Matrix.h"

namespace s21 {
Matrix::Matrix() {}

Matrix::~Matrix() {}

void Matrix::matrixInit(int row, int col) {
  this->_row = row;
  this->_col = col;
  _matrix = new double *[_row];
  for (int i = 0; i < _row; i++) _matrix[i] = new double[_col];
  for (int i = 0; i < _row; i++)
    for (int j = 0; j < _col; j++) {
      _matrix[i][j] = (rand() % 100) * 0.03 / (_row + 21);
    }
}

void Matrix::multi(const Matrix &m, const double *neuron, int n,
                   double *neuron_new) {
  if (m._col != n) throw std::runtime_error("Error multi\n");
  for (int i = 0; i < m._row; ++i) {
    double tmp = 0;
    for (int j = 0; j < m._col; ++j) tmp += m._matrix[i][j] * neuron[j];
    neuron_new[i] = tmp;
  }
}

void Matrix::multi_T(const Matrix &m, const double *neuron, int n,
                     double *neuron_new) {
  if (m._row != n) throw std::runtime_error("Error multi_T\n");
  for (int i = 0; i < m._col; ++i) {
    double tmp = 0;
    for (int j = 0; j < m._row; ++j) tmp += m._matrix[j][i] * neuron[j];
    neuron_new[i] = tmp;
  }
}

void Matrix::sumVector(double *a, const double *b, int n) {
  for (int i = 0; i < n; ++i) a[i] += b[i];
}

double &Matrix::operator()(int i, int j) { return _matrix[i][j]; }

std::ostream &operator<<(std::ostream &os, const Matrix &m) {
  for (int i = 0; i < m._col; ++i)
    for (int j = 0; j < m._row; j++) os << m._matrix[j][i] << " ";
  return os;
}

std::istream &operator>>(std::istream &is, Matrix &m) {
  for (int i = 0; i < m._row; ++i)
    for (int j = 0; j < m._col; j++) is >> m._matrix[i][j];
  return is;
}

int Matrix::getCol() { return _col; }

int Matrix::getRow() { return _row; }

void Matrix::clear(int row_) {
  for (int i = 0; i < row_; i++) delete[] _matrix[i];
  delete[] _matrix;
}
}  // namespace s21
