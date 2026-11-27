#pragma once

#include <qframe.h>
#include <qpixmap.h>
#include <qwidget.h>

class PhotoOverlay : public QFrame {
    Q_OBJECT;
public:
    explicit PhotoOverlay(QWidget* parent = nullptr);
    void show();
    void hide();
    bool setImage(const QString& path);
private:
    QPixmap image;
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};