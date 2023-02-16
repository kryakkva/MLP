//
// Created by apaege on 08.01.2023.
//

#ifndef MLP_NETWORK_H
#define MLP_NETWORK_H

#include "./Matrix.h"
#include "./Dataset.h"
#include <fstream>
#include <sstream>
#include <vector>

namespace s21 {
    class Network {
    private:
        int _typeNet = 0;
        int _hidden = 2;
        int _epoch = 10;
        int _maxra = 0;
        std::vector<int> _layerSize;
        ActivateFunction    _actFunc;
        Matrix  *_weights;
        double  **_bias;
        double  **_neurons_val, **_neurons_err;
        double  *_neurons_bias_val;
    public:
        Network();
        Network(int l);
        ~Network();

        void InitMatrixNet();
        void SetInput(std::vector<double> values);

        int NetWork(const Dataset& data, Datatype d);
        int ForwardFeed();
        int SearchMaxIndex(double *value);

        void BackPropogation(double expect);
        void WeightsUpdater(double lr);

        void SaveWeights_M(std::string filename);
        void ReadWeights_M(std::string filename);

        void setTypeNet(int n);
        void setLayer(int n);
        void setEpoch(int n);

//        void ClearLeaks();
    };
} // s21

#endif //MLP_NETWORK_H
