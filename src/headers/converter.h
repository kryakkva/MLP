//
// Created by Yarik on 09.02.2023.
//

#ifndef MLP_CONVERTER_H
#define MLP_CONVERTER_H

#include <QImage>
#include <QObject>
#include <iostream>
#include <vector>

namespace s21 {

class Converter : public QObject {
  Q_OBJECT

 public:
  Converter(QImage *image);
  ~Converter();

 private:
  QImage *_image;

 public:
  void saveImage();
  QRect findCropArea();
  std::vector<double> convertDrawImg();
  std::vector<double> convertLoadImg();

 signals:
  void sendStr(const QString &);

 private slots:
  void intToString(int);

 private:
  bool isInvert(QImage &img);
};

}  // namespace s21

#endif  // MLP_CONVERTER_H
