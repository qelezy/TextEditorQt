#include "resizableframe.h"

ResizableFrame::ResizableFrame(QWidget *parent) : QFrame(parent) { setMouseTracking(true); }

void ResizableFrame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && startPos != none) {
        dragStartPosition = event->pos();
        dragStartGeometry = window()->geometry();
    }
}

void ResizableFrame::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons()) && !(window()->isMaximized())) {
        if (event->pos().x() <= 5 && event->pos().y() <= 6) {
            startPos = topLeft;
            setCursor(Qt::SizeFDiagCursor);
        } else if (event->pos().x() <= 8 && event->pos().y() >= height() - 8) {
            startPos = bottomLeft;
            setCursor(Qt::SizeBDiagCursor);
        } else if (event->pos().x() >= width() - 6 && event->pos().y() <= 6) {
            startPos = topRight;
            setCursor(Qt::SizeBDiagCursor);
        } else if (event->pos().x() >= width() - 8 && event->pos().y() >= height() - 8) {
            startPos = bottomRight;
            setCursor(Qt::SizeFDiagCursor);
        } else if (event->pos().x() <= 5) {
            startPos = left;
            setCursor(Qt::SizeHorCursor);
        } else if (event->pos().x() >= width() - 6) {
            startPos = right;
            setCursor(Qt::SizeHorCursor);
        } else if (event->pos().y() <= 5) {
            startPos = top;
            setCursor(Qt::SizeVerCursor);
        } else if (event->pos().y() >= height() - 5) {
            startPos = bottom;
            setCursor(Qt::SizeVerCursor);
        } else {
            startPos = none;
            setCursor(Qt::ArrowCursor);
        }
        return;
    }
    else if (event->buttons() & Qt::LeftButton) {
        switch (startPos) {
        case topLeft:
            window()->setGeometry(QRect(QPoint((window()->width() - event->pos().x() >= window()->minimumWidth() && window()->width() - (event->pos().x() - dragStartPosition.x()) <= window()->screen()->size().width() && window()->width() - (event->pos().x() - dragStartPosition.x()) <= window()->maximumWidth()) ? qMax(event->globalPosition().toPoint().x() - dragStartPosition.x(), 0) : window()->geometry().left(),
                                               (window()->height() - event->pos().y() >= window()->minimumHeight() && window()->height() - (event->pos().y() - dragStartPosition.y()) <= window()->screen()->availableSize().height() && window()->height() - (event->pos().y() - dragStartPosition.y()) <= window()->maximumHeight()) ? qMax(event->globalPosition().toPoint().y() - dragStartPosition.y(), 0) : window()->geometry().top()),
                                        QPoint(dragStartGeometry.right(), dragStartGeometry.bottom())));
            break;

        case bottomLeft:
            window()->setGeometry(QRect(QPoint((window()->width() - event->pos().x() >= window()->minimumWidth() && window()->width() - (event->pos().x() - dragStartPosition.x()) <= window()->screen()->size().width() && window()->width() - (event->pos().x() - dragStartPosition.x()) <= window()->maximumWidth()) ? qMax(event->globalPosition().toPoint().x() - dragStartPosition.x(), 0) : window()->geometry().left(), dragStartGeometry.top()),
                                        QPoint(dragStartGeometry.right(), (event->pos().y() + dragStartGeometry.height() - dragStartPosition.y() <= window()->screen()->availableSize().height() && event->pos().y() + dragStartGeometry.height() - dragStartPosition.y() <= window()->maximumHeight()) ? qMin(event->globalPosition().toPoint().y() + dragStartGeometry.height() - dragStartPosition.y(), window()->screen()->availableSize().height()) : window()->geometry().bottom())));
            break;

        case topRight:
            window()->setGeometry(QRect(QPoint(dragStartGeometry.left(), (window()->height() - event->pos().y() >= window()->minimumHeight() && window()->height() - event->pos().y() + dragStartPosition.y() <= window()->screen()->availableSize().height() && window()->height() - event->pos().y() + dragStartPosition.y() <= window()->maximumHeight()) ? qMax(event->globalPosition().toPoint().y() - dragStartPosition.y(), 0) : window()->geometry().top()),
                                        QPoint((event->pos().x() + dragStartGeometry.width() - dragStartPosition.x() <= window()->screen()->size().width() && event->pos().x() + dragStartGeometry.width() - dragStartPosition.x() <= window()->maximumWidth()) ? qMin(event->globalPosition().toPoint().x() + dragStartGeometry.width() - dragStartPosition.x(), window()->screen()->size().width()) : window()->geometry().right(), dragStartGeometry.bottom())));
            break;

        case bottomRight:
            window()->setGeometry(QRect(QPoint(dragStartGeometry.left(), dragStartGeometry.top()),
                                        QPoint((event->pos().x() + dragStartGeometry.width() - dragStartPosition.x() <= window()->screen()->size().width() && event->pos().x() + dragStartGeometry.width() - dragStartPosition.x() <= window()->maximumWidth()) ? qMin(event->globalPosition().toPoint().x() + dragStartGeometry.width() - dragStartPosition.x(), window()->screen()->size().width()) : window()->geometry().right(),
                                               (event->pos().y() + dragStartGeometry.height() - dragStartPosition.y() <= window()->screen()->availableSize().height() && event->pos().y() + dragStartGeometry.height() - dragStartPosition.y() <= window()->maximumHeight()) ? qMin(event->globalPosition().toPoint().y() + dragStartGeometry.height() - dragStartPosition.y(), window()->screen()->availableSize().height()) : window()->geometry().bottom())));
            break;

        case left:
            if (window()->width() - event->pos().x() >= window()->minimumWidth() && window()->width() - (event->pos().x() - dragStartPosition.x()) <= window()->screen()->size().width() && window()->width() - (event->pos().x() - dragStartPosition.x()) <= window()->maximumWidth()) {
                window()->setGeometry(QRect(QPoint(qMax(event->globalPosition().toPoint().x() - dragStartPosition.x(), 0), dragStartGeometry.top()),
                                            QPoint(dragStartGeometry.right(), dragStartGeometry.bottom())));
            }
            break;

        case right:
            if (event->pos().x() + dragStartGeometry.width() - dragStartPosition.x() <= window()->screen()->size().width()) {
                window()->setGeometry(QRect(QPoint(dragStartGeometry.left(), dragStartGeometry.top()),
                                            QPoint(qMin(event->globalPosition().toPoint().x() + dragStartGeometry.width() - dragStartPosition.x(), window()->screen()->size().width()), dragStartGeometry.bottom())));
            }
            break;

        case top:
            if (window()->height() - event->pos().y() >= window()->minimumHeight() && window()->height() - (event->pos().y() - dragStartPosition.y()) <= window()->screen()->availableSize().height() && window()->height() - (event->pos().y() - dragStartPosition.y()) <= window()->maximumHeight()) {
                window()->setGeometry(QRect(QPoint(dragStartGeometry.left(), qMax(event->globalPosition().toPoint().y() - dragStartPosition.y(), 0)),
                                            QPoint(dragStartGeometry.right(), dragStartGeometry.bottom())));
            }
            break;

        case bottom:
            if (event->pos().y() + dragStartGeometry.height() - dragStartPosition.y() <= window()->screen()->availableSize().height()) {
                window()->setGeometry(QRect(QPoint(dragStartGeometry.left(), dragStartGeometry.top()),
                                            QPoint(dragStartGeometry.right(), qMin(event->globalPosition().toPoint().y() + dragStartGeometry.height() - dragStartPosition.y(), window()->screen()->availableSize().height()))));
            }
            break;

        default:
            break;
        }
    } else {
        if (cursor() != Qt::ArrowCursor)
            setCursor(Qt::ArrowCursor);
    }
}

void ResizableFrame::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (cursor() != Qt::ArrowCursor)
        setCursor(Qt::ArrowCursor);
}
