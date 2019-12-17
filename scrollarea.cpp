#include "scrollarea.h"

#include <QPixmap>
#include <QGridLayout>

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
