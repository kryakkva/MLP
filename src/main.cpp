//
// Created by apaege on 17.02.2023.
//

#include "headers/NeuralNetwork.h"

using namespace s21;

int main() {
  double ra = 0;
  std::chrono::duration<double> time;

  NeuralNetwork net;

  if (net._vector_train.empty())
    net._vector_train =
        net.readData("../datasets/emnist-letters/emnist-letters-train.csv");
  size_t ex_train = net._vector_train.size();

  net.setEpoch(5);
  net._typeNetVal = 1;
  net.reInitNet();

  auto begin = std::chrono::steady_clock::now();
  for (int i = 0; i < net.getEpoch(); ++i) {
    ra = net.networkTrain(net._vector_train);
    std::cout << "Train RA: " << ra / ex_train * 100
              << "\tmaxra: " << net.getMaxRa() / ex_train * 100
              << "\tepoch: " << i + 1 << "\tTIME: " << net.getTime().count()
              << std::endl;
  }
  auto end = std::chrono::steady_clock::now();
  time = end - begin;
  std::cout << std::endl
            << "TIME: " << time.count() / 60. << "min" << std::endl;

  net.saveWeights("../src_data/new_weights/2layers.mlp");
  //    net._typeNetVal = 1;
  //    net.reInitNet();
  //    net.readWeights("../src_data/new_weights/2layers.mlp");

  //    net._typeNetVal = 0;
  //    net.reInitNet();

  std::cout << "_TypeNet = " << net.getTypeNet()
            << " _hidden = " << net.getLayer() << std::endl;
  if (net._vector_test.empty())
    net._vector_test =
        net.readData("../datasets/emnist-letters/emnist-letters-test.csv");
  size_t ex_tests = net._vector_test.size();
  auto begin2 = std::chrono::steady_clock::now();
  ra = net.networkTest(net._vector_test);
  auto end2 = std::chrono::steady_clock::now();
  time = end2 - begin2;
  std::cout << "_TypeNet = " << net.getTypeNet()
            << " _hidden = " << net.getLayer()
            << " Test RA: " << ra / ex_tests * 100 << std::endl;
  std::cout << std::endl
            << "TIME: " << time.count() / 60. << "min" << std::endl
            << std::endl;

  /*
      net._typeNetVal = 1;
      net.reInitNet();

      if (net._vector_test.empty())
          net._vector_test =
     net.readData("datasets/emnist-letters/emnist-letters-test.csv"); auto
     begin_3 = std::chrono::steady_clock::now(); ra =
     net.networkTest(net._vector_test); auto end_3 =
     std::chrono::steady_clock::now(); time = end_3 - begin_3; std::cout <<
     "_TypeNet = " << net.getTypeNet() << " _hidden = " << net.getLayer() << "
     Test RA: " << ra / ex_tests * 100 << std::endl; std::cout << std::endl <<
     "TIME: " << time.count() / 60. << "min" << std::endl << std::endl;
  */

  /*
      net._typeNetVal = 1;
  //    net._hiddenVal = 4;
      net.reInitNet();

      if (net._vector_train.empty())
          net._vector_train =
  net.readData("datasets/emnist-letters/emnist-letters-train.csv");

      auto begin_2 = std::chrono::steady_clock::now();
      for (int i = 0; i < net.getEpoch(); ++i) {
          ra = net.networkTrain(net._vector_train);
          std::cout << "Train RA: " << ra / ex_train * 100 << "\t" << "maxra: "
  << net.getMaxRa() / ex_train * 100 << "\t"
                    << "epoch: " << i + 1 << "\tTIME: " << net.getTime().count()
  << std::endl;
      }
      auto end_2 = std::chrono::steady_clock::now();
      time = end_2 - begin_2;
      std::cout << std::endl << "TIME: " << time.count() / 60. << "min" <<
  std::endl;

  //    net.readWeights_M("./src_data/Weights_2.txt");

      std::cout << "_TypeNet = " << net.getTypeNet() << " _hidden = " <<
  net.getLayer() << std::endl; if (net._vector_test.empty()) net._vector_test =
  net.readData("datasets/emnist-letters/emnist-letters-test.csv"); auto begin3 =
  std::chrono::steady_clock::now(); ra = net.networkTest(net._vector_test); auto
  end3 = std::chrono::steady_clock::now(); time = end3 - begin3; std::cout <<
  "_TypeNet = " << net.getTypeNet() << " _hidden = " << net.getLayer() << " Test
  RA: " << ra / ex_tests * 100 << std::endl; std::cout << std::endl << "TIME: "
  << time.count() / 60. << "min" << std::endl << std::endl;
  */
}
