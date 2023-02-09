//
// Created by Yarik on 09.02.2023.
//

#ifndef MLP_CONVERTER_H
#define MLP_CONVERTER_H

#include <QImage>
namespace s21 {

    class Converter {
    public:
        Converter(QImage *image);
        ~Converter();
    private:
        QImage *_image;
    public:
        void saveImage();
    };

} // s21

#endif //MLP_CONVERTER_H
