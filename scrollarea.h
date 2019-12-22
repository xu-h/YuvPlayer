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
    void display(QImage *img);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QLabel *m_imageLabel;
    QImage *m_img;

    QPoint m_startPos;
    QPoint m_lastPos;
};

#endif // SCROLLAREA_H
