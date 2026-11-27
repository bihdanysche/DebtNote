#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>

#include "domain/repositories/IAttachmentRepository.h"

class SQLiteAttachmentRepository : public IAttachmentRepository {
public:
    explicit SQLiteAttachmentRepository(QSqlDatabase db);

    std::optional<qint64> add(const Attachment& attachment, QString* error = nullptr) override;
    bool remove(qint64 id, QString* error = nullptr) override;

    std::optional<Attachment> getById(qint64 id, QString* error = nullptr) override;
    QList<Attachment> getByPayment(qint64 paymentId, QString* error = nullptr) override;
    QList<Attachment> getByContact(qint64 contactId, QString* error = nullptr) override;

private:
    static Attachment fromQuery(const QSqlQuery& query);

    QSqlDatabase m_db;
};