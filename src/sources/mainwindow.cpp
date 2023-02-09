#include "../headers/mainwindow.h"
#include "./ui_view.h"

namespace s21 {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);

    drawArea = new DrawArea(ui->frame);
    connect(drawArea, SIGNAL(sendPicture(QImage *)), this, SLOT(GetPicture(QImage *)));
    drawArea->setGeometry(QRect(2, 2, 280, 280));
}

void MainWindow::GetPicture(QImage *img){
    qInfo() << "test string";
    Converter conv(img);
    conv.saveImage();
//    img.save(QDir::homePath() + "/3.png", "png");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete drawArea;
//    delete convert;
}

} // namespace s21
