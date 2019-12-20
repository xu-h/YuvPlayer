#include "sequence.h"

#include <QtDebug>

Sequence::Sequence(QString filename)
{
    f = new QFile(filename);
    f->open(QIODevice::ReadOnly);

    memset(m_yuv, 0, sizeof(m_yuv));
    m_rgb = NULL;

    m_colorType = YUV2RGB_BT709_FULL;

    QString name = filename.section('/', -1).section('.', 0, 0);
    parseName(name);
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
    QStringList info = name.split('_');

    int width = 0, height = 0, depth = 8;
    bool ok;
    // TODO parse chroma format
    for (int i = 0; i < info.size(); i++) {
        QString attr = info.at(i);
        if (attr.contains('x')) {
            QStringList size = attr.split('x');
            if (size.size() == 2) {
                width = size.at(0).toInt(&ok);
                if (!ok) {
                    width = 0;
                }
                height = size.at(1).toInt(&ok);
                if (!ok) {
                    height = 0;
                }
            }
        } else if (attr.endsWith("bit")) {
            depth = attr.left(attr.indexOf("bit")).toUInt(&ok);
            if (!ok) {
                depth = 8;
            }
        }
    }

    config(width, height, depth);
}

void Sequence::config(int width, int height, int depth)
{
    m_depth = depth;
    int pixBytes = (depth + 15) >> 4;
//    qDebug() << "pix byte " << pixBytes << endl;
    qDebug() << "seq init: width" << width << "height" << height << "depth" << depth << endl;

    if (width > 0 && height > 0) {
    // TODO support other chroma format and depth
        m_width = width;
        m_height = height;
        m_widthC = width >> 1;
        m_heightC = height >> 1;
        m_sizeL = width * height * pixBytes;
        m_sizeC = m_sizeL >> 2;
        m_size = m_sizeL + m_sizeC + m_sizeC;

        for (int i = 0; i < 3; i++) {
            if (m_yuv[i]) {
                free(m_yuv[i]);
            }
        }
        m_yuv[0] = reinterpret_cast<uchar *>(malloc(static_cast<size_t>(m_sizeL)));
        m_yuv[1] = reinterpret_cast<uchar *>(malloc(static_cast<size_t>(m_sizeC)));
        m_yuv[2] = reinterpret_cast<uchar *>(malloc(static_cast<size_t>(m_sizeC)));

        if (m_rgb) {
            delete m_rgb;
        }
        m_rgb = new QImage(width, height, QImage::Format_RGB888);
    }
}

void Sequence::convertRGB()
{
    // TODO only bit depth smaller than 14 is supported
    const int* cvt = yuv2rgb[m_colorType];
    int lumaOffset = 16 << (m_depth - 8);
    int chromaOffset = 128 << (m_depth - 8);

    if (m_colorType == YUV2RGB_BT709_FULL || m_colorType == YUV2RGB_BT601_FULL || m_colorType == YUV2RGB_BT2020_FULL) {
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
}


QImage* Sequence::nextFrame()
{
    f->read(reinterpret_cast<char *>(m_yuv[0]), m_sizeL);
    f->read(reinterpret_cast<char *>(m_yuv[1]), m_sizeC);
    f->read(reinterpret_cast<char *>(m_yuv[2]), m_sizeC);

    convertRGB();

    return m_rgb;
}

QImage* Sequence::prevFrame()
{
    f->seek(f->pos() - m_size * 2);
    f->read(reinterpret_cast<char *>(m_yuv[0]), m_sizeL);
    f->read(reinterpret_cast<char *>(m_yuv[1]), m_sizeC);
    f->read(reinterpret_cast<char *>(m_yuv[2]), m_sizeC);

    convertRGB();

    return m_rgb;
}

int Sequence::getWidth() const
{
    return m_width;
}

int Sequence::getHeight() const
{
    return m_height;
}

int Sequence::getDepth() const
{
    return m_depth;
}
