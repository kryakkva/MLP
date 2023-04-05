#ifndef NETWORK_H
#define NETWORK_H

#include <QMessageBox>
#include <QWidget>
#include <fstream>
#include <sstream>
#include <vector>

#include "layer.h"
#include "matrix.h"

namespace s21 {
class NeuralNetwork;
class Neuron;
class Layer;

enum mStatus {
  test_ = 0,
  train_,
  read_,
};
class NeuralNetwork : public QObject {
  Q_OBJECT
 private:
  bool typeNet_;
  int hidden_;
  int epoch_;
  int counter_;
  int temp_counter_;
  int cross_val_ = 0;
  double error_train_ = 0;

 public:
  double getErrorTrain() const;

 private:
  std::vector<double> test_result_;
  double max_ra_;
  std::vector<int> layer_size_;
  Function func_;
  Matrix *weights_;
  double **bias_;
  double **neurons_val_, **neurons_err_;
  std::chrono::duration<double> time_;
  std::vector<Layer *> layers_;
  std::vector<std::unordered_map<std::string, double>> configuration_;
  bool break_;
  double test_part_;
  std::vector<std::vector<std::vector<double>>> w_temp_;
  std::vector<std::vector<double>> b_temp_;
  bool rewrite_ = false;
  void crossVal(int e);
  void initNet();
 private slots:
  void setCrossVal(int c);
  void networkTrain(bool b);
  void networkTest(bool is_auto);
  void setTestPart(int i);
  void setEpoch(int n);
  void readData(std::string filename, mStatus v);
 signals:
  void updateBar(int i, mStatus stat, int e = 0);
  void dialogMsg(mStatus status, std::string str = "");
  void actTrainBtn(bool);
  void actTestBtn(bool);
  void actChartBtn(bool);
  void iAmReady(mStatus status);
  void updateChart(double);
  void wrongFile();
  void isTrained(bool);
  void showResult();

 public:
  std::vector<std::vector<double>> vector_train_;
  std::vector<std::vector<double>> vector_test_;
  std::vector<std::vector<double>> vector_train_cross_;
  std::vector<std::vector<double>> vector_test_cross_;
  NeuralNetwork(QObject *parent = nullptr);

  ~NeuralNetwork();
  void setBreak(bool b);
  void reInitNet(int l, bool b);
  void reSaveStudy();
  void reLoadStudy();
  void setInput(std::vector<double> values, int fl = 1);
  int forwardFeed();
  int searchMaxIndex(double *value);
  void backPropagation(double expect);
  void weightsUpdater(double lr);
  void trigger();
  int predictLetter();
  int searchMaxIndexGraph(std::vector<double> values);
  void getBackPropagationShifts(double expect);
  void shiftBackWeights(double lr);
  void autogenerate();
  void connectComplete();
  void randomizeAllWeightsBias();
  void addLayer(std::unordered_map<std::string, double> parameters);
  void clean();
  std::vector<double> output();
  std::vector<std::vector<std::vector<double>>> getWeights();
  void alterWeights(
      const std::vector<std::vector<std::vector<double>>> &weights);
  void saveWeights(std::string filename);
  void readWeights(std::string filename);
  void destroyNet();
  int getEpoch();
  int getCrossVal();
  bool getTypeNet();
  std::chrono::duration<double> getTime();
  int getLayer();
  int getCounter() const;
  std::vector<double> getResult();
};
}  // namespace s21

#endif  // NETWORK_H
