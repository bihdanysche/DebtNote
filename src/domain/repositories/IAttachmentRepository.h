#pragma once

#include <QList>
#include <QString>
#include <optional>

#include "domain/entities/Attachment.h"

class IAttachmentRepository {
public:
    virtual ~IAttachmentRepository() = default;

    virtual std::optional<qint64> add(const Attachment& attachment, QString* error = nullptr) = 0;
    virtual bool remove(qint64 id, QString* error = nullptr) = 0;

    virtual std::optional<Attachment> getById(qint64 id, QString* error = nullptr) = 0;
    virtual QList<Attachment> getByPayment(qint64 paymentId, QString* error = nullptr) = 0;
    virtual QList<Attachment> getByContact(qint64 contactId, QString* error = nullptr) = 0;
};