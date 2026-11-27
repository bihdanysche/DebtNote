#include "AttachmentTile.h"

#include <QFileInfo>
#include <QImage>
#include <QImageReader>
#include <QLabel>
#include <QPixmap>
#include <QToolButton>

AttachmentTile::AttachmentTile(const QString& path, const QString& fileName, QWidget* parent)
    : QFrame(parent)
{
    setProperty("tile", "attachment");
    setFixedSize(56, 56);
    setToolTip(fileName);

    auto* preview = new QLabel(this);
    preview->setGeometry(1, 1, 54, 54);
    preview->setAlignment(Qt::AlignCenter);

    QImageReader reader(path);
    reader.setAutoTransform(true);
    const QSize sourceSize = reader.size();
    if (sourceSize.isValid()) {
        reader.setScaledSize(sourceSize.scaled(108, 108, Qt::KeepAspectRatioByExpanding));
    }

    const QImage image = reader.read();
    if (image.isNull()) {
        preview->setText(QFileInfo(fileName).suffix().toUpper().left(4));
    } else {
        const int side = qMin(image.width(), image.height());
        const QImage square = image.copy((image.width() - side) / 2, (image.height() - side) / 2, side, side);
        preview->setPixmap(QPixmap::fromImage(square.scaled(54, 54, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    }

    auto* removeButton = new QToolButton(this);
    removeButton->setText("✕");
    removeButton->setCursor(Qt::PointingHandCursor);
    removeButton->setFixedSize(18, 18);
    removeButton->move(width() - 22, 4);

    connect(removeButton, &QToolButton::clicked, this, &AttachmentTile::removeRequested);
}