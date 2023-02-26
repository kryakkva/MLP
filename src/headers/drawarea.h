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
  //	bool SaveImage(const QString &fileName, const char *fileFormat);
  double *SaveImage();
  double *OpenImage(const QString &fileName);
  void SetPenWidth(int val);
  QRect FindCropArea();
  Converter *getConv();

 signals:
  void SendLetter(std::vector<double>);

 protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  //	void resizeEvent(QResizeEvent *event) override;

 private slots:
  void LoadImage();

 private:
  void DrawLineTo(const QPoint &endPoint);
  //	void resizeImage(QImage  *image_, const QSize &newSize);
  bool scribling_;
  QImage *image_;
  QPoint last_point_;
  Converter *conv_;
};

}  // namespace s21
#endif  // DRAWAREA_H
