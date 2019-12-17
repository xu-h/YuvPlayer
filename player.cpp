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

    imageLabel = new QLabel();
//    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    ui->scrollArea->setWidget(imageLabel);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
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

    seq = new Sequence(fileName);

    rgb = seq->getFrame(YUV2RGB_BT709_FULL);
}
