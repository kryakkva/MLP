#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QtWidgets>
#include <iostream>

#include "converter.h"

namespace s21 {

class DrawArea : public QWidget {
  Q_OBJECT

 public:
  explicit DrawArea(QWidget *parent = nullptr);
  ~DrawArea() noexcept;
  //	bool saveImage(const QString &fileName, const char *fileFormat);
  double *saveImage();
  double *openImage(const QString &fileName);
  void setPenWidth(int val);
  QRect findCropArea();
  Converter *getConv();

 signals:
  void sendLetter(std::vector<double>);

 protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  //	void resizeEvent(QResizeEvent *event) override;

 private slots:
  void loadImage();

 private:
  void drawLineTo(const QPoint &endPoint);
  //	void resizeImage(QImage  *image, const QSize &newSize);
  bool scribling;
  QImage *image;
  QPoint lastPoint;
  Converter *_conv;
};

}  // namespace s21
#endif  // DRAWAREA_H
