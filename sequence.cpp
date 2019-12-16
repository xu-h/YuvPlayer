#include "sequence.h"

Sequence::Sequence(QString filename)
{
    f = new QFile(filename);
    f->open(QIODevice::ReadOnly);
}

Sequence::~Sequence()
{
    if (f) {
        f->close();
        delete f;
    }

    for (int i = 0; i < 3; i++)
    {
        if (m_yuv[i])
        {
            free(m_yuv[i]);
        }
    }

    if (m_rgb)
    {
        delete m_rgb;
    }
}

void Sequence::parseName(QString name)
{

}

void Sequence::config(int width, int height, int depth)
{
    // TODO support other chroma format and depth
    m_depth = depth;

    m_width = width;
    m_height = height;
    m_widthC = width >> 1;
    m_heightC = height >> 1;
    m_size = width * height;
    m_sizeC = m_size >> 2;

    m_yuv[0] = reinterpret_cast<uchar *>(malloc(static_cast<size_t>(m_size)));
    m_yuv[1] = reinterpret_cast<uchar *>(malloc(static_cast<size_t>(m_sizeC)));
    m_yuv[2] = reinterpret_cast<uchar *>(malloc(static_cast<size_t>(m_sizeC)));

    m_rgb = new QImage(width, height, QImage::Format_RGB888);
}

void Sequence::readFrame()
{
    f->read(reinterpret_cast<char *>(m_yuv[0]), m_size);
    f->read(reinterpret_cast<char *>(m_yuv[1]), m_sizeC);
    f->read(reinterpret_cast<char *>(m_yuv[2]), m_sizeC);
}

QImage* Sequence::getFrame(ColorCvtType type)
{
    readFrame();

    // TODO only bit depth smaller than 14 is supported
    const int* cvt = yuv2rgb[type];
    int lumaOffset = 16 << (m_depth - 8);
    int chromaOffset = 128 << (m_depth - 8);

    if (type == YUV2RGB_BT709_FULL || type == YUV2RGB_BT601_FULL || type == YUV2RGB_BT2020_FULL) {
        lumaOffset = 0;
    }
    int shift = m_depth + 8;

    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            int pos = i * m_width + j;
            int posC = (i >> 1) * m_widthC + (j >> 1);
            int y = m_yuv[0][pos] - lumaOffset;
            int u = m_yuv[1][posC] - chromaOffset;
            int v = m_yuv[2][posC] - chromaOffset;

            int r = (cvt[0] * y +            + cvt[1] * v) >> shift;
            int g = (cvt[0] * y + cvt[2] * u + cvt[3] * v) >> shift;
            int b = (cvt[0] * y + cvt[4] * u             ) >> shift;

            r = r < 0 ? 0: r > 255 ? 255: r;
            g = g < 0 ? 0: g > 255 ? 255: g;
            b = b < 0 ? 0: b > 255 ? 255: b;

            m_rgb->setPixel(j, i, qRgb(r, g, b));
        }
    }

    return m_rgb;
}
