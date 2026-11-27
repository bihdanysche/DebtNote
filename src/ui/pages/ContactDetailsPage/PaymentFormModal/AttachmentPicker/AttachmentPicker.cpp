#include "AttachmentPicker.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>

#include "ui/common/photoOverlay/PhotoOverlay.h"
#include "ui/common/toasts/toast.h"
#include "ui/mainwindow.h"
#include "ui/pages/ContactDetailsPage/PaymentFormModal/AttachmentPicker/AttachmentTile/AttachmentTile.h"

AttachmentPicker::AttachmentPicker(MainWindow* mv)
    : QWidget()
    , m_toast(mv->uiInstances().toast)
    , m_photoOverlay(mv->uiInstances().photoOverlay)
{
    setProperty("picker", "attachments");

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);

    auto* title = new QLabel("Вкладення", this);
    title->setProperty("role", "field");
    layout->addWidget(title);

    auto* container = new QWidget(this);
    container->setFixedHeight(56);
    layout->addWidget(container);

    m_tilesLayout = new QHBoxLayout(container);
    m_tilesLayout->setContentsMargins(0, 0, 0, 0);
    m_tilesLayout->setSpacing(8);

    rebuild();
}

void AttachmentPicker::setExisting(const QList<Attachment>& attachments) {
    m_entries.clear();
    m_removedIds.clear();

    for (const Attachment& attachment : attachments) {
        Entry entry;
        entry.existingId = attachment.id();
        entry.path = attachment.storedPath();
        entry.fileName = attachment.fileName();
        m_entries.append(entry);
    }

    rebuild();
}

void AttachmentPicker::clear() {
    m_entries.clear();
    m_removedIds.clear();
    rebuild();
}

QStringList AttachmentPicker::newFilePaths() const {
    QStringList paths;
    for (const Entry& entry : m_entries) {
        if (entry.existingId == 0) {
            paths << entry.path;
        }
    }
    return paths;
}

QList<qint64> AttachmentPicker::removedIds() const {
    return m_removedIds;
}

void AttachmentPicker::chooseFiles() {
    const QStringList selected = QFileDialog::getOpenFileNames(
        this, "Вибрати файли", QString(),
        "Зображення (*.png *.jpg *.jpeg *.bmp *.webp);;Усі файли (*)");

    auto alreadyAdded = [this](const QString& path) {
        for (const Entry& entry : m_entries) {
            if (entry.path == path) {
                return true;
            }
        }
        return false;
    };

    bool limitHit = false;
    for (const QString& path : selected) {
        if (m_entries.size() >= kMaxFiles) {
            limitHit = true;
            break;
        }
        if (alreadyAdded(path)) {
            continue;
        }

        Entry entry;
        entry.path = path;
        entry.fileName = QFileInfo(path).fileName();
        m_entries.append(entry);
    }

    rebuild();

    if (limitHit) {
        m_toast->makeToast(QString("Максимум %1 вкладень.").arg(kMaxFiles));
    }
}

void AttachmentPicker::removeEntry(int index) {
    if (index < 0 || index >= m_entries.size()) {
        return;
    }

    const Entry entry = m_entries.takeAt(index);
    if (entry.existingId > 0) {
        m_removedIds << entry.existingId;
    }

    rebuild();
}

void AttachmentPicker::rebuild() {
    while (QLayoutItem* item = m_tilesLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->hide();
            widget->deleteLater();
        }
        delete item;
    }

    QWidget* parent = m_tilesLayout->parentWidget();

    for (int i = 0; i < m_entries.size(); ++i) {
        const Entry& entry = m_entries.at(i);

        auto* tile = new AttachmentTile(entry.path, entry.fileName, parent);
        m_tilesLayout->addWidget(tile, 0, Qt::AlignTop);

        connect(tile, &AttachmentTile::removeRequested, this, [this, i]() {
            removeEntry(i);
        });

        connect(tile, &AttachmentTile::viewRequested, this, [this, entry]{
            m_photoOverlay->setImage(entry.path);
        });
    }

    if (m_entries.size() < kMaxFiles) {
        auto* addButton = new QToolButton(parent);
        addButton->setProperty("tile", "add");
        addButton->setText("+");
        addButton->setFixedSize(56, 56);
        addButton->setCursor(Qt::PointingHandCursor);
        addButton->setToolTip("Додати файли");
        m_tilesLayout->addWidget(addButton, 0, Qt::AlignTop);

        connect(addButton, &QToolButton::clicked, this, &AttachmentPicker::chooseFiles);
    }

    m_tilesLayout->addStretch();
}