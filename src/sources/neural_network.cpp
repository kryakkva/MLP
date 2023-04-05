#include "../headers/neural_network.h"

#include <mach-o/dyld.h>
#include <sys/stat.h>
#include <unistd.h>

#include <QFileDialog>
#include <QPushButton>
#include <QtCore>

namespace s21 {
NeuralNetwork::NeuralNetwork(QObject *parent)
    : typeNet_(false), hidden_(2), epoch_(10), test_part_(1), break_(false) {
  initNet();
}

NeuralNetwork::~NeuralNetwork() { destroyNet(); }

void NeuralNetwork::initNet() {
  srand(time(NULL));
  max_ra_ = 0;
  counter_ = 0;
  if (!typeNet_) {
    layer_size_.push_back(784);
    for (int i = 0; i < hidden_; i++) layer_size_.push_back(150);
    layer_size_.push_back(26);
    weights_ = new Matrix[hidden_ + 1];
    bias_ = new double *[hidden_ + 1];
    for (int i = 0; i < hidden_ + 1; i++) {
      weights_[i].matrixInit(layer_size_[i + 1], layer_size_[i]);
      bias_[i] = new double[layer_size_[i + 1]];
      for (int j = 0; j < layer_size_[i + 1]; j++)
        bias_[i][j] = ((rand() % 50)) * 0.06 / (layer_size_[i] + 21);
    }
    neurons_val_ = new double *[hidden_ + 2];
    neurons_err_ = new double *[hidden_ + 2];
    for (int i = 0; i < hidden_ + 2; i++) {
      neurons_val_[i] = new double[layer_size_[i]];
      neurons_err_[i] = new double[layer_size_[i]];
    }
  } else {
    addLayer({{"type", layerType::INPUT}, {"size", 784}});
    for (int i = 0; i < hidden_; ++i) {
      addLayer({{"type", layerType::STANDARD}, {"size", 150}});
    }
    addLayer({{"type", layerType::OUTPUT}, {"size", 26}});
    autogenerate();
  }
  if (!rewrite_) emit isTrained(false);
}

void NeuralNetwork::reInitNet(int hiddenVal, bool typeNetVal) {
  if (typeNetVal != typeNet_ && hiddenVal == hidden_) reSaveStudy();
  destroyNet();
  hidden_ = hiddenVal;
  typeNet_ = typeNetVal;
  initNet();
  if (rewrite_) reLoadStudy();
}

void NeuralNetwork::reSaveStudy() {
  temp_counter_ = counter_;
  w_temp_.clear();
  b_temp_.clear();
  if (!typeNet_) {
    w_temp_.resize(layer_size_.size() - 1);
    b_temp_.resize(layer_size_.size() - 1);
    for (size_t i = 0; i < layer_size_.size() - 1; ++i) {
      w_temp_[i].resize(layer_size_[i]);
      for (int j = 0; j < layer_size_[i]; ++j) {
        w_temp_[i][j].resize(layer_size_[i + 1]);
        b_temp_[i].resize(layer_size_[i + 1]);
        for (int k = 0; k < layer_size_[i + 1]; k++) {
          w_temp_[i][j][k] = weights_[i](k, j);
          b_temp_[i][k] = bias_[i][k];
        }
      }
    }
  } else {
    w_temp_ = getWeights();
    b_temp_.resize(layers_.size() - 1);
    for (size_t i = 1; i < layers_.size(); ++i) {
      b_temp_[i - 1].resize(layers_[i]->neurons().size());
      for (size_t k = 0; k < layers_[i]->neurons().size(); ++k)
        b_temp_[i - 1][k] = layers_[i]->neurons()[k]->getBias();
    }
  }
  rewrite_ = true;
}

void NeuralNetwork::reLoadStudy() {
  counter_ = temp_counter_;
  if (!typeNet_) {
    for (size_t i = 0; i < layer_size_.size() - 1; ++i)
      for (int j = 0; j < layer_size_[i]; ++j)
        for (int k = 0; k < layer_size_[i + 1]; k++) {
          weights_[i](k, j) = w_temp_[i][j][k];
          bias_[i][k] = b_temp_[i][k];
        }
  } else {
    alterWeights(w_temp_);
    for (size_t i = 1; i < layers_.size(); ++i)
      for (size_t k = 0; k < layers_[i]->neurons().size(); ++k)
        layers_[i]->neurons()[k]->setBias(b_temp_[i - 1][k]);
  }
  rewrite_ = false;
}

void NeuralNetwork::setInput(std::vector<double> values, int fl) {
  if (!typeNet_) {
    for (size_t i = 0; i < (values.size() - fl); i++)
      neurons_val_[0][i] = values[i + fl];
  } else {
    clean();
    for (size_t i = 0; i < (values.size() - fl); ++i)
      layers_[0]->neurons()[i]->setAccumulated(values[i + fl]);
  }
}

void NeuralNetwork::networkTest(bool is_auto) {
  double ra = 0;
  double right;
  int predict;
  double recall;
  if (!is_auto) {
    break_ = false;
    if (test_part_) emit dialogMsg(test_);
    recall = test_part_;
  } else
    recall = 1;
  std::vector<std::vector<double>> value;
  if (is_auto && cross_val_)
    value = vector_test_cross_;
  else
    value = vector_test_;
  int j = 0;
  auto t1 = std::chrono::steady_clock::now();
  for (double i = 0; i < value.size() && !break_ && recall;
       i += 1 / recall, j++) {
    setInput(value[(int)i]);
    predict = predictLetter() - 64;
    right = value[(int)i][0];
    if (right == predict) ra++;
    if (!is_auto && !(j % (int)((value.size() * recall) / 100)) && !break_) {
      emit updateBar(j / (int)((value.size() * recall) / 100), test_);
    }
  }
  auto t2 = std::chrono::steady_clock::now();
  time_ = t2 - t1;
  if (!break_ && recall) {
    test_result_.clear();
    test_result_.push_back(100 * ra / (value.size() * recall));
    test_result_.push_back(1 - test_result_[0] / (100 - error_train_));
    test_result_.push_back(recall);
    test_result_.push_back(2 * (test_result_[1] * test_result_[2]) /
                           (test_result_[1] + test_result_[2]));
    test_result_.push_back(getTime().count());
    std::cout << "error " << 100 - test_result_[0] << std::endl;
    if (!is_auto) emit {
        showResult();
      }
  }
  if (is_auto && !break_) emit {
      updateChart(100 - test_result_[0]);
    }
  else
    emit { iAmReady(test_); }
}

void NeuralNetwork::networkTrain(bool b) {
  double ra;
  double right;
  double predict;
  int ep;
  break_ = false;
  error_train_ = 100;
  std::vector<std::vector<double>> value;
  if (!cross_val_) {
    value = vector_train_;
    ep = epoch_;
  } else {
    ep = cross_val_;
  }
  emit dialogMsg(train_);
  auto t1 = std::chrono::steady_clock::now();
  for (int j = 0; j < ep && !break_; j++) {
    auto t1 = std::chrono::steady_clock::now();
    ra = 0;
    if (cross_val_) {
      crossVal(j);
      value = vector_train_cross_;
    }
    for (int i = 0; i < value.size() && !break_; i++) {
      setInput(value[i]);
      predict = predictLetter() - 64;
      right = value[i][0];
      if (right != predict) {
        if (!typeNet_) {
          backPropagation(right);
          weightsUpdater(2.1 * pow(2.1, -counter_ / 10.));
        } else {
          getBackPropagationShifts(right);
          shiftBackWeights(2.1 * pow(2.1, -counter_ / 10.));
        }
      } else
        ra++;
      if (!(i % (value.size() / 100)) && !break_) {
        emit updateBar(i / (value.size() / 100), train_, j + 1);
      }
    }
    error_train_ = 100 - ra * 100 / value.size();
    auto t2 = std::chrono::steady_clock::now();
    time_ = t2 - t1;
    counter_++;
    networkTest(true);
    if (ra > max_ra_) {
      max_ra_ = ra;
    }
  }
  if (break_) counter_--;
  auto t2 = std::chrono::steady_clock::now();
  time_ = t2 - t1;
  emit iAmReady(train_);
  if (counter_) {
    emit isTrained(true);
    emit actChartBtn(true);
  }
}

int NeuralNetwork::forwardFeed() {
  for (int i = 1; i < hidden_ + 2; ++i) {
    Matrix::multi(weights_[i - 1], neurons_val_[i - 1], layer_size_[i - 1],
                  neurons_val_[i]);
    Matrix::sumVector(neurons_val_[i], bias_[i - 1], layer_size_[i]);
    func_.use(neurons_val_[i], layer_size_[i]);
  }
  int pred = searchMaxIndex(neurons_val_[hidden_ + 1]);
  return pred;
}

int NeuralNetwork::searchMaxIndex(double *value) {
  double max = value[0];
  int prediction = 1;
  double tmp;
  for (int i = 1; i < layer_size_[hidden_ + 1]; i++) {
    tmp = value[i];
    if (tmp > max) {
      prediction = i + 1;
      max = tmp;
    }
  }
  return prediction;
}

int NeuralNetwork::predictLetter() {
  if (!typeNet_)
    return (forwardFeed() + 64);
  else {
    trigger();
    return (searchMaxIndexGraph(output()) + 64);
  }
}

void NeuralNetwork::backPropagation(double expect) {
  for (int i = 0; i < layer_size_[hidden_ + 1]; i++) {
    if (i != int(expect - 1))
      neurons_err_[hidden_ + 1][i] = -neurons_val_[hidden_ + 1][i] *
                                     func_.useDer(neurons_val_[hidden_ + 1][i]);
    else
      neurons_err_[hidden_ + 1][i] = (1.0 - neurons_val_[hidden_ + 1][i]) *
                                     func_.useDer(neurons_val_[hidden_ + 1][i]);
  }
  for (int k = hidden_; k > 0; k--) {
    Matrix::multi_T(weights_[k], neurons_err_[k + 1], layer_size_[k + 1],
                    neurons_err_[k]);
    for (int j = 0; j < layer_size_[k]; j++)
      neurons_err_[k][j] *= func_.useDer(neurons_val_[k][j]);
  }
}

void NeuralNetwork::weightsUpdater(double lr) {
  for (int i = 0; i < hidden_ + 1; ++i)
    for (int j = 0; j < layer_size_[i + 1]; ++j)
      for (int k = 0; k < layer_size_[i]; ++k)
        weights_[i](j, k) += neurons_val_[i][k] * neurons_err_[i + 1][j] * lr;
  for (int i = 0; i < hidden_ + 1; i++)
    for (int j = 0; j < layer_size_[i + 1]; j++)
      bias_[i][j] += neurons_err_[i + 1][j] * lr;
}

void NeuralNetwork::trigger() {
  for (Layer *l : layers_) l->trigger();
}

int NeuralNetwork::searchMaxIndexGraph(std::vector<double> value) {
  double max = value[0];
  int prediction = 1;
  double tmp;
  for (std::size_t i = 1; i < value.size(); i++) {
    tmp = value[i];
    if (tmp > max) {
      prediction = i + 1;
      max = tmp;
    }
  }
  return prediction;
}

void NeuralNetwork::getBackPropagationShifts(double expect) {
  for (int i = layers_.size() - 1; i >= 1; --i)
    layers_[i]->getBackPropagationShifts(expect);
}

void NeuralNetwork::shiftBackWeights(double lr) {
  for (int i = layers_.size() - 1; i >= 1; --i)
    layers_[i]->shiftBackWeights(lr);
}

void NeuralNetwork::autogenerate() {
  connectComplete();
  randomizeAllWeightsBias();
}

void NeuralNetwork::connectComplete() {
  for (size_t i_layer = 0; i_layer < layers_.size() - 1; ++i_layer) {
    layers_[i_layer]->connectComplete(layers_[i_layer + 1]);
  }
}

void NeuralNetwork::randomizeAllWeightsBias() {
  for (size_t i_layer = 0; i_layer < layers_.size() - 1; ++i_layer)
    layers_[i_layer]->randomizeAllWeights(
        layers_[i_layer + 1]->neurons().size());
  for (size_t i_layer = 1; i_layer < layers_.size(); ++i_layer)
    layers_[i_layer]->randomizeAllBias(layers_[i_layer]->neurons().size());
}

void NeuralNetwork::addLayer(
    std::unordered_map<std::string, double> parameters) {
  layers_.push_back(new Layer(parameters));
}

void NeuralNetwork::clean() {
  for (Layer *l : layers_) l->clean();
}

std::vector<double> NeuralNetwork::output() {
  return (layers_.back())->output();
}

std::vector<std::vector<std::vector<double>>> NeuralNetwork::getWeights() {
  std::vector<std::vector<std::vector<double>>> w;
  for (size_t i_layer = 0; i_layer < layers_.size() - 1; ++i_layer)
    w.push_back(layers_[i_layer]->getWeights());
  return w;
}

void NeuralNetwork::alterWeights(
    const std::vector<std::vector<std::vector<double>>> &weights) {
  for (size_t i_layer = 0; i_layer < layers_.size() - 1; ++i_layer)
    layers_[i_layer]->alterWeights(weights[i_layer]);
}

void NeuralNetwork::saveWeights(std::string filename) {
  std::ofstream fout;
  fout.open(filename);
  if (!fout.is_open())
    throw std::invalid_argument(std::string("Error reading file"));
  fout << "This is weights file" << std::endl;
  fout << " " << counter_ << " " << error_train_ << std::endl;
  if (!typeNet_) {
    for (int i = 0; i < hidden_ + 2; ++i) fout << " " << layer_size_[i];
    fout << std::endl;
    for (int i = 0; i < hidden_ + 1; ++i) fout << weights_[i] << " ";
    for (int i = 0; i < hidden_ + 1; ++i)
      for (int j = 0; j < layer_size_[i + 1]; ++j) fout << bias_[i][j] << " ";
  } else {
    for (int i = 0; i < hidden_ + 2; ++i)
      fout << " " << layers_[i]->neurons().size();
    fout << std::endl;
    std::vector<std::vector<std::vector<double>>> w;
    w = getWeights();
    for (size_t i_layer = 0; i_layer < layers_.size() - 1; ++i_layer)
      for (size_t i_neuron = 0; i_neuron < layers_[i_layer]->neurons().size();
           ++i_neuron)
        for (size_t i_edge = 0;
             i_edge <
             layers_[i_layer]->neurons()[i_neuron]->getNextEdge().size();
             ++i_edge)
          fout << w[i_layer][i_neuron][i_edge] << " ";
    for (size_t i_layer = 1; i_layer < layers_.size(); ++i_layer)
      for (size_t i_neuron = 0; i_neuron < layers_[i_layer]->neurons().size();
           ++i_neuron)
        fout << layers_[i_layer]->neurons()[i_neuron]->getBias() << " ";
  }
  fout.close();
}

void NeuralNetwork::readData(std::string filename, mStatus v) {
  break_ = false;
  emit dialogMsg(read_, filename);
  int size = -1;
  std::ifstream fin(filename);
  if (!fin.is_open())
    throw std::invalid_argument(std::string("Error reading file"));
  std::string tmp;
  while (!fin.eof() && !break_) {
    std::getline(fin, tmp);
    size++;
  }
  std::cout << size << std::endl;
  fin.clear();
  fin.seekg(0, std::ios::beg);
  std::vector<std::vector<double>> _vector;
  int i = 0;
  std::string line;
  bool first;
  while (!fin.eof() && !break_) {
    std::getline(fin, tmp);
    std::istringstream input;
    input.str(tmp.c_str());
    first = true;
    std::vector<double> _number;
    for (std::string num; std::getline(input, num, ',') && !break_;) {
      if (first) {
        _number.push_back(stoi(num));
        first = false;
      } else
        _number.push_back(stoi(num) / 255.);
    }
    if (tmp != "") _vector.push_back(_number);
    if (!(++i % (size / 100)) && !break_)
      emit updateBar(i / (size / 100), test_);
  }
  fin.close();
  emit iAmReady(read_);
  if (v == train_ && !break_)
    vector_train_ = _vector;
  else if (v == test_ && !break_)
    vector_test_ = _vector;
  if (!vector_test_.empty()) emit actTestBtn(true);
  if (!vector_train_.empty() && (cross_val_ || !vector_test_.empty())) {
    emit actTrainBtn(true);
  }
}

void NeuralNetwork::readWeights(std::string filename) {
  std::ifstream fin;
  fin.open(filename);
  if (!fin.is_open())
    throw std::invalid_argument(std::string("Error reading file"));
  std::string test;
  std::getline(fin, test);
  if (test.compare("This is weights file") != 0) {
    printf("You open wrong file!!!\n");
    emit wrongFile();
    return;
  }
  double num;
  while (fin.get() != '\n') {
    fin >> num;
    if (num - (int)num != 0) {
      printf("Wrong in file 1\n");
      emit wrongFile();
      return;
    }
    counter_ = num;
    fin >> error_train_;
  }
  std::vector<int> line;
  while (fin.get() != '\n') {
    fin >> num;
    if (num - (int)num != 0) {
      printf("Wrong in file 2\n");
      emit wrongFile();
      return;
    }
    line.push_back(static_cast<int>(num));
  }

  if (hidden_ + 2 != (int)line.size()) {
    printf("Wrong in file 3\n");
    emit wrongFile();
    return;
  }
  int multi = 0;
  int count = 0;
  if (!typeNet_) {
    for (int x = 0; x < hidden_ + 1; ++x) {
      multi += (line[x] * line[x + 1]);
      for (int i = 0; i < weights_[x].getCol(); ++i)
        for (int j = 0; j < weights_[x].getRow(); ++j, count++)
          fin >> weights_[x](j, i);
    }
    for (int i = 0; i < hidden_ + 1; ++i) {
      multi += line[i + 1];
      for (int j = 0; j < layer_size_[i + 1]; ++j) {
        fin >> bias_[i][j];
        if (!fin.eof()) count++;
      }
    }
  } else {
    std::vector<std::vector<std::vector<double>>> w(layers_.size());
    for (size_t i_layer = 0; i_layer < layers_.size() - 1; ++i_layer) {
      multi += (line[i_layer] * line[i_layer + 1]);
      w[i_layer].resize(layers_[i_layer]->neurons().size());
      for (size_t i_neuron = 0; i_neuron < layers_[i_layer]->neurons().size();
           ++i_neuron) {
        w[i_layer][i_neuron].resize(
            layers_[i_layer]->neurons()[i_neuron]->getNextEdge().size());
        for (size_t i_edge = 0;
             i_edge <
             layers_[i_layer]->neurons()[i_neuron]->getNextEdge().size();
             ++i_edge, count++)
          fin >> w[i_layer][i_neuron][i_edge];
      }
    }
    alterWeights(w);
    for (size_t i_layer = 1; i_layer < layers_.size(); ++i_layer) {
      multi += line[i_layer];
      for (size_t i_neuron = 0; i_neuron < layers_[i_layer]->neurons().size();
           ++i_neuron) {
        double tmp;
        fin >> tmp;
        layers_[i_layer]->neurons()[i_neuron]->setBias(tmp);
        if (!fin.eof()) count++;
      }
    }
  }
  double temp;
  fin >> temp;
  if (!fin.eof()) count++;
  if (count != multi) {
    printf("Amount of number is wrong!!!\n");
    emit wrongFile();
    return;
  }
  fin.close();
  emit isTrained(true);
}

void NeuralNetwork::destroyNet() {
  if (!typeNet_) {
    for (int i = 0; i < hidden_ + 2; i++) {
      delete[] neurons_val_[i];
      delete[] neurons_err_[i];
    }
    delete[] neurons_val_;
    delete[] neurons_err_;
    for (int i = 0; i < hidden_ + 1; i++) delete[] bias_[i];
    delete[] bias_;
    for (int i = 0; i < hidden_ + 1; i++) weights_[i].clear(layer_size_[i + 1]);
    delete[] weights_;
    layer_size_.clear();
  } else {
    for (Layer *l : layers_) l->clearAll();
    layers_.clear();
  }
}

void NeuralNetwork::crossVal(int e) {
  if (!cross_val_) {
    if (!e) {
      vector_train_cross_.clear();
      vector_test_cross_.clear();
      for (size_t i = 0; i < vector_test_.size(); ++i)
        vector_test_cross_.push_back(vector_test_[i]);
      for (size_t i = 0; i < vector_train_.size(); ++i)
        vector_train_cross_.push_back(vector_train_[i]);
    }
  } else {
    vector_train_cross_.clear();
    vector_test_cross_.clear();
    double coeff = 1 - ((cross_val_ - 1.) / cross_val_);
    size_t tmp = 0;
    size_t tmp_1 = 0;
    if (cross_val_ == 1) {
      for (size_t i = 0; i < vector_train_.size(); ++i)
        vector_train_cross_.push_back(vector_train_[i]);
    } else {
      for (size_t i = 0;
           i < (size_t)(vector_train_.size() * (cross_val_ - 1 - e) * coeff);
           ++i, tmp_1 = i)
        vector_train_cross_.push_back(vector_train_[i]);
      for (size_t i =
               (size_t)(vector_train_.size() * (cross_val_ - 1 - e) * coeff);
           i < (size_t)(vector_train_.size() * (cross_val_ - e) * coeff);
           ++i, tmp = i)
        vector_test_cross_.push_back(vector_train_[i]);
      for (size_t i = (size_t)(vector_train_.size() * (cross_val_ - e) * coeff);
           i < vector_train_.size(); ++i)
        vector_train_cross_.push_back(vector_train_[i]);
    }
  }
}

void NeuralNetwork::setCrossVal(int c) { cross_val_ = c; }

int NeuralNetwork::getEpoch() { return epoch_; }

int NeuralNetwork::getCrossVal() { return cross_val_; }

int NeuralNetwork::getLayer() { return hidden_; }

bool NeuralNetwork::getTypeNet() { return typeNet_; }

void NeuralNetwork::setEpoch(int n) {
  epoch_ = n;
  std::cout << "Set " << epoch_ << " epochs\n";
}

void NeuralNetwork::setBreak(bool b) { break_ = b; }

void NeuralNetwork::setTestPart(int i) { test_part_ = i / 100.; }

std::chrono::duration<double> NeuralNetwork::getTime() { return time_; }

int NeuralNetwork::getCounter() const { return counter_; }

std::vector<double> NeuralNetwork::getResult() { return test_result_; }
double NeuralNetwork::getErrorTrain() const { return error_train_; }

}  // namespace s21
