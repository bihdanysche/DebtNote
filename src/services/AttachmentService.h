#pragma once

#include <QList>
#include <QObject>
#include <optional>

#include "domain/entities/Attachment.h"
#include "domain/repositories/IAttachmentRepository.h"
#include "domain/repositories/IFileStorage.h"

class AttachmentService : public QObject {
    Q_OBJECT

public:
    AttachmentService(IAttachmentRepository& repo, IFileStorage& storage, QObject* parent = nullptr);

    bool attach(qint64 paymentId, const QString& sourcePath, QString* error = nullptr);
    bool remove(qint64 id, QString* error = nullptr);

    std::optional<Attachment> getById(qint64 id, QString* error = nullptr);
    QList<Attachment> getByPayment(qint64 paymentId, QString* error = nullptr);
    QList<Attachment> getByContact(qint64 contactId, QString* error = nullptr);

    void removeFiles(const QList<Attachment>& attachments);

signals:
    void attachmentsChanged(qint64 paymentId);

private:
    IAttachmentRepository& m_repo;
    IFileStorage& m_storage;
};