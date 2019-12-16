#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollarea>

#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class Player; }
QT_END_NAMESPACE

enum ColorSpaceCvt {
    YUV2RGB_BT709_LIMIT,
    YUV2RGB_BT709_FULL,
    YUV2RGB_BT601_LIMIT,
    YUV2RGB_BT601_FULL,
    YUV2RGB_BT2020_LIMIT,
    YUV2RGB_BT2020_FULL,
    YUV2RGB_TYPE_NUM
};

const int yuv2rgb[YUV2RGB_TYPE_NUM][12] = {
    {76309, 117489, -13975, -34925, 138438}, // BT709_LimitedRange
    {65536, 103206, -12276, -30679, 121608}, // BT709_FullRange
    {76309, 104597, -25675, -53279, 132201}, // BT601_LimitedRange
    {65536,  91881, -22553, -46802, 116129}, // BT601_FullRange
    {76309, 110013, -12276, -42626, 140363}, // BT2020_LimitedRange
    {65536,  96638, -10783, -37444, 123299}  // BT2020_FullRange
};

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
    QImage *rgb;
    int width;
    int height;

    QLabel *imageLabel;
    QScrollArea *scrollArea;
    void openYuvFile();
};
#endif // PLAYER_H
