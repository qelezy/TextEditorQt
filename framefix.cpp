#include "framefix.h"

FrameFix::FrameFix(QWidget *parent) : QFrame(parent) { setMouseTracking(true); }

void FrameFix::mousePressEvent(QMouseEvent *event)
{
    QFrame::mousePressEvent(event);
}

void FrameFix::mouseMoveEvent(QMouseEvent *event)
{
    QFrame::mouseMoveEvent(event);
}

void FrameFix::mouseReleaseEvent(QMouseEvent *event)
{
    QFrame::mouseReleaseEvent(event);
}
