//
// Created by Yarik on 09.02.2023.
//

#include "converter.h"

#include <QDir>

namespace s21 {
Converter::Converter(QImage *image) { _image = image; }
Converter::~Converter() {}

void Converter::saveImage() {
  QImage im = *_image;
  im.invertPixels();
  im.save(QDir::homePath() + "/4.png", "png");
}

QRect Converter::findCropArea() {
  QImage img = *_image;
  int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
  bool first = false;
  QRgb bg = img.pixel(0, 0);
  for (int y = 0; y < img.height(); y++) {
    QDebug dbg(QtDebugMsg);
    QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(y));
    for (int x = 0; x < img.width(); x++) {
      QRgb &rgb = line[x];
      if (rgb != bg) {
        if (!first) {
          y1 = y2 = y;
          x1 = x2 = x;
          first = true;
        } else {
          if (y2 < y) y2 = y;
          if (x1 > x) x1 = x;
          if (x2 < x && x2 > x1) x2 = x;
        }
      }
    }
  }
  qInfo() << x1 << y1 << x2 << y2;
  if (x1 < 10) x1 = 0;
  if (y1 < 10) y1 = 0;
  return QRect(x1 - 10, y1 - 10, x2 - x1 + 20, y2 - y1 + 20);
}

void rescaled(QImage *img) {
  bool rottate = false;
  if (img->width() < img->height()) {
    *img = img->transformed(QTransform().rotate(90));
    rottate = true;
  }
  QImage tmp(28, 28, QImage::Format_RGB32);
  int x = (img->width() - img->height()) / 2;
  int i = 0;
  for (; i < x; i++)
    for (int k = 0; k < 28; k++) tmp.setPixelColor(k, i, img->pixelColor(0, 0));
  for (int j = 0; j < img->height(); i++, j++)
    for (int k = 0; k < 28; k++) tmp.setPixelColor(k, i, img->pixelColor(k, j));
  for (; i < 28; i++)
    for (int k = 0; k < 28; k++) tmp.setPixelColor(k, i, img->pixelColor(0, 0));
  if (rottate)
    *img = tmp.transformed(QTransform().rotate(-90));
  else
    *img = tmp;
  img->save(QDir::homePath() + "/222.png", "png");
}

std::vector<double> Converter::convertDrawImg() {
  std::vector<double> _v;
  _v.push_back(-1.);
  QImage drawImg =
      _image->copy(findCropArea())
          .scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation)
          .transformed(QTransform().rotate(-90));
  drawImg.invertPixels();
  drawImg.mirror(false);
  if (drawImg.width() > drawImg.height() * 3)
    rescaled(&drawImg);
  else
    drawImg =
        drawImg.scaled(28, 28, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  //		QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
  //		blur->setBlurRadius(2);
  //		QImage result = applyEffectToImage(drawImg, blur);
  //		result.save(QDir::homePath() + "/22.png", "png");
  for (int y = 0; y < drawImg.height(); ++y) {
    QDebug dbg(QtDebugMsg);
    for (int x = 0; x < drawImg.width(); ++x) {
      QColor color = drawImg.pixelColor(x, y).toHsl();
      _v.push_back(color.lightness() / 255.);
    }
  }
  drawImg.save(QDir::homePath() + "/2.png", "png");
  //        modified = false;
  return _v;
}

std::vector<double> Converter::convertLoadImg() {
  std::vector<double> _v;
  return _v;
}
}  // namespace s21