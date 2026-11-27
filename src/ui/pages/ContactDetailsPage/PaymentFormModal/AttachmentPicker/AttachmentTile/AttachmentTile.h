#pragma once

#include <QFrame>
#include <qevent.h>

class AttachmentTile : public QFrame {
    Q_OBJECT

public:
    AttachmentTile(const QString& path, const QString& fileName, QWidget* parent = nullptr, bool removable = true);
signals:
    void viewRequested();
    void removeRequested();
protected:
    void mousePressEvent(QMouseEvent* event) override;
};