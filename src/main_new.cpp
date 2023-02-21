//
// Created by apaege on 17.02.2023.
//

#include "headers/Network.h"

using namespace s21;

int main()
{
    double ra;
    double maxra = 0;
    auto begin = std::chrono::steady_clock::now();
    std::chrono::duration<double> time;

    Network net;

    net.InitMatrixNet();

    net._vector_train = net.ReadData("../datasets/emnist-letters/emnist-letters-train.csv");
    size_t ex_train = net._vector_train.size();

    for (int i = 0; i < net.getEpoch(); ++i) {
        auto t1 = std::chrono::steady_clock::now();
        ra = net.NetworkTrain(net._vector_train);
        if (ra > maxra)
            maxra = ra;
        auto t2 = std::chrono::steady_clock::now();
        time = t2 - t1;
        std::cout << "Train RA: " << ra / ex_train * 100 << "\t" << "maxra: " << maxra / ex_train * 100 << "\t"
                << "epoch: " << i + 1 << "\tTIME: " << time.count() << std::endl;
    }
    std::cout << std::endl << "MAXRA: " << maxra / ex_train * 100 << std::endl;

//    net.ReadWeights_M("./src_data/Weights_2.txt");

    net._vector_test = net.ReadData("../datasets/emnist-letters/emnist-letters-test.csv");
    size_t ex_tests = net._vector_test.size();
    ra = net.NetworkTest(net._vector_test);
    std::cout << "TEst RA: " << ra / ex_tests * 100 << std::endl;

}
