//
// Created by apaege on 14.02.2023.
//

#ifndef MLP_DATASET_H
#define MLP_DATASET_H

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "./ActivateFunction.h"

namespace s21 {
    enum Datatype {
        TRAIN,
        TEST,
        WORK
    };

    class Dataset {
    private:
        std::vector<std::vector<double> > _ins;

        std::vector<const std::vector<double> *> _train_ins;

        std::vector<const std::vector<double> *> _test_ins;

        ActivateFunction _func;
    public:
        Dataset(std::string filename, Datatype d);

        ~Dataset();

//        void split(double ptrain);

        const std::vector<const std::vector<double> *>& getIns(Datatype d) const;
    };

} // s21

#endif //MLP_DATASET_H
