//
// Created by apaege on 25.12.2022.
//

#ifndef MLP_MATRIX_H
#define MLP_MATRIX_H

#include "ActivateFunction.h"

namespace s21 {
    class Matrix {
        double  **matrix;
        int     row, col;
    public:
        void Init(int row, int col);
        static void Multi(const Matrix &m, const double *neuron, int n, double *neuron_new);
        static void Multi_T(const Matrix &m, const double *neuron, int n, double *neuron_new);
        static void SumVector(double *a, const double *b, int n);
        double &operator () (int i, int j);
        ~Matrix();
        void ClearLeaks(int row);
        friend std::ostream &operator << (std::ostream &os, const Matrix &m);
        friend std::istream &operator >> (std::istream &is, Matrix &m);
    };
} // s21

#endif //MLP_MATRIX_H
