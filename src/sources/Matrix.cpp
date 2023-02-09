//
// Created by apaege on 25.12.2022.
//

#include "../headers/Matrix.h"

namespace s21 {
    void Matrix::Init(int row_, int col_) {
        this->row = row_;
        this->col = col_;
        matrix = new double *[row];
        for (int i = 0; i < row; i++)
            matrix[i] = new double [col];
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                matrix[i][j] = (rand() % 100) * 0.03 / (row + 21);
    }

    void Matrix::Multi(const Matrix &m, const double *neuron, int n, double *neuron_new) {
        if (m.col != n)
            throw std::runtime_error("Error Multi\n");
        for (int i = 0; i < m.row; ++i) {
            double tmp = 0;
            for (int j = 0; j < m.col; ++j)
                tmp += m.matrix[i][j] * neuron[j];
            neuron_new[i] = tmp;
        }
    }

    void Matrix::Multi_T(const Matrix &m, const double *neuron, int n, double *neuron_new) {
        if (m.row != n)
            throw std::runtime_error("Error Multi_T\n");
        for (int i = 0; i < m.col; ++i) {
            double tmp = 0;
            for (int j = 0; j < m.row; ++j)
                tmp += m.matrix[j][i] * neuron[j];
            neuron_new[i] = tmp;
        }
    }

    void Matrix::SumVector(double *a, const double *b, int n) {
        for (int i = 0; i < n; ++i)
            a[i] += b[i];
    }

    double &Matrix::operator()(int i, int j) {
        return matrix[i][j];
    }

    std::ostream &operator << (std::ostream &os, const Matrix &m) {
        for (int i = 0; i < m.row; ++i)
            for (int j = 0; j < m.col; j++)
                os << m.matrix[i][j] << " ";
        return os;
    }

    std::istream &operator >> (std::istream &is, Matrix &m) {
        for (int i = 0; i < m.row; ++i)
            for (int j = 0; j < m.col; j++)
                is >> m.matrix[i][j];
        return is;
    }

    void Matrix::ClearLeaks(int row_) {
        for (int i = 0; i < row_; i++)
            delete [] matrix[i];
    }

    Matrix::~Matrix() {
        delete [] matrix;
    }
 } // s21
