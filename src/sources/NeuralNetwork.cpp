//
// Created by apaege on 13.02.2023.
//

#include "../headers/NeuralNetwork.h"

namespace s21 {
NeuralNetwork::NeuralNetwork() : _typeNet(0), _hidden(2), _epoch(10) {
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
}

void NeuralNetwork::reInitNet() {
  if (_typeNetVal != _typeNet && _hiddenVal == _hidden)
      reSaveStudy();
  destroyNet();
  _hidden = _hiddenVal;
  _typeNet = _typeNetVal;
  initNet();
  if (_reWrite)
      reLoadStudy();
}

void NeuralNetwork::reSaveStudy() {
    _temp_counter = _counter;
    _w_temp.clear();
    _b_temp.clear();
    if (!_typeNet) {
        _w_temp.resize(_layerSize.size() - 1);
        _b_temp.resize(_layerSize.size() - 1);
        for (size_t i = 0; i < _layerSize.size() - 1; ++i) {
            _w_temp[i].resize(_layerSize[i]);
            for (int j = 0; j < _layerSize[i]; ++j) {
                _w_temp[i][j].resize(_layerSize[i + 1]);
                _b_temp[i].resize(_layerSize[i + 1]);
                for (int k = 0; k < _layerSize[i + 1]; k++) {
                    _w_temp[i][j][k] = _weights[i](k, j);
                    _b_temp[i][k] = _bias[i][k];
                }
            }
        }
    }
    else {
        _w_temp = getWeights();
        _b_temp.resize(_layers.size() - 1);
        for (size_t i = 1; i < _layers.size(); ++i) {
            _b_temp[i - 1].resize(_layers[i]->neurons().size());
            for (size_t k = 0; k < _layers[i]->neurons().size(); ++k)
                _b_temp[i - 1][k] = _layers[i]->neurons()[k]->getBias();
        }
    }
    _reWrite = true;
}

void NeuralNetwork::reLoadStudy() {
    _counter = _temp_counter;
    if (!_typeNet) {
        for (size_t i = 0; i < _layerSize.size() - 1; ++i)
            for (int j = 0; j < _layerSize[i]; ++j)
                for (int k = 0; k < _layerSize[i + 1]; k++) {
                    _weights[i](k, j) = _w_temp[i][j][k];
                    _bias[i][k] = _b_temp[i][k];
                }
    }
    else {
        alterWeights(_w_temp);
        for (size_t i = 1; i < _layers.size(); ++i)
            for (size_t k = 0; k < _layers[i]->neurons().size(); ++k)
                _layers[i]->neurons()[k]->setBias(_b_temp[i - 1][k]);
    }
    _reWrite = false;
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

double NeuralNetwork::networkTest(std::vector<std::vector<double>> value) {
  double ra = 0;
  double right;
  double predict;
//  printf("TypeNet = %d, Epoch = %d, layer = %d, counter = %d   ", _typeNet,
//         _epoch, _hidden, _counter);
  for (size_t i = 0; i < value.size(); i++) {
    setInput(value[i]);
    if (!_typeNet)
      predict = forwardFeed();
    else {
      trigger();
      predict = searchMaxIndexGraph(output());
    }
    right = value[i][0];
    if (right == predict) ra++;
  }
  _test_ra = ra;
  return (ra);
}

double NeuralNetwork::networkTrain(std::vector<std::vector<double>> value) {
  double ra = 0;
  double right;
  double predict;

  auto t1 = std::chrono::steady_clock::now();
  for (size_t i = 0; i < value.size(); i++) {
    setInput(value[i]);
    if (!_typeNet)
      predict = forwardFeed();
    else {
      trigger();
      predict = searchMaxIndexGraph(output());
    }
    right = value[i][0];
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
  }
  auto t2 = std::chrono::steady_clock::now();
  _time = t2 - t1;
  printf("TypeNet = %d, layer = %d ", _typeNet, _hidden);
  _counter++;
  if (ra > _maxRa) _maxRa = ra;
  return (ra);
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

std::vector<std::vector<std::vector<double>>> NeuralNetwork::getWeights() {
  std::vector<std::vector<std::vector<double>>> w;
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
  fout << " " << _epoch << " " << (100 - _test_ra * 100 / (_vector_test.size()*test_part_)) << std::endl;
  std::cout << (100 - _test_ra * 100 / (_vector_test.size()*test_part_)) << std::endl;
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
    std::vector<std::vector<std::vector<double>>> w;
    w = getWeights();
    for (size_t i_layer = 0; i_layer < _layers.size() - 1; ++i_layer)
      for (size_t i_neuron = 0; i_neuron < _layers[i_layer]->neurons().size();
           ++i_neuron)
        for (size_t i_edge = 0;
             i_edge <
             _layers[i_layer]->neurons()[i_neuron]->getNextEdge().size();
             ++i_edge)
          fout << w[i_layer][i_neuron][i_edge] << " ";
    for (size_t i_layer = 1; i_layer < _layers.size(); ++i_layer)
      for (size_t i_neuron = 0; i_neuron < _layers[i_layer]->neurons().size();
           ++i_neuron)
        fout << _layers[i_layer]->neurons()[i_neuron]->getBias() << " ";
  }
  std::cout << filename << " saved\n";
  fout.close();
}

void NeuralNetwork::readWeights(std::string filename) {
  std::ifstream fin;
  fin.open(filename);
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
  double num;
  while (fin.get() != '\n') {
    fin >> num;
    if (num - (int) num != 0) {
      printf("Wrong in file 1\n");
      exit(0);
    }
    _counter = num;
    fin >> _test_ra;
  }
  std::vector<int> line;
  while (fin.get() != '\n') {
    fin >> num;
    if (num - (int)num != 0) {
      printf("Wrong in file 2\n");
      exit(0);
    }
    line.push_back(static_cast<int>(num));
  }

  if (_hidden + 2 != (int)line.size()) {
    printf("Wrong in file 3\n");
    exit(0);
  }
  int multi = 0;
  int count = 0;
  if (!_typeNet) {
    for (int x = 0; x < _hidden + 1; ++x) {
      multi += (line[x] * line[x + 1]);
      for (int i = 0; i < _weights[x].getCol(); ++i)
        for (int j = 0; j < _weights[x].getRow(); ++j, count++)
          fin >> _weights[x](j, i);
    }
    for (int i = 0; i < _hidden + 1; ++i) {
      multi += line[i + 1];
      for (int j = 0; j < _layerSize[i + 1]; ++j) {
        fin >> _bias[i][j];
        if (!fin.eof()) count++;
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
  if (!fin.eof()) count++;
  if (count != multi) {
    printf("Amount of number is wrong!!!\n");
    exit(0);
  }

  std::cout << filename << " readed \n";
  ;
  fin.close();
}

std::vector<std::vector<double>> NeuralNetwork::readData(std::string filename) {
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
    if (tmp != "") _vect.push_back(_number);
  }
  fin.close();
  std::cout << "MNIST loaded... \n";
  return _vect;
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
      for (Layer *l : _layers)
          l->clearAll();
      _layers.clear();
  }
}

int NeuralNetwork::getEpoch() { return _epoch; }

int NeuralNetwork::getLayer() { return _hidden; }

int NeuralNetwork::getTypeNet() { return _typeNet; }

void NeuralNetwork::setEpoch(int n) { _epoch = n; }

double NeuralNetwork::getMaxRa() { return _maxRa; }

void NeuralNetwork::crossVal(int e) {
    if (!_crossVal) {
        if (!e) {
            _vector_train_cross.clear();
            _vector_test_cross.clear();
            for (size_t i = 0; i < _vector_test.size(); ++i)
                _vector_test_cross.push_back(_vector_test[i]);
            for (size_t i = 0; i < _vector_train.size(); ++i)
                _vector_train_cross.push_back(_vector_train[i]);
        }
    }
    else {
        _vector_train_cross.clear();
        _vector_test_cross.clear();
        double coeff = 1 - ((_crossVal - 1.) / _crossVal);
        size_t tmp = 0;
        size_t tmp_1 = 0;
        std::cout << "coeff = " << 1 - ((_crossVal - 1.) / _crossVal) << std::endl << std::endl;
        std::cout << "Train_vector_part_1 from 0 to ";
        if (_crossVal == 1) {
            for (size_t i = 0; i < _vector_train.size(); ++i)
                _vector_train_cross.push_back(_vector_train[i]);
            std::cout << _vector_train.size() << " size = " << _vector_train.size() << std::endl << std::endl;
            std::cout << "Test_vector is clear!!!" << std::endl << std::endl;
            std::cout << "Train_vector_part_2 is clear!!!" << std::endl << std::endl;
        }
        else {
            for (size_t i = 0; i < (size_t) (_vector_train.size() * (_crossVal - 1 - e) * coeff); ++i, tmp_1 = i)
                _vector_train_cross.push_back(_vector_train[i]);
            std::cout << tmp_1 << " size = " << tmp_1 << std::endl << std::endl;
            std::cout << "Test_vector from " << (size_t) (_vector_train.size() * (_crossVal - 1 - e) * coeff) << " to ";
            for (size_t i = (size_t) (_vector_train.size() * (_crossVal - 1 - e) * coeff);
                 i < (size_t) (_vector_train.size() * (_crossVal - e) * coeff); ++i, tmp = i)
                _vector_test_cross.push_back(_vector_train[i]);
            std::cout << tmp << " size = " << tmp - (size_t) (_vector_train.size() * (_crossVal - 1 - e) * coeff) << std::endl << std::endl;
            std::cout << "Train_vector_part_2 from " << (size_t) (_vector_train.size() * (_crossVal - e) * coeff)
                      << " to ";
            for (size_t i = (size_t) (_vector_train.size() * (_crossVal - e) * coeff); i < _vector_train.size(); ++i)
                _vector_train_cross.push_back(_vector_train[i]);
            std::cout << _vector_train.size() << " size = " << _vector_train.size() - (size_t) (_vector_train.size() * (_crossVal - e) * coeff) << std::endl << std::endl;
            std::cout << "Full size train_vector = " << _vector_train.size() - (size_t) (_vector_train.size() * (_crossVal - e) * coeff) + tmp_1 << std::endl << std::endl;
        }
    }
}

void NeuralNetwork::setCrossVal(int c) {
    _crossVal = c;
    _epoch = _crossVal;
}

std::chrono::duration<double> NeuralNetwork::getTime() { return _time; }

}  // namespace s21