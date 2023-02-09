//
// Created by apaege on 25.12.2022.
//

#include "../headers/ActivateFunction.h"

namespace s21 {
    void ActivateFunction::Use(double *value, int n) {
        for (int i = 0; i < n; i++)
            value[i] = 1 / (1 + exp(-value[i]));
    }
    double ActivateFunction::UseDer(double value) {
        value = value * (1 - value);
        return value;
    }
} // s21
