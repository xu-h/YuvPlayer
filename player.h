#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollarea>

#include <QFile>

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

    QFile *yuvFile;
    QImage *yuv[3];
    int width;
    int height;

    QLabel *imageLabel;
    QScrollArea *scrollArea;
    void openYuvFile();
};
#endif // PLAYER_H
