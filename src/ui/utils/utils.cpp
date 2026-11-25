#include "utils.h"

QIcon Utils::recolorIcon(const QIcon &icon, const QColor &color) {
    QPixmap pixmap = icon.pixmap(24, 24);

    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor pixel = image.pixelColor(x, y);

            if (pixel.alpha() == 0)
                continue;

            pixel.setRed(color.red());
            pixel.setGreen(color.green());
            pixel.setBlue(color.blue());

            image.setPixelColor(x, y, pixel);
        }
    }

    return QIcon(QPixmap::fromImage(image));
}