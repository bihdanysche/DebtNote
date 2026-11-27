#pragma once

#include <QList>
#include <QStringList>
#include <QWidget>

#include "domain/entities/Attachment.h"

class MainWindow;
class Toast;
class PhotoOverlay;

class QHBoxLayout;

class AttachmentPicker : public QWidget {
    Q_OBJECT

public:
    explicit AttachmentPicker(MainWindow* mv);

    void setExisting(const QList<Attachment>& attachments);
    void clear();

    QStringList newFilePaths() const;
    QList<qint64> removedIds() const;
private:
    struct Entry {
        qint64 existingId = 0;
        QString path;
        QString fileName;
    };

    void chooseFiles();
    void removeEntry(int index);
    void rebuild();

    QList<Entry> m_entries;
    QList<qint64> m_removedIds;
    static constexpr int kMaxFiles = 5;
    Toast* m_toast;
    PhotoOverlay* m_photoOverlay;
    QHBoxLayout* m_tilesLayout;
};