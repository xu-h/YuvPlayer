#include "scrollarea.h"

#include <QtDebug>

#include <QPixmap>
#include <QMouseEvent>
#include <QGridLayout>
#include <QScrollBar>
#include <QRgb>

ScrollArea::ScrollArea(QWidget *parent):
    QScrollArea(parent)
{
    setBackgroundRole(QPalette::Dark);

    m_logScale = 0;
}

ScrollArea::~ScrollArea()
{
    delete m_imageLabel;
    if (m_scaledImg) {
        delete m_scaledImg;
    }
}

void ScrollArea::init()
{
    // qt designer add a scrollAreaWidgetContents
    m_imageLabel = new QLabel(this);
    m_scaledImg = NULL;
    setWidget(m_imageLabel);
}

void ScrollArea::setImg(QImage *img)
{
    m_img = img;
    imgInit();
    display();
}

void ScrollArea::imgInit()
{
    QSize size;
    if (m_logScale > 0) {
        size.setWidth(m_img->width() << m_logScale);
        size.setHeight(m_img->height() << m_logScale);
    } else if (m_logScale < 0) {
        size.setWidth(m_img->width() >> -m_logScale);
        size.setHeight(m_img->height() >> -m_logScale);
    } else {
        size = m_img->size();
    }

    if (m_scaledImg == NULL || size != m_scaledImg->size()) {
        if (m_scaledImg) {
            delete m_scaledImg;
        }
        m_scaledImg = new QImage(size, m_img->format());
        qDebug() << "img display size" << m_scaledImg->size() << endl;
    }
}

void ScrollArea::display()
{
    if (m_logScale > 0) {
        int scale = m_logScale;
        for (int i = 0; i < m_scaledImg->height(); i++) {
            for (int j = 0; j < m_scaledImg->width(); j++) {
                m_scaledImg->setPixel(j, i, m_img->pixel(j >> scale, i >> scale));
            }
        }
        qDebug() << m_scaledImg->size() << endl;
        m_imageLabel->setPixmap(QPixmap::fromImage(*m_scaledImg));
    } else if (m_logScale < 0) {
        // downsample
        int scale = -m_logScale;
        for (int i = 0; i < m_scaledImg->height(); i++) {
            for (int j = 0; j < m_scaledImg->width(); j++) {
                m_scaledImg->setPixel(j, i, m_img->pixel(j << scale, i << scale));
            }
        }
        m_imageLabel->setPixmap(QPixmap::fromImage(*m_scaledImg));
    } else {
        m_imageLabel->setPixmap(QPixmap::fromImage(*m_img));
    }
}

void ScrollArea::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        qDebug() << "move start" << event->pos() << endl;
        m_startPos = event->pos();
        m_lastPos = event->pos();
    }
}

void ScrollArea::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->pos() - m_lastPos;
        QScrollBar* bar = horizontalScrollBar();
        bar->setValue(bar->value() - delta.x());
        bar = verticalScrollBar();
        bar->setValue(bar->value() - delta.y());

        m_lastPos = event->pos();
    }
}

void ScrollArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        QPoint delta = event->pos() - m_startPos;
        qDebug() << "move end" << event->pos() << delta << endl;
    }
}

void ScrollArea::wheelEvent(QWheelEvent *event) {
    if (event->delta() > 0) {
        m_logScale += 1;
        qDebug() << "upsample to level" << m_logScale << exp2(m_logScale) << endl;
    } else if (event->delta() < 0){
        m_logScale -= 1;
        qDebug() << "downsample to level" << m_logScale << exp2(m_logScale) << endl;
    }
    imgInit();
    display();
}


