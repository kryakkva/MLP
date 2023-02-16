//
// Created by apaege on 25.12.2022.
//

#ifndef MLP_MATRIX_H
#define MLP_MATRIX_H

#include "./ActivateFunction.h"
#include <vector>

namespace s21 {
    class Matrix {
    private:
        double** _matrix;
        int     _row, _col;
    public:
        Matrix();
        ~Matrix();
        Matrix(int row, int col);
        static void Multi(const Matrix &m, const double *neuron, int n, double *neuron_new);
        static void Multi_T(const Matrix &m, const double *neuron, int n, double *neuron_new);
        static void SumVector(double *a, const double *b, int n);
        double &operator () (int i, int j);

        friend std::ostream &operator << (std::ostream &os, const Matrix &m);
        friend std::istream &operator >> (std::istream &is, Matrix &m);
        int getCol();
        int getRow();
        double** getMatrix();
        void setMatrix(std::vector<std::vector<double>>& m);
    };
} // s21

#endif //MLP_MATRIX_H
