#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QString>
#include <QFile>
#include <QImage>


typedef enum ColorCvtType {
    YUV2RGB_BT709_LIMIT,
    YUV2RGB_BT709_FULL,
    YUV2RGB_BT601_LIMIT,
    YUV2RGB_BT601_FULL,
    YUV2RGB_BT2020_LIMIT,
    YUV2RGB_BT2020_FULL,
    YUV2RGB_TYPE_NUM
} ColorCvtType;

const int yuv2rgb[YUV2RGB_TYPE_NUM][12] = {
    {76309, 117489, -13975, -34925, 138438}, // BT709_LimitedRange
    {65536, 103206, -12276, -30679, 121608}, // BT709_FullRange
    {76309, 104597, -25675, -53279, 132201}, // BT601_LimitedRange
    {65536,  91881, -22553, -46802, 116129}, // BT601_FullRange
    {76309, 110013, -12276, -42626, 140363}, // BT2020_LimitedRange
    {65536,  96638, -10783, -37444, 123299}  // BT2020_FullRange
};

class Sequence
{
public:
    Sequence(QString filename);
    ~Sequence();
    void config(int width, int height, int depth);
    QImage* getFrame(ColorCvtType type);

private:
    QFile* f;

    int m_width;
    int m_height;
    int m_widthC;
    int m_heightC;
    int m_size;
    int m_sizeC;

    int m_depth;

    uchar* m_yuv[3];
    QImage* m_rgb;

    void parseName(QString name);
    void readFrame();
};

#endif // SEQUENCE_H
