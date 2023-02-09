#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QtWidgets>
#include <QColor>
#include <QPoint>
#include <QImage>

namespace s21 {

class DrawArea : public QWidget
{
Q_OBJECT

public:
    explicit DrawArea(QWidget *parent = nullptr);
//	bool saveImage(const QString &fileName, const char *fileFormat);
    double *saveImage();
    double *openImage(const QString &fileName);
    void setPenWidth(int val);
    QRect findCropArea();

signals:
    void sendPicture(QImage *);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
//	void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
//	void resizeImage(QImage  *image, const QSize &newSize);
    bool modified;
    bool scribling;
    QColor myPenColor;
    QImage *image;
    QPoint lastPoint;
    int myPenWidth;
    int widthP;
};

} // namespace s21
#endif // DRAWAREA_H
