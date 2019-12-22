#include "scrollarea.h"

#include <QtDebug>

#include <QPixmap>
#include <QMouseEvent>
#include <QGridLayout>
#include <QScrollBar>

ScrollArea::ScrollArea(QWidget *parent):
    QScrollArea(parent)
{
    setBackgroundRole(QPalette::Dark);
}

ScrollArea::~ScrollArea()
{
    delete m_imageLabel;
}

void ScrollArea::init()
{
    // qt designer add a scrollAreaWidgetContents
    m_imageLabel = new QLabel(this);
    setWidget(m_imageLabel);
}

void ScrollArea::display(QImage *img)
{
    m_img = img;
    m_imageLabel->setPixmap(QPixmap::fromImage(*img));
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
