#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QObject>
#include <QScrollArea>
#include <QLabel>

#include <QPoint>

class ScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    ScrollArea(QWidget *parent = nullptr);
    ~ScrollArea();
    void init();
    void setImg(QImage *img);
    void display();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    QLabel *m_imageLabel;

    QImage *m_img;
    QImage *m_scaledImg;

    QPoint m_startPos;
    QPoint m_lastPos;

    int m_logScale;

    void imgInit();
};

#endif // SCROLLAREA_H
