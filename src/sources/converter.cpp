//
// Created by Yarik on 09.02.2023.
//

#include <QDir>
#include "converter.h"

namespace s21 {
    Converter::Converter(QImage *image) {
        _image = image;
    }
    Converter::~Converter() {}

    void Converter::saveImage(){
        QImage im = *_image;
        im.invertPixels();
        im.save(QDir::homePath() + "/4.png", "png");
    }
} // s21