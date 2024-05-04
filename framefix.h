#ifndef FRAMEFIX_H
#define FRAMEFIX_H

#include <QFrame>

class FrameFix : public QFrame
{
public:
    FrameFix(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // FRAMEFIX_H
