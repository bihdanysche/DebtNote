#include "PhotoOverlay.h"
#include <qevent.h>
#include <qframe.h>
#include <qguiapplication.h>
#include <qimagereader.h>
#include <qnamespace.h>
#include <qpaintdevice.h>
#include <qpainter.h>
#include <qpoint.h>
#include <qscreen.h>
#include <qsize.h>
#include <qtimer.h>

PhotoOverlay::PhotoOverlay(QWidget* parent)
    : QFrame(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
}

bool PhotoOverlay::setImage(const QString& path) {
    QImageReader reader(path);
    reader.setAutoTransform(true);

    QImage loadedImage = reader.read();

    if (loadedImage.isNull()) {
        return false;
    }

    image = QPixmap::fromImage(loadedImage);

    update();
    show();
    return true;
}

void PhotoOverlay::show() {
    QScreen* screen = QGuiApplication::screenAt(
        parentWidget()->mapToGlobal(parentWidget()->rect().center())
    );

    if (screen) {
        setGeometry(screen->geometry());
    }
    
    showFullScreen();
    raise();
    activateWindow();
}

void PhotoOverlay::hide() {
    QFrame::hide();
}

void PhotoOverlay::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(rect(), Qt::transparent);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.fillRect(rect(), QColor(0, 0, 0, 160));

    if (!image.isNull()) {
        QSize scaledSize = image.size();

        scaledSize.scale(
            size() - QSize(500, 500),
            Qt::KeepAspectRatio
        );

        QRect imgRect(
            QPoint(0, 0),
            scaledSize
        );

        imgRect.moveCenter(rect().center());
        painter.drawPixmap(imgRect, image);
    }
}

void PhotoOverlay::mousePressEvent(QMouseEvent* event) {
    hide();
    event->accept();
}