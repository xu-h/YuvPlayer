#include "player.h"
#include "ui_player.h"

#include <QFile>
#include <QDataStream>
#include <QPixmap>

Player::Player(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Player)
{
    ui->setupUi(this);

    width = 1920;
    height = 1080;

    yuv[0] = new QImage(width, height, QImage::Format_Grayscale8);
    yuv[1] = new QImage(width >> 1, height >> 1, QImage::Format_Grayscale8);
    yuv[2] = new QImage(width >> 1, height >> 1, QImage::Format_Grayscale8);

    imageLabel = new QLabel();
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    ui->scrollArea->setWidget(imageLabel);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);

}

Player::~Player()
{
    delete ui;

    if (yuvFile) {
        delete yuvFile;
    }
}

void Player::on_OpenButton_clicked()
{
    openYuvFile();
    imageLabel->setPixmap(QPixmap::fromImage(*yuv[0]));
}

void Player::on_updateButton_clicked()
{
    imageLabel->setPixmap(QPixmap::fromImage(*yuv[0]));
}

void Player::openYuvFile()
{
    yuvFile = new QFile("F:/seq/vvc/BasketballDrive_1920x1080_50.yuv");
    yuvFile->open(QIODevice::ReadOnly);
    yuvFile->read((char *)yuv[0]->bits(), width * height);
}
