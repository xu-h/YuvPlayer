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

private:
    Ui::Player *ui;

    Sequence *seq;

    QImage *rgb;

    QLabel *imageLabel;
    QScrollArea *scrollArea;
    void openYuvFile();
};
#endif // PLAYER_H
