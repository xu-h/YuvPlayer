#include "player.h"
#include "ui_player.h"

#include <QtDebug>
#include <QFileDialog>

#include <QFile>
#include <QPixmap>

Player::Player(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Player)
{
    ui->setupUi(this);

    width = 1920;
    height = 1080;
    size = static_cast<size_t>(width * height);

    yuvBuf[0] = reinterpret_cast<uchar *>(malloc(size));
    yuvBuf[1] = reinterpret_cast<uchar *>(malloc(size >> 2));
    yuvBuf[2] = reinterpret_cast<uchar *>(malloc(size >> 2));

    rgb = new QImage(width, height, QImage::Format_RGB888);

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
    imageLabel->setPixmap(QPixmap::fromImage(*rgb));
}

void Player::on_updateButton_clicked()
{
    imageLabel->setPixmap(QPixmap::fromImage(*rgb));
}

void Player::openYuvFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open YUV"), QString("F:/seq/vvc"));
    ui->seqNameLabel->setText(fileName);


    yuvFile = new QFile(fileName);
    yuvFile->open(QIODevice::ReadOnly);
    yuvFile->read(reinterpret_cast<char *>(yuvBuf[0]), static_cast<qint64>(size));
    yuvFile->read(reinterpret_cast<char *>(yuvBuf[1]), size >> 2);
    yuvFile->read(reinterpret_cast<char *>(yuvBuf[2]), size >> 2);

    // TODO only bit depth smaller than 14 is supported
    ColorSpaceCvt type = YUV2RGB_BT709_FULL;
    const int* cvt = yuv2rgb[type];
    int bitdepth = 8;
    int lumaOffset = 16 << (bitdepth - 8);
    int chromaOffset = 128 << (bitdepth - 8);
    if (type == YUV2RGB_BT709_FULL || type == YUV2RGB_BT601_FULL || type == YUV2RGB_BT2020_FULL) {
        lumaOffset = 0;
    }

    int widthC = width >> 1;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pos = i * width + j;
            int posC = (i >> 1) * widthC + (j >> 1);
            int y = (yuvBuf[0][pos] - lumaOffset);
            int u = yuvBuf[1][posC] - chromaOffset;
            int v = yuvBuf[2][posC] - chromaOffset;

            int r = (cvt[0] * y +            + cvt[1] * v) >> (bitdepth + 8);
            int g = (cvt[0] * y + cvt[2] * u + cvt[3] * v) >> (bitdepth + 8);
            int b = (cvt[0] * y + cvt[4] * u             ) >> (bitdepth + 8);

            r = r < 0 ? 0: r > 255 ? 255: r;
            g = g < 0 ? 0: g > 255 ? 255: g;
            b = b < 0 ? 0: b > 255 ? 255: b;

            rgb->setPixel(j, i, qRgb(r, g, b));
        }
    }
}
