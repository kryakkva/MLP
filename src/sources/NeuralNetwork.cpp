//
// Created by apaege on 08.01.2023.
//

#include "../headers/NeuralNetwork.h"
#include <QPushButton>
#include <sys/stat.h>
#include <QFileDialog>
#include <mach-o/dyld.h>
#include <QtCore>

namespace s21 {
NeuralNetwork::NeuralNetwork(QObject *parent) : _typeNet(0), _hidden(2), _epoch(10), test_part_(1), break_(false){
  initNet();
}

NeuralNetwork::~NeuralNetwork() { destroyNet(); }

void NeuralNetwork::initNet() {
  srand(time(NULL));
  _hiddenVal = _hidden;
  _typeNetVal = _typeNet;
  _maxRa = 0;
  _counter = 0;
  if (!_typeNet) {
    _layerSize.push_back(784);
    for (int i = 0; i < _hidden; i++) _layerSize.push_back(150);
    _layerSize.push_back(26);
    _weights = new Matrix[_hidden + 1];
    _bias = new double *[_hidden + 1];
    for (int i = 0; i < _hidden + 1; i++) {
      _weights[i].matrixInit(_layerSize[i + 1], _layerSize[i]);
      _bias[i] = new double[_layerSize[i + 1]];
      for (int j = 0; j < _layerSize[i + 1]; j++)
        _bias[i][j] = ((rand() % 50)) * 0.06 / (_layerSize[i] + 21);
    }
    _neurons_val = new double *[_hidden + 2];
    _neurons_err = new double *[_hidden + 2];
    for (int i = 0; i < _hidden + 2; i++) {
      _neurons_val[i] = new double[_layerSize[i]];
      _neurons_err[i] = new double[_layerSize[i]];
    }
  } else {
    addLayer({{"type", layerType::INPUT}, {"size", 784}});
    for (int i = 0; i < _hidden; ++i)
      addLayer({{"type", layerType::STANDARD}, {"size", 150}});
    addLayer({{"type", layerType::OUTPUT}, {"size", 26}});
    autogenerate();
  }
  emit isTrained(false);
  // std::cout << "init net done\n" ;
}

void NeuralNetwork::reInitNet(int l) {
  destroyNet();
  _hidden = l;
  // setLayer(l);
  initNet();
}

void NeuralNetwork::setInput(std::vector<double> values, int fl) {
  if (!_typeNet) {
    for (size_t i = 0; i < (values.size() - fl); i++)
      _neurons_val[0][i] = values[i + fl];
  } else {
    clean();
    for (size_t i = 0; i < (values.size() - fl); ++i)
      _layers[0]->neurons()[i]->setAccumulated(values[i + fl]);
  }
}
/*
double NeuralNetwork::NetworkTest(std::vector<std::vector<double>> value) {
  double ra = 0;
  double right;
  int predict;
  for (int i = 0; i < value.size(); ++i) {
    SetInput(value[i]);
    predict = ForwardFeed();
    right = value[i][0];
    if (right == predict)
      ra++;
  }
  return (ra);
}
*/

void NeuralNetwork::networkTest(bool is_auto) {
  double ra = 0;
  double right;
  int predict;
  if (!is_auto){
    break_ = false;
    emit dialogMsg(test_);
  }
  int j = 0;
  for (double i = 0; i < _vector_test.size() && !break_; i += 1/test_part_, j++) {
    setInput(_vector_test[(int)i]);
    if (!_typeNet)
      predict = forwardFeed();
    else {
      std::cout << "1";
      trigger();
      std::cout << "2";
      predict = searchMaxIndexGraph(output());
      std::cout << "3";
    }
    right = _vector_test[(int)i][0];
    if (right == predict)
      ra++;
    if (!is_auto && !(j % (int)((_vector_test.size()*test_part_)/100)) && !break_){
      emit updateBar(j / (int)((_vector_test.size()*test_part_)/100), test_);
    }
  }
  // qInfo() << j;
  if (is_auto && !break_)
    emit updateChart(100 - (ra * 100 / _vector_test.size()));
  else
    emit iAmReady(test_);
  std::cout << "error " << 100 - ra * 100 / (_vector_test.size()*test_part_)<< std::endl;
}

/*
  double NeuralNetwork::NetworkTrain(std::vector<std::vector<double>> value) {
      double ra = 0;
      double right;
      int predict;

      auto t1 = std::chrono::steady_clock::now();
      for (int i = 0; i < value.size(); ++i) {
          SetInput(value[i]);
          predict = ForwardFeed();
          right = value[i][0];
          if (right != predict) {
              BackPropogation(right);
              WeightsUpdater(2.1 * pow(2.1, -_counter / 10.));
          }
          else
              ra++;
      }
      auto t2 = std::chrono::steady_clock::now();
      _time = t2 - t1;
      _counter++;
      if (ra > _maxRa)
          _maxRa = ra;
      return (ra);
  }
*/
void NeuralNetwork::networkTrain(bool b) {
  double ra;
  double right;
  double predict;
  break_ = false;
  emit dialogMsg(train_);
  auto t1 = std::chrono::steady_clock::now();
  for (int j = 0; j < _epoch && !break_; j++) {
    auto t1 = std::chrono::steady_clock::now();
    ra = 0;
    for (int i = 0; i < _vector_train.size() && !break_; i++) {
      setInput(_vector_train[i]);
      if (!_typeNet) {
        predict = forwardFeed();
      }
      else {
        trigger();
        predict = searchMaxIndexGraph(output());
      }
      right = _vector_train[i][0];
      if (right != predict) {
        if (!_typeNet) {
          backPropagation(right);
          weightsUpdater(2.1 * pow(2.1, -_counter / 10.));
        } else {
          getBackPropagationShifts(right);
          shiftBackWeights(2.1 * pow(2.1, -_counter / 10.));
        }
      } else
        ra++;
      if (!(i % (_vector_train.size()/100)) && !break_){
        emit updateBar(i / (_vector_train.size()/100), train_, j + 1);
      }
    }
    auto t2 = std::chrono::steady_clock::now();
    _time = t2 - t1;
    _counter++;
    std::cout << _counter << " Epoch | "  << 100 - (ra * 100 / 88800) << " % Error | " << getTime().count() << " sec | Test ";
    // std::thread _th([&](){NetworkTest();});
    // _th.detach();
    networkTest(true);
    // NetworkTest();
    if (ra > _maxRa) {
      _maxRa = ra;
    }
  }
  if (break_)
    _counter--;
  auto t2 = std::chrono::steady_clock::now();
  _time = t2 - t1;
  std::cout << "all time: " << getTime().count() << std::endl;
  emit iAmReady(train_);
  if (_counter){
    emit isTrained(true, _counter);
    emit actChartBtn(true);
  }
}

int NeuralNetwork::forwardFeed() {
  for (int i = 1; i < _hidden + 2; ++i) {
    Matrix::multi(_weights[i - 1], _neurons_val[i - 1], _layerSize[i - 1],
                  _neurons_val[i]);
    Matrix::sumVector(_neurons_val[i], _bias[i - 1], _layerSize[i]);
    _func.use(_neurons_val[i], _layerSize[i]);
  }
  int pred = searchMaxIndex(_neurons_val[_hidden + 1]);
  return pred;
}

int NeuralNetwork::searchMaxIndex(double *value) {
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

void NeuralNetwork::backPropagation(double expect) {
  for (int i = 0; i < _layerSize[_hidden + 1]; i++) {
    if (i != int(expect - 1))
      _neurons_err[_hidden + 1][i] = -_neurons_val[_hidden + 1][i] *
          _func.useDer(_neurons_val[_hidden + 1][i]);
    else
      _neurons_err[_hidden + 1][i] = (1.0 - _neurons_val[_hidden + 1][i]) *
          _func.useDer(_neurons_val[_hidden + 1][i]);
  }
  for (int k = _hidden; k > 0; k--) {
    Matrix::multi_T(_weights[k], _neurons_err[k + 1], _layerSize[k + 1],
                    _neurons_err[k]);
    for (int j = 0; j < _layerSize[k]; j++)
      _neurons_err[k][j] *= _func.useDer(_neurons_val[k][j]);
  }
}

void NeuralNetwork::weightsUpdater(double lr) {
  for (int i = 0; i < _hidden + 1; ++i)
    for (int j = 0; j < _layerSize[i + 1]; ++j)
      for (int k = 0; k < _layerSize[i]; ++k)
        _weights[i](j, k) += _neurons_val[i][k] * _neurons_err[i + 1][j] * lr;
  for (int i = 0; i < _hidden + 1; i++)
    for (int j = 0; j < _layerSize[i + 1]; j++)
      _bias[i][j] += _neurons_err[i + 1][j] * lr;
}

void NeuralNetwork::trigger() {
  for (Layer *l : _layers) l->trigger();
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
  for (int i = _layers.size() - 1; i >= 1; --i)
    _layers[i]->getBackPropagationShifts(expect);
}

void NeuralNetwork::shiftBackWeights(double lr) {
  for (int i = _layers.size() - 1; i >= 1; --i)
    _layers[i]->shiftBackWeights(lr);
}

void NeuralNetwork::autogenerate() {
  connectComplete();
  randomizeAllWeightsBias();
}

void NeuralNetwork::connectComplete() {
  for (size_t i_layer = 0; i_layer < _layers.size() - 1; ++i_layer)
    _layers[i_layer]->connectComplete(_layers[i_layer + 1]);
}

void NeuralNetwork::randomizeAllWeightsBias() {
  for (size_t i_layer = 0; i_layer < _layers.size() - 1; ++i_layer)
    _layers[i_layer]->randomizeAllWeights(
        _layers[i_layer + 1]->neurons().size());
  for (size_t i_layer = 1; i_layer < _layers.size(); ++i_layer)
    _layers[i_layer]->randomizeAllBias(_layers[i_layer]->neurons().size());
}

void NeuralNetwork::addLayer(
    std::unordered_map<std::string, double> parameters) {
  _layers.push_back(new Layer(parameters));
}

void NeuralNetwork::clean() {
  for (Layer *l : _layers) l->clean();
}

std::vector<double> NeuralNetwork::output() {
  return (_layers.back())->output();
}

std::vector<std::vector<std::vector<double *>>> NeuralNetwork::getWeights() {
  std::vector<std::vector<std::vector<double *>>> w;
  for (size_t i_layer = 0; i_layer < _layers.size() - 1; ++i_layer)
    w.push_back(_layers[i_layer]->getWeights());
  return w;
}

void NeuralNetwork::alterWeights(
    const std::vector<std::vector<std::vector<double>>> &weights) {
  for (size_t i_layer = 0; i_layer < _layers.size() - 1; ++i_layer)
    _layers[i_layer]->alterWeights(weights[i_layer]);
}

void NeuralNetwork::saveWeights(std::string filename) {
  std::ofstream fout;
  fout.open(filename);
  if (!fout.is_open()) {
    std::cout << "Error reading file " << filename << std::endl;
    exit(0);
  }
  fout << "This is weights file" << std::endl;
  if (!_typeNet) {
    for (int i = 0; i < _hidden + 2; ++i) fout << " " << _layerSize[i];
    fout << std::endl;
    for (int i = 0; i < _hidden + 1; ++i) fout << _weights[i] << " ";
    for (int i = 0; i < _hidden + 1; ++i)
      for (int j = 0; j < _layerSize[i + 1]; ++j) fout << _bias[i][j] << " ";
  } else {
    for (int i = 0; i < _hidden + 2; ++i)
      fout << " " << _layers[i]->neurons().size();
    fout << std::endl;
    std::vector<std::vector<std::vector<double *>>> w;
    w = getWeights();
    for (size_t i_layer = 0; i_layer < _layers.size() - 1; ++i_layer)
      for (size_t i_neuron = 0; i_neuron < _layers[i_layer]->neurons().size();
           ++i_neuron)
        for (size_t i_edge = 0;
             i_edge <
                 _layers[i_layer]->neurons()[i_neuron]->getNextEdge().size();
             ++i_edge)
          fout << *w[i_layer][i_neuron][i_edge] << " ";
    for (size_t i_layer = 1; i_layer < _layers.size(); ++i_layer)
      for (size_t i_neuron = 0; i_neuron < _layers[i_layer]->neurons().size();
           ++i_neuron)
        fout << _layers[i_layer]->neurons()[i_neuron]->getBias() << " ";
  }
  std::cout << filename << " saved\n";
  fout.close();
}

void NeuralNetwork::readWeights(std::string filename) {
  std::ifstream           fin;
  fin.open(filename);
  if (!fin.is_open()) {
    std::cout << "Error reading file " << filename << std::endl;
    exit(0);
  }
  std::string test;
  std::getline(fin, test);
  if (test.compare("This is weights file") != 0) {
    printf("You open wrong file!!!\n");
    emit wrongFile();
    return;
  }
  std::vector<int> line;
  double num;
  while (fin.get() != '\n') {
    fin >> num;
    if (num - (int)num != 0) {
      printf("Wrong in file 2\n");
      emit wrongFile();
      return;
    }
    line.push_back(static_cast<int>(num));
  }
  if (_hidden + 2 != line.size()) {
    printf("Wrong in file 3\n");
    emit wrongFile();
    return;
  }
  int multi = 0;
  int count = 0;
  if (!_typeNet) {
    for (int x = 0; x < _hidden + 1; ++x) {
      multi += (line[x] * line[x + 1]);
      for (int i = 0; i < _weights[x].getRow(); ++i)
        for (int j = 0; j < _weights[x].getCol(); ++j, count++)
          fin >> _weights[x](i, j);
    }
    for (int i = 0; i < _hidden + 1; ++i) {
      multi += line[i + 1];
      for (int j = 0; j < _layerSize[i + 1]; ++j) {
        fin >> _bias[i][j];
        if (!fin.eof())
          count++;
      }
    }
  } else {
    std::vector<std::vector<std::vector<double>>> w(_layers.size());
    for (size_t i_layer = 0; i_layer < _layers.size() - 1; ++i_layer) {
      multi += (line[i_layer] * line[i_layer + 1]);
      w[i_layer].resize(_layers[i_layer]->neurons().size());
      for (size_t i_neuron = 0; i_neuron < _layers[i_layer]->neurons().size();
           ++i_neuron) {
        w[i_layer][i_neuron].resize(
            _layers[i_layer]->neurons()[i_neuron]->getNextEdge().size());
        for (size_t i_edge = 0;
             i_edge <
                 _layers[i_layer]->neurons()[i_neuron]->getNextEdge().size();
             ++i_edge, count++)
          fin >> w[i_layer][i_neuron][i_edge];
      }
    }
    alterWeights(w);
    for (size_t i_layer = 1; i_layer < _layers.size(); ++i_layer) {
      multi += line[i_layer];
      for (size_t i_neuron = 0; i_neuron < _layers[i_layer]->neurons().size();
           ++i_neuron) {
        double tmp;
        fin >> tmp;
        _layers[i_layer]->neurons()[i_neuron]->setBias(tmp);
        if (!fin.eof()) count++;
      }
    }
  }
  double temp;
  fin >> temp;
  if (!fin.eof())
    count++;
  if (count != multi) {
    printf("Amount of number is wrong!!!\n");
    emit wrongFile();
    return;
  }
  std::cout << filename << " readed \n";;
  fin.close();
  emit isTrained(true);
}

void NeuralNetwork::readData(std::string filename, mStatus v) {
  break_ = false;
  int size;
  if (v == train_)
    size = 88800;
  else
    size = 14800;
  emit dialogMsg(read_, filename);
    std::vector<std::vector<double>> _vect;
    std::ifstream fin(filename);
    int i = 0;
    if (!fin.is_open())
      std::cout << "Error reading file" << filename << std::endl;
    std::string tmp;
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
      if (tmp != "")
        _vect.push_back(_number);
      if (!(++i % (size/100)) && !break_)
        emit updateBar(i / (size/100), test_);
    }
    fin.close();
    // std::cout << " MNIST loaded... \n";
  emit iAmReady(read_);
  if (v == train_ && !break_)
    _vector_train = _vect;
  else if (v == test_ && !break_)
    _vector_test = _vect;
  if (!_vector_test.empty()){
    emit actTestBtn(true);
    if (!_vector_train.empty())
      emit actTrainBtn(true);
  }
}

void NeuralNetwork::destroyNet() {
  if (!_typeNet) {
    for (int i = 0; i < _hidden + 2; i++) {
      delete[] _neurons_val[i];
      delete[] _neurons_err[i];
    }
    delete[] _neurons_val;
    delete[] _neurons_err;
    for (int i = 0; i < _hidden + 1; i++) delete[] _bias[i];
    delete[] _bias;
    for (int i = 0; i < _hidden + 1; i++) _weights[i].clear(_layerSize[i + 1]);
    delete[] _weights;
    _layerSize.clear();
  } else {
    for (Layer *l : _layers) delete l;
  }
}

int NeuralNetwork::getEpoch() {return _epoch;}

int NeuralNetwork::getLayer() {return _hidden;}

int NeuralNetwork::getTypeNet() { return _typeNet; }

void NeuralNetwork::setTypeNet(bool n) { _typeNet = n;}

// void NeuralNetwork::setLayer(int n) {_hidden = n; std::cout << "Set " << _hidden << " layers\n";}

void NeuralNetwork::setEpoch(int n) { _epoch = n; std::cout << "Set " << _epoch << " epochs\n";}

void NeuralNetwork::setBreak(bool b) { break_ = b;}

double NeuralNetwork::getMaxRa() {return _maxRa;}

void NeuralNetwork::SetTestPart(int i) { test_part_ = i/100.;}

std::chrono::duration<double> NeuralNetwork::getTime() {return _time;}

} // s21
