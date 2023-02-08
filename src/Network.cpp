//
// Created by apaege on 08.01.2023.
//

#include "Network.h"

namespace s21 {
    void Network::Init(data_Network data) {
        srand(time(NULL));
        L = data.L;
        size = new int[L];
        for (int i = 0; i < L; i++)
            size[i] = data.size[i];
        weights = new Matrix[L - 1];
        bias = new double *[L - 1];
        for (int i = 0; i < L - 1; i++) {
            weights[i].Init(size[i + 1], size[i]);
            bias[i] = new double [size[i + 1]];
            for (int j = 0; j < size[i + 1]; j++)
                bias[i][j] = ((rand() % 50)) * 0.06 / (size[i] + 21);
        }
        neurons_val = new double *[L];
        neurons_err = new double *[L];
        for (int i = 0; i < L; i++) {
            neurons_val[i] = new double [size[i]];
            neurons_err[i] = new double [size[i]];
        }
        neurons_bias_val = new double[L - 1];
        for (int i = 0; i < L - 1; i++)
            neurons_bias_val[i] = 1;
    }

    void Network::PrintConfig() {
        std::cout << "***************************************************\n";
        std::cout << "Network has " << L - 2 << " hidden layers\nSIZE[]: ";
        for (int i = 0; i < L; i++)
            std::cout << size[i] << " ";
        std::cout << "\n***************************************************\n\n";
    }

    void Network::SetInput(std::vector<double> values) {
        std::vector<double>::iterator vIt;
        int i = 0;
        for (vIt = ++values.begin(); vIt != values.end(); ++vIt, i++)
           neurons_val[0][i] = *vIt;
    }

    int Network::ForwardFeed() {
        for (int i = 1; i < L; ++i) {
            Matrix::Multi(weights[i - 1], neurons_val[i - 1], size[i - 1], neurons_val[i]);
            Matrix::SumVector(neurons_val[i], bias[i - 1], size[i]);
            actFunc.Use(neurons_val[i], size[i]);
        }
        int pred = SearchMaxIndex(neurons_val[L - 1]);
        return pred;
    }

    int Network::SearchMaxIndex(double *value) {
        double max = value[0];
        int prediction = 1;
        double tmp;
        for (int i = 1; i < size[L - 1]; i++) {
            tmp = value[i];
            if (tmp > max) {
                prediction = i + 1;
                max = tmp;
            }
        }
        return prediction;
    }

    void Network::BackPropogation(double expect) {
        for (int i = 0; i < size[L - 1]; i++) {
            if (i != int(expect))
                neurons_err[L - 1][i] = -neurons_val[L - 1][i] * actFunc.UseDer(neurons_val[L - 1][i]);
            else
                neurons_err[L - 1][i] = (1.0 - neurons_val[L - 1][i]) * actFunc.UseDer(neurons_val[L - 1][i]);
        }
        for (int k = L - 2; k > 0; k--) {
            Matrix::Multi_T(weights[k], neurons_err[k + 1], size[k + 1], neurons_err[k]);
            for (int j = 0; j < size[k]; j++)
                neurons_err[k][j] *= actFunc.UseDer(neurons_val[k][j]);
        }
    }

    void Network::WeightsUpdater(double lr) {
        for (int i = 0; i < L - 1; ++i)
            for (int j = 0; j < size[i + 1]; ++j)
                for (int k = 0; k < size[i]; ++k)
                    weights[i](j, k) += neurons_val[i][k] * neurons_err[i + 1][j] * lr;
        for (int i = 0; i < L - 1; i++)
            for (int j = 0; j < size[i + 1]; j++)
                bias[i][j] += neurons_err[i + 1][j] * lr;
    }

    void Network::SaveWeights() {
        std::ostringstream      filename;
        std::ofstream           fout;
        filename << "src_data/Weights_" << L - 2 << ".txt";
        fout.open(filename.str());
        if (!fout.is_open()) {
            std::cout << "Error reading file " << filename.str() << std::endl;
            exit(0);
        }
        for (int i = 0; i < L - 1; ++i)
            fout << weights[i] << " ";
        for (int i = 0; i < L - 1; ++i)
            for (int j = 0; j < size[i + 1]; ++j)
                fout << bias[i][j] << " ";
        filename << " saved\n";
        std::cout << filename.str();
        fout.close();
    }

    void Network::ReadWeights() {
        std::ostringstream      filename;
        std::ifstream           fin;
        filename << "src_data/Weights_" << L - 2 << ".txt";
        fin.open(filename.str());
        if (!fin.is_open()) {
            std::cout << "Error reading file " << filename.str() << std::endl;
            exit(0);
        }
        printf("L = %d\n", L - 2);
        for (int i = 0; i < L - 1; ++i)
            fin >> weights[i];
        for (int i = 0; i < L - 1; ++i)
            for (int j = 0; j < size[i + 1]; ++j)
                fin >> bias[i][j];
        filename << " readed \n";
        std::cout << filename.str();
        fin.close();
    }

    void Network::ClearLeaks() {
        for (int i = 0; i < L -1; i++)
            weights[i].ClearLeaks(size[i + 1]);
    }

    Network::~Network() {
        for (int i = 0; i < L; i++) {
            delete [] neurons_val[i];
            delete [] neurons_err[i];
        }
        delete [] neurons_val;
        delete [] neurons_err;
        delete [] neurons_bias_val;
        for (int i = 0; i < L -1; i++)
            delete [] bias[i];
        delete [] bias;
        delete [] size;
        delete [] weights;
    };
 } // s21