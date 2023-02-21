//
// Created by apaege on 08.01.2023.
//

#ifndef MLP_NETWORK_H
#define MLP_NETWORK_H

#include "../headers/Matrix.h"
#include <fstream>
#include <sstream>
#include <vector>

namespace s21 {
    class Network {
    private:
        int _typeNet;
        int _hidden;
        int _epoch;
        double _maxRa;
        std::vector<int> _layerSize;
        ActivateFunction    _actFunc;
        Matrix  *_weights;
        double **_bias;
        double  **_neurons_val, **_neurons_err;
        std::chrono::duration<double> _time;

        void initNet();
    public:
        std::vector<std::vector<double>> _vector_train;
        std::vector<std::vector<double>> _vector_test;
        Network();
        ~Network();

        void reInitNet(int l);
        void SetInput(std::vector<double> values, int fl = 1);

        double NetworkTest(std::vector<std::vector<double>> value);
        double NetworkTrain(std::vector<std::vector<double>> value);
        int ForwardFeed();
        int SearchMaxIndex(double *value);

        void BackPropogation(double expect);
        void WeightsUpdater(double lr);

        void SaveWeights_M(std::string filename);
        void ReadWeights_M(std::string filename);

        std::vector<std::vector<double>> ReadData(std::string filename);
        void destroyNet();

        int getEpoch();
        void setTypeNet(int n);
        void setLayer(int n);
        void setEpoch(int n);
        double getMaxRa();
        std::chrono::duration<double> getTime();
    };
} // s21

#endif //MLP_NETWORK_H
