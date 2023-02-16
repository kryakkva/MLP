//
// Created by apaege on 14.02.2023.
//

#include "../headers/Dataset.h"

namespace s21 {
    Dataset::Dataset(std::string filename, Datatype d)
    {
        std::ifstream infile(filename);
        std::string tmp;
        bool first;
        while (!infile.eof()) {
            std::getline(infile, tmp);
            std::istringstream input;
            input.str(tmp.c_str());
            first = true;
            std::vector<double> line;
            if (d == WORK) {
                line.push_back(-1);
                first = false;
            }
            for (std::string num; std::getline(input, num, ',');) {
                if (first) {
                    line.push_back({static_cast<double>(stoi(num))});
                    first = false;
                } else
                    line.push_back(stoi(num) / 255.);
            }
            if (tmp != "")
                _ins.push_back(line);
        }
    }

    Dataset::~Dataset()
    {
    }
/*
    void Dataset::split(double ptrain)
    {
        for (size_t i = 0; i < _ins.size(); i++)
        {
            if (_func.randomize(0, 1) < ptrain)
                _train_ins.push_back(&_ins[i]);
            else
                _test_ins.push_back(&_ins[i]);
        }
    }

*/
    const std::vector<const std::vector<double> *>& Dataset::getIns(Datatype d) const
    {
//        if(d == Datatype::TRAIN)
//            return _train_ins;
//        return _test_ins;
        for (size_t i = 0; i < _ins.size(); i++)
            _test_ins.push_back(&_ins[i]);
        return _test_ins;
    }
 } // s21