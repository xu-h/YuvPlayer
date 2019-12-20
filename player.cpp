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
    if (seq)
    {
        delete seq;
    }
}

void Player::on_OpenButton_clicked()
{
    openYuvFile();

    ui->widthEdit->setText(QString::number(seq->getWidth()));
    ui->heightEdit->setText(QString::number(seq->getHeight()));
    ui->depthBox->setValue(seq->getDepth());

    rgb = seq->nextFrame();
    ui->scrollArea->display(rgb);
}

void Player::on_updateButton_clicked()
{
    ui->scrollArea->display(rgb);
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
    ui->scrollArea->display(rgb);
}

void Player::on_prevButton_clicked()
{
    rgb = seq->prevFrame();
    ui->scrollArea->display(rgb);
}
