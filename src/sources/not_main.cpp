//
// Created by apaege on 25.12.2022.
//

#include "NeuralNetwork.h"
#include <chrono>
#include <string>

    s21::data_Network ReadDataNetwork(std::string path) {
        s21::data_Network data{};
        std::ifstream fin;
        fin.open(path);
        if (!fin.is_open())
            std::cout << "Error reading file" << path << std::endl;
        else
            std::cout << path << " loading...\n";
        std::string tmp;
        int L;
        while (!fin.eof()) {
            fin >> tmp;
            if (tmp == "NetWork") {
                fin >> L;
                data.L = L;
                data.size = new int[L];
                for (int i = 0; i < L; i++)
                    fin >> data.size[i];
            }
        }
        fin.close();
        return data;
    }

    std::vector<std::vector<double>> ReadData_sep(std::string path, int &ex) {
        std::vector<std::vector<double>> _vect;
        std::ifstream fin(path);
        if (!fin.is_open())
            std::cout << "Error reading file" << path << std::endl;
        else
            std::cout << path << " loading...\n";
        std::string tmp;
        bool first;
        ex = 0;
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
            if (tmp != "") {
                _vect.push_back(_number);
                ex++;
            }
        }
        fin.close();
        printf("\nExamples = %d\n", ex);
        std::cout << "MNIST loaded... \n";
        return _vect;
    }

int main() {
        s21::Network NW{};
        s21::data_Network NW_config;
        double ra = 0;
        double right;
        double predict;
        double maxra = 0;
        int epoch = 0;
        int epoch_count;
        bool study;
        bool repeat = true;
        bool train_first = true;
        bool test_first = true;
        std::chrono::duration<double> time;
        std::vector<std::vector<double>> vect_train;
        std::vector<std::vector<double>> vect_test;
        std::vector<std::vector<double>> vect_;
        std::vector<std::vector<double>>::iterator it;
        std::vector<double>::iterator vIt;
        int examples;

        NW_config = ReadDataNetwork("src_data/Config.txt");
        NW.Init(NW_config);
        NW.PrintConfig();

        while (repeat) {
            std::cout << "Do you want to study? (1/0)" << std::endl;
            std::cin >> study;
            if (study) {
                std::cout << "Enter number of epoch (10 - 100):" << std::endl;
                std::cin >> epoch_count;
                if (train_first) {
                    vect_train = ReadData_sep("datasets/emnist-letters/emnist-letters-train.csv", examples);
                    train_first = false;
                }
                auto begin = std::chrono::steady_clock::now();
                while (ra / examples * 100 < 100) {
                    ra = 0;
                    auto t1 = std::chrono::steady_clock::now();
                    for (it=vect_train.begin(); it!=vect_train.end(); ++it) {
                        NW.SetInput(*it);
                        predict = NW.ForwardFeed();
                        right = *it->begin();
                        if (right != predict) {
                            NW.BackPropogation(right - 1);
                            NW.WeightsUpdater(2.1 * pow(2.1, -epoch / 10.));
                        } else
                            ra++;
                    }
                    auto t2 = std::chrono::steady_clock::now();
                    time = t2 - t1;
                    if (ra > maxra)
                        maxra = ra;
                    std::cout << "ra: " << ra / examples * 100 << "\t" << "maxra: " << maxra / examples * 100 << "\t"
                              << "epoch: " << epoch + 1 << "\tTIME: " << time.count() << std::endl;
                    epoch++;
                    if (epoch  % epoch_count == 0)
                        break;
                }
                auto end = std::chrono::steady_clock::now();
                time = end - begin;
                std::cout << "Time: " << time.count() / 60. << " min" << std::endl;
                NW.SaveWeights();
            } else
                NW.ReadWeights();
            std::cout << "Test? (1/0)\n";
            bool test_flag;
            std::cin >> test_flag;
            if (test_flag) {
                int ex_tests;
                if (test_first) {
                    vect_test = ReadData_sep("datasets/emnist-letters/emnist-letters-test.csv", ex_tests);
                    test_first = false;
                }
                ra = 0;
                for (it=vect_test.begin(); it!=vect_test.end(); ++it) {
                    NW.SetInput(*it);
                    predict = NW.ForwardFeed();
                    right = *it->begin();
                    if (right == predict)
                        ra++;
                }
                std::cout << "RA: " << ra / ex_tests * 100 << std::endl;
            }

/*
            std::cout << "Single test? (1/0)\n";
            bool s_test_flag;
            std::cin >> s_test_flag;
            if (s_test_flag) {
                int ex_tests;
                data_info *data_test;
                data_test = ReadData_sep("src_data/test_mnist.txt", NW_config, ex_tests);
                ra = 0;
                for (int i = 0; i < ex_tests; ++i) {
                    NW.SetInput(data_test[i].pixels);
                    predict = NW.ForwardFeed();
                    right = data_test[i].character;
                    if (right == predict)
                        ra++;
                    std::cout << "Reading number: " << right << std::endl << "Predict number: " << predict << std::endl
                              << std::endl;
                    delete data_test[i].pixels;
                }
                std::cout << "RA: " << ra / ex_tests * 100 << std::endl;
                delete[] data_test;
            }
*/
            std::cout << "Repeat? (1/0)\n";
            std::cin >> repeat;
        }
        NW.ClearLeaks();
        delete NW_config.size;
        return 0;
    }
