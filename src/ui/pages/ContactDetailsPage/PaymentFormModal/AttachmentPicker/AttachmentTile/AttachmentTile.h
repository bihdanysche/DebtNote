#pragma once

#include <QFrame>

class AttachmentTile : public QFrame {
    Q_OBJECT

public:
    AttachmentTile(const QString& path, const QString& fileName, QWidget* parent = nullptr);

signals:
    void removeRequested();
};