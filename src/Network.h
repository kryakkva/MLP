//
// Created by apaege on 08.01.2023.
//

#ifndef MLP_NETWORK_H
#define MLP_NETWORK_H

#include "Matrix.h"
#include <fstream>
#include <sstream>
#include <vector>

namespace s21 {
    struct data_Network {
        int L;
        int *size;
    };
    class Network {
        int L;
        int *size;
        ActivateFunction    actFunc;
        Matrix  *weights;
        double  **bias;
        double  **neurons_val, **neurons_err;
        double  *neurons_bias_val;
    public:
        void Init(data_Network data);
        void PrintConfig();
        void SetInput(std::vector<double> values);

        int ForwardFeed();
        int SearchMaxIndex(double *value);

        void BackPropogation(double expect);
        void WeightsUpdater(double lr);

        void SaveWeights();
        void ReadWeights();

        void ClearLeaks();
        ~Network();
    };
} // s21

#endif //MLP_NETWORK_H
