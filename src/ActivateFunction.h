//
// Created by apaege on 25.12.2022.
//

#ifndef MLP_ACTIVATEFUNCTION_H
#define MLP_ACTIVATEFUNCTION_H

#include <iostream>

namespace s21 {
    class ActivateFunction {
    public:
        void Use(double *value, int n);
        double UseDer(double value);
    };

} // s21
#endif //MLP_ACTIVATEFUNCTION_H
