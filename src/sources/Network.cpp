//
// Created by apaege on 08.01.2023.
//

#include "../headers/Network.h"

namespace s21 {
    Network::Network() {}

    Network::~Network() {
        for (int i = 0; i < _hidden + 2; i++) {
            delete [] _neurons_val[i];
            delete [] _neurons_err[i];
        }
        delete [] _neurons_val;
        delete [] _neurons_err;
//        delete [] _neurons_bias_val;
        for (int i = 0; i < _hidden + 1; i++)
            delete [] _bias[i];
        delete [] _bias;
        for (int i = 0; i < _hidden + 1; i++)
            _weights[i].ClearLeaks(_layerSize[i + 1]);
        delete [] _weights;
    };

    void Network::InitMatrixNet() {
        _typeNet = 0;
        _layerSize.push_back(784);
        for (int i = 0; i < _hidden; i++)
            _layerSize.push_back(150);
        _layerSize.push_back(26);
        srand(time(NULL));
        _weights = new Matrix[_hidden + 1];
        _bias = new double *[_hidden + 1];
//        _bias.resize(_hidden + 1);
        for (int i = 0; i < _hidden + 1; i++) {
            _weights[i].MatrixInit(_layerSize[i + 1], _layerSize[i]);
            _bias[i] = new double [_layerSize[i + 1]];
//            _bias[i].resize(_layerSize[i + 1], ((rand() % 50)) * 0.06 / (_layerSize[i] + 21));
            for (int j = 0; j < _layerSize[i + 1]; j++)
                _bias[i][j] = ((rand() % 50)) * 0.06 / (_layerSize[i] + 21);
        }
        _neurons_val = new double *[_hidden + 2];
        _neurons_err = new double *[_hidden + 2];
        for (int i = 0; i < _hidden + 2; i++) {
            _neurons_val[i] = new double [_layerSize[i]];
            _neurons_err[i] = new double [_layerSize[i]];
        }
//        _neurons_bias_val = new double[_hidden + 1];
//        for (int i = 0; i < _hidden + 1; i++)
//            _neurons_bias_val[i] = 1;
    }

    void Network::SetInput(std::vector<double> values, int fl) {
        if (!_typeNet) {
            for (int i = 0; i < values.size(); i++)
                _neurons_val[0][i] = values[i + fl];
        }
    }

    double Network::NetworkTest(std::vector<std::vector<double>> value) {
        double ra = 0;
        double right;
        double predict;
        for (int i = 0; i < value.size(); ++i) {
            SetInput(value[i]);
            predict = ForwardFeed();
            right = value[i][0];
            if (right == predict)
                ra++;
        }
        return (ra);
    }

    double Network::NetworkTrain(std::vector<std::vector<double>> value) {
        double ra = 0;
        double right;
        double predict;
        for (int i = 0; i < value.size(); ++i) {
            SetInput(value[i]);
            predict = ForwardFeed();
            right = value[i][0];
            if (right != predict) {
                BackPropogation(right);
                WeightsUpdater(2.1 * pow(2.1, -_epoch / 10.));
            }
            else
                ra++;
        }
        return (ra);
    }

    double Network::ForwardFeed() {
        for (int i = 1; i < _hidden + 2; ++i) {
            Matrix::Multi(_weights[i - 1], _neurons_val[i - 1], _layerSize[i - 1], _neurons_val[i]);
            Matrix::SumVector(_neurons_val[i], _bias[i - 1], _layerSize[i]);
            _actFunc.Use(_neurons_val[i], _layerSize[i]);
        }
        double pred = SearchMaxIndex(_neurons_val[_hidden + 1]);
        return pred;
    }

    int Network::SearchMaxIndex(double *value) {
        double max = value[0];
        int prediction = 1;
        double tmp;
        for (int i = 1; i < _layerSize[_hidden + 1]; i++) {
            tmp = value[i];
            if (tmp > max) {
                prediction = i + 1;
                max = tmp;
            }
        }
        return prediction;
    }

    void Network::BackPropogation(double expect) {
        for (int i = 0; i < _layerSize[_hidden + 1]; i++) {
            if (i != int(expect - 1))
                _neurons_err[_hidden + 1][i] = -_neurons_val[_hidden + 1][i] * _actFunc.UseDer(_neurons_val[_hidden + 1][i]);
            else
                _neurons_err[_hidden + 1][i] = (1.0 - _neurons_val[_hidden + 1][i]) * _actFunc.UseDer(_neurons_val[_hidden + 1][i]);
        }
        for (int k = _hidden; k > 0; k--) {
            Matrix::Multi_T(_weights[k], _neurons_err[k + 1], _layerSize[k + 1], _neurons_err[k]);
            for (int j = 0; j < _layerSize[k]; j++)
                _neurons_err[k][j] *= _actFunc.UseDer(_neurons_val[k][j]);
        }
    }

    void Network::WeightsUpdater(double lr) {
        for (int i = 0; i < _hidden + 1; ++i)
            for (int j = 0; j < _layerSize[i + 1]; ++j)
                for (int k = 0; k < _layerSize[i]; ++k)
                    _weights[i](j, k) += _neurons_val[i][k] * _neurons_err[i + 1][j] * lr;
        for (int i = 0; i < _hidden + 1; i++)
            for (int j = 0; j < _layerSize[i + 1]; j++)
                _bias[i][j] += _neurons_err[i + 1][j] * lr;
    }

    void Network::SaveWeights_M(std::string filename) {

        std::ofstream           fout;
        fout.open(filename);
        if (!fout.is_open()) {
            std::cout << "Error reading file " << filename << std::endl;
            exit(0);
        }
        fout << "This is weights file" << std::endl;
        for (int i = 0; i < _hidden + 2; ++i)
            fout << _layerSize[i] << " ";
        fout << std::endl;
        for (int x = 0; x < _hidden + 1; ++x)
            fout << _weights[x] << " ";
        for (int i = 0; i < _hidden + 1; ++i)
            for (int j = 0; j < _layerSize[i + 1]; ++j)
                fout << _bias[i][j] << " ";

        std::cout << filename << " saved\n";
        fout.close();
    }

    void Network::ReadWeights_M(std::string filename) {
        std::ifstream           fin;
        fin.open(filename);
        int multi;
        if (!fin.is_open()) {
            std::cout << "Error reading file " << filename << std::endl;
            exit(0);
        }
        std::string test;
        std::getline(fin, test);
        if (test.compare("This is weights file") != 0) {
            printf("You open wrong file!!!\n");
            exit(0);
        }
        std::vector<int> line;
        double num;
        fin >> num;
        if (num - static_cast<int>(num) != 0) {
            printf("Wrong in file\n");
            exit(0);
        }
        line.push_back(static_cast<int>(num));
        while (fin.get() != '\n') {
            fin >> num;
            if (num - (int)num != 0) {
                printf("Wrong in file\n");
                exit(0);
            }
            line.push_back(static_cast<int>(num));
        }
        if (_hidden + 2 != line.size()) {
            printf("Wrong in file\n");
            exit(0);
        }
        for (int x = 0; x < _hidden + 1; ++x) {
            multi = line[x] * line[x + 1];
            for (int i = 0; i < line[x + 1]; ++i) {
            }
            int coun = 0;
            fin >> _weights[x];
            for (int i = 0; i < _weights[x].getRow(); ++i) {
                for (int j = 0; j < _weights[x].getCol(); ++j) {
                    coun++;
                    if (fin.eof())
                        break;
                }
                if (fin.eof())
                    break;
            }

            if (coun != multi) {
                printf("Amount of number is wrong!!!\n");
                exit(0);
            }
        }
        for (int i = 0; i < _hidden + 1; ++i)
            for (int j = 0; j < _layerSize[i + 1]; ++j)
                fin >> _bias[i][j];

        std::cout << filename << " readed \n";;
        fin.close();
    }

    std::vector<std::vector<double>> Network::ReadData(std::string filename) {
        std::vector<std::vector<double>> _vect;
        std::ifstream fin(filename);
        if (!fin.is_open())
            std::cout << "Error reading file" << filename << std::endl;
        else
            std::cout << filename << " loading...\n";
        std::string tmp;
        bool first;
        while (!fin.eof()) {
            std::getline(fin, tmp);
            std::istringstream input;
            input.str(tmp.c_str());
            first = true;
            std::vector<double> _number;
            for (std::string num; std::getline(input, num, ',');) {
                if (first) {
                    _number.push_back(stoi(num));
                    first = false;
                } else
                    _number.push_back(stoi(num) / 255.);
            }
            if (tmp != "")
                _vect.push_back(_number);
        }
        fin.close();
        std::cout << "MNIST loaded... \n";
        return _vect;
    }

    int Network::getEpoch() {return _epoch;}

    void Network::setTypeNet(int n) {_typeNet = n;}

    void Network::setLayer(int n) {_hidden = n;}

    void Network::setEpoch(int n) {_epoch = n;}

    int Network::getMaxra() {return _maxra;}

 } // s21
