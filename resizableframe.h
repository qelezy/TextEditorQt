#ifndef RESIZABLEFRAME_H
#define RESIZABLEFRAME_H

#include <QFrame>
#include <QMouseEvent>

enum startPosition {topLeft, bottomLeft, topRight, bottomRight, left, right, top, bottom, none};

class ResizableFrame : public QFrame
{
public:
    ResizableFrame(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    startPosition startPos;
    QPoint dragStartPosition;
    QRect dragStartGeometry;
};

#endif // RESIZABLEFRAME_H
