//
// Created by apaege on 17.02.2023.
//

#include "headers/NeuralNetwork.h"

using namespace s21;

int main()
{
    double ra;
    std::chrono::duration<double> time;

    Network net;

    if (net._vector_train.empty())
        net._vector_train = net.ReadData("../datasets/emnist-letters/emnist-letters-train.csv");
    size_t ex_train = net._vector_train.size();

    net.setEpoch(3);

    auto begin = std::chrono::steady_clock::now();
    for (int i = 0; i < net.getEpoch(); ++i) {
        ra = net.NetworkTrain(net._vector_train);
        std::cout << "Train RA: " << ra / ex_train * 100 << "\t" << "maxra: " << net.getMaxRa() / ex_train * 100 << "\t"
                << "epoch: " << i + 1 << "\tTIME: " << net.getTime().count() << std::endl;
    }
    auto end = std::chrono::steady_clock::now();
    time = end - begin;
    std::cout << std::endl << "TIME: " << time.count() / 60. << "min" << std::endl;

//    net.ReadWeights_M("./src_data/Weights_2.txt");

    if (net._vector_test.empty())
        net._vector_test = net.ReadData("../datasets/emnist-letters/emnist-letters-test.csv");
    size_t ex_tests = net._vector_test.size();
    ra = net.NetworkTest(net._vector_test);
    std::cout << "Test RA: " << ra / ex_tests * 100 << std::endl << std::endl;

    net.reInitNet(5);

    if (net._vector_train.empty())
        net._vector_train = net.ReadData("../datasets/emnist-letters/emnist-letters-train.csv");

    auto begin_2 = std::chrono::steady_clock::now();
    for (int i = 0; i < net.getEpoch(); ++i) {
        ra = net.NetworkTrain(net._vector_train);
        std::cout << "Train RA: " << ra / ex_train * 100 << "\t" << "maxra: " << net.getMaxRa() / ex_train * 100 << "\t"
                  << "epoch: " << i + 1 << "\tTIME: " << net.getTime().count() << std::endl;
    }
    auto end_2 = std::chrono::steady_clock::now();
    time = end_2 - begin_2;
    std::cout << std::endl << "TIME: " << time.count() / 60. << "min" << std::endl;

//    net.ReadWeights_M("./src_data/Weights_2.txt");

    if (net._vector_test.empty())
        net._vector_test = net.ReadData("../datasets/emnist-letters/emnist-letters-test.csv");
    ra = net.NetworkTest(net._vector_test);
    std::cout << "Test RA: " << ra / ex_tests * 100 << std::endl << std::endl;

}
