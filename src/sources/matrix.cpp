#include "../headers/matrix.h"

namespace s21 {
Matrix::Matrix() {}

Matrix::~Matrix() {}

void Matrix::matrixInit(int row, int col) {
  this->row_ = row;
  this->col_ = col;
  matrix_ = new double *[row_];
  for (int i = 0; i < row_; i++) matrix_[i] = new double[col_];
  for (int i = 0; i < row_; i++)
    for (int j = 0; j < col_; j++) {
      matrix_[i][j] = (rand() % 100) * 0.03 / (row_ + 21);
    }
}

void Matrix::multi(const Matrix &m, const double *neuron, int n,
                   double *neuron_new) {
  if (m.col_ != n) throw std::runtime_error("Error multi\n");
  for (int i = 0; i < m.row_; ++i) {
    double tmp = 0;
    for (int j = 0; j < m.col_; ++j) tmp += m.matrix_[i][j] * neuron[j];
    neuron_new[i] = tmp;
  }
}

void Matrix::multi_T(const Matrix &m, const double *neuron, int n,
                     double *neuron_new) {
  if (m.row_ != n) throw std::runtime_error("Error multi_T\n");
  for (int i = 0; i < m.col_; ++i) {
    double tmp = 0;
    for (int j = 0; j < m.row_; ++j) tmp += m.matrix_[j][i] * neuron[j];
    neuron_new[i] = tmp;
  }
}

void Matrix::sumVector(double *a, const double *b, int n) {
  for (int i = 0; i < n; ++i) a[i] += b[i];
}

double &Matrix::operator()(int i, int j) { return matrix_[i][j]; }

std::ostream &operator<<(std::ostream &os, const Matrix &m) {
  for (int i = 0; i < m.col_; ++i)
    for (int j = 0; j < m.row_; j++) os << m.matrix_[j][i] << " ";
  return os;
}

std::istream &operator>>(std::istream &is, Matrix &m) {
  for (int i = 0; i < m.row_; ++i)
    for (int j = 0; j < m.col_; j++) is >> m.matrix_[i][j];
  return is;
}

int Matrix::getCol() { return col_; }

int Matrix::getRow() { return row_; }

void Matrix::clear(int row_) {
  for (int i = 0; i < row_; i++) delete[] matrix_[i];
  delete[] matrix_;
}
}  // namespace s21
