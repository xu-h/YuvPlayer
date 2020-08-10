#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollarea>

#include <QFile>

#include "sequence.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Player; }
QT_END_NAMESPACE


class Player : public QMainWindow
{
    Q_OBJECT

public:
    Player(QWidget *parent = nullptr);
    ~Player();

private slots:
    void on_updateButton_clicked();
    void on_OpenButton_clicked();
    void on_nextButton_clicked();

    void on_prevButton_clicked();

    void on_heightBox_valueChanged(int height);
    void on_widthBox_valueChanged(int width);

    void on_depthBox_valueChanged(int arg1);

    void on_frateBox_valueChanged(int arg1);

    void on_playButton_clicked();

    void on_stopButton_clicked();

private:
    Ui::Player *ui;

    Sequence *seq;

    QImage *rgb;
    QTimer *timer;

    QLabel *imageLabel;

    void openYuvFile();
};
#endif // PLAYER_H
