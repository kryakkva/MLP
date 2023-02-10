#include "../headers/drawarea.h"

#include <QPen>

namespace s21 {
DrawArea::DrawArea(QWidget *parent) : QWidget{parent} {
  myPenColor = Qt::black;
  myPenWidth = 14;
  modified = false;
  scribling = false;
  image = new QImage(280, 280, QImage::Format_RGB32);
  image->fill(Qt::white);
  //  QPainter painter(&image);
  //  painter.drawImage(QPoint(0,0), image);
}

DrawArea::~DrawArea() noexcept { delete image; }

void DrawArea::mousePressEvent(QMouseEvent *event) {
  // if (modified == false){
  //   image.fill(Qt::white);
  //   update();
  // }
  if (event->button() == Qt::LeftButton) {
    lastPoint = event->pos();
    scribling = true;
  } else if (event->button() == Qt::RightButton) {
    image->fill(Qt::white);
    update();
  }
}

void DrawArea::mouseMoveEvent(QMouseEvent *event) {
  if ((event->buttons() & Qt::LeftButton) && scribling)
    drawLineTo(event->pos());
}

void DrawArea::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton && scribling) {
    drawLineTo(event->pos());
    scribling = false;
    emit sendPicture(image);
  }
  update();
}

void DrawArea::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  //  QRect dirtyRect = event->rect();
  painter.drawImage(event->rect(), *image);
}

void DrawArea::drawLineTo(const QPoint &endPoint) {
  QPainter painter(image);
  painter.setPen(
      QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter.drawPoint(lastPoint);
  painter.drawLine(lastPoint, endPoint);
  modified = true;
  //  int rad = (myPenWidth / 3) + 3;
  //  update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad,
  //  +rad, +rad));
  update();
  lastPoint = endPoint;
}
/*
QRect DrawArea::findCropArea() {
  QImage img = image;
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

QImage applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent = 0) {
  if (src.isNull()) return QImage();  // No need to do anything else!
  if (!effect) return src;            // No need to do anything else!
  QGraphicsScene scene;
  QGraphicsPixmapItem item;
  item.setPixmap(QPixmap::fromImage(src));
  item.setGraphicsEffect(effect);
  scene.addItem(&item);
  QImage res(src.size() + QSize(extent * 2, extent * 2), QImage::Format_ARGB32);
  res.fill(Qt::transparent);
  QPainter ptr(&res);
  scene.render(&ptr, QRectF(),
               QRectF(-extent, -extent, src.width() + extent * 2,
                      src.height() + extent * 2));
  return res;
}

// bool DrawArea::saveImage(const QString &fileName, const char *fileFormat){
double *DrawArea::saveImage() {
  //		double ret[784];
  //		int num = 0;
  QImage visibleImage =
      image.copy(findCropArea())
          .scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation)
          .transformed(QTransform().rotate(-90));
  visibleImage.invertPixels();
  visibleImage.mirror(false);
  if (visibleImage.width() > visibleImage.height() * 3)
    rescaled(&visibleImage);
  else
    visibleImage = visibleImage.scaled(28, 28, Qt::IgnoreAspectRatio,
                                       Qt::SmoothTransformation);
  //		QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
  //		blur->setBlurRadius(2);
  //		QImage result = applyEffectToImage(visibleImage, blur);
  //		result.save(QDir::homePath() + "/22.png", "png");
  for (int y = 0; y < visibleImage.height(); ++y) {
    QDebug dbg(QtDebugMsg);
    for (int x = 0; x < visibleImage.width(); ++x) {
      QColor color = visibleImage.pixelColor(x, y).toHsl();
      //				ret[num++] = color.lightness() / 255.;
    }
  }
  visibleImage.save(QDir::homePath() + "/2.png", "png");
  modified = false;
  return NULL;
}

bool isInvert(QImage *img, QColor *col) {
  int i = 0, k = 0;
  for (int y = 0; y < img->height(); ++y) {
    for (int x = 0; x < img->width(); ++x) {
      QColor color = img->pixelColor(x, y).toHsl();
      if (*col != color) {
        i += color.lightness();
        k++;
      }
    }
  }
  if (!k) k = 1;
  return (((i / k) < col->lightness()) ? true : false);
}

double *DrawArea::openImage(const QString &fileName) {
  QImage loadedImage;
  //		double *ret = new double[784];
  //		int num = 0;
  if (!loadedImage.load(fileName)) return NULL;
  image = loadedImage.scaled(280, 280);
  loadedImage =
      image.copy(findCropArea())
          .scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation)
          .transformed(QTransform().rotate(-90));
  loadedImage.mirror(false);
  QColor bg = loadedImage.pixelColor(0, 0).toHsl();
  qInfo() << "before invert" << bg.hue() << bg.saturation() << bg.lightness();
  if (isInvert(&loadedImage, &bg)) loadedImage.invertPixels();
  bg = loadedImage.pixelColor(0, 0).toHsl();
  qInfo() << "after invert" << bg.hue() << bg.saturation() << bg.lightness();
  if (loadedImage.width() > loadedImage.height() * 3)
    rescaled(&loadedImage);
  else
    loadedImage = loadedImage.scaled(28, 28, Qt::IgnoreAspectRatio,
                                     Qt::SmoothTransformation);
  for (int y = 0; y < loadedImage.height(); ++y) {
    QDebug dbg(QtDebugMsg);
    for (int x = 0; x < loadedImage.width(); ++x) {
      QColor color = loadedImage.pixelColor(x, y).toHsl();
      if (color == bg) {
        color = Qt::black;
        loadedImage.setPixelColor(x, y, color);
      } else {
        //					dbg << color.hue() <<
        //color.saturation()
        << color.lightness() << " | ";
        if (color.lightness() < 20) color = color.lighter(1000);
        // color.setHsl(color.hue(), color.saturation(), 255);
//					loadedImage.setPixelColor(x, y,
QColor::fromHsl(color.hue(), 0, color.lightness()).lighter(500));
loadedImage.setPixelColor(x, y, color);
      }
      //				dbg << color.lightness();
      //				ret[num++] = color.lightness() / 255.;
    }
  }
  //		for (int y = 0; y < loadedImage.height(); ++y) {
  //				QDebug dbg(QtDebugMsg);
  //			QRgb *line =
  reinterpret_cast<QRgb *>(loadedImage.scanLine(y));
  //			for (int x = 0; x < loadedImage.width(); ++x) {
  //				QRgb &rgb = line[x];
  //				if (rgb == bg)
  //					rgb = qRgb(0, 0, 0);
  //				else{
  //					QColor pix = loadedImage.pixelColor(x,
y);
//					pix.setHsl(120, pix.saturation(),
pix.lightness());
//					rgb = pix.rgb();
//				}
//				dbg << qGreen(rgb);
//			}
//		}
if (loadedImage.save(QDir::homePath() + "/3.png", "png")) {
  modified = false;
  update();
  return NULL;
} else {
  return NULL;
}
}

void DrawArea::setPenWidth(int val) { myPenWidth = val; }
*/
}  // namespace s21
