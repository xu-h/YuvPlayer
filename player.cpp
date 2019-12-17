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

    rgb = seq->getFrame(YUV2RGB_BT709_FULL);
}
