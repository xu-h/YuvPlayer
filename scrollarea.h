#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QObject>
#include <QScrollArea>
#include <QLabel>

class ScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    ScrollArea(QWidget *parent = nullptr);
    ~ScrollArea();
    void init();
    void display(QImage *img);
private:
    QLabel *m_imageLabel;
    QImage *m_img;
};

#endif // SCROLLAREA_H
