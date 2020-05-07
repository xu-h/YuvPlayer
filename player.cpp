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

    ui->scrollArea->init();
//    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

Player::~Player()
{
    delete ui;
    if (seq) {
        delete seq;
    }
}

void Player::on_OpenButton_clicked()
{
    openYuvFile();

    ui->widthBox->setValue(seq->getWidth());
    ui->heightBox->setValue(seq->getHeight());
    ui->depthBox->setValue(seq->getDepth());

    rgb = seq->nextFrame();
    ui->scrollArea->setImg(rgb);
}

void Player::on_updateButton_clicked()
{
    ui->scrollArea->setImg(rgb);
}

void Player::openYuvFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open YUV"), QString("F:/seq/vvc"));
    ui->seqNameLabel->setText(fileName);

    seq = new Sequence(fileName);
}

void Player::on_nextButton_clicked()
{
    rgb = seq->nextFrame();
    ui->scrollArea->setImg(rgb);
}

void Player::on_prevButton_clicked()
{
    rgb = seq->prevFrame();
    ui->scrollArea->setImg(rgb);
}

void Player::on_heightBox_valueChanged(int height)
{
    if (height != seq->getHeight()) {
        qDebug() << "set height from" << seq->getHeight() << "to" << height << endl;
        SeqError error = seq->setWidth(height);
        if (error == SEQ_INVALID_FILE_SIZE) {
            ui->logText->append(QString("The file size is not divisible by the number of bytes per frame."));
        } else {
            ui->logText->append(QString("Height changed."));
        }
        rgb = seq->nextFrame();
        ui->scrollArea->setImg(rgb);
    }
}

void Player::on_widthBox_valueChanged(int width)
{
    if (width != seq->getWidth()) {
        qDebug() << "set width from" << seq->getWidth() << "to" << width << endl;
        SeqError error = seq->setWidth(width);
        if (error == SEQ_INVALID_FILE_SIZE) {
            ui->logText->append(QString("The file size is not divisible by the number of bytes per frame."));
        } else {
            ui->logText->append(QString("Width changed."));
        }
        rgb = seq->nextFrame();
        ui->scrollArea->setImg(rgb);
    }
}

void Player::on_depthBox_valueChanged(int depth)
{
    if (depth != seq->getDepth()) {
        qDebug() << "set depth from" << seq->getDepth() << "to" << depth << endl;
        SeqError error = seq->setDepth(depth);
        if (error == SEQ_INVALID_DEPTH) {
            ui->logText->append(QString("Only 8bit ~ 16bit are supported."));
        } else {
            ui->logText->append(QString("Depth changed."));
        }
        rgb = seq->nextFrame();
        ui->scrollArea->setImg(rgb);
    }
}
