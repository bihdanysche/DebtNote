#include "SQLiteAttachmentRepository.h"

#include <QSqlError>
#include <QVariant>

SQLiteAttachmentRepository::SQLiteAttachmentRepository(QSqlDatabase db)
    : m_db(db)
{}

std::optional<qint64> SQLiteAttachmentRepository::add(const Attachment& attachment, QString* error) {
    QSqlQuery query(m_db);

    query.prepare(R"(
        INSERT INTO attachments (payment_id, file_name, stored_path, added_at)
        VALUES (:payment_id, :file_name, :stored_path, :added_at)
    )");

    query.bindValue(":payment_id", attachment.paymentId());
    query.bindValue(":file_name", attachment.fileName());
    query.bindValue(":stored_path", attachment.storedPath());
    query.bindValue(":added_at", attachment.addedAt().toString(Qt::ISODate));

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return std::nullopt;
    }

    return query.lastInsertId().toLongLong();
};

bool SQLiteAttachmentRepository::remove(qint64 id, QString* error) {
    QSqlQuery query(m_db);

    query.prepare(R"(
        DELETE FROM attachments
        WHERE id = :id
    )");

    query.bindValue(":id", id);

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return false;
    }

    return true;
};

std::optional<Attachment> SQLiteAttachmentRepository::getById(qint64 id, QString* error) {
    QSqlQuery query(m_db);

    query.prepare(R"(
        SELECT id, payment_id, file_name, stored_path, added_at
        FROM attachments
        WHERE id = :id
    )");

    query.bindValue(":id", id);

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return std::nullopt;
    }

    if (!query.next()) {
        return std::nullopt;
    }

    return fromQuery(query);
};

QList<Attachment> SQLiteAttachmentRepository::getByPayment(qint64 paymentId, QString* error) {
    QSqlQuery query(m_db);

    query.prepare(R"(
        SELECT id, payment_id, file_name, stored_path, added_at
        FROM attachments
        WHERE payment_id = :payment_id
        ORDER BY added_at, id
    )");

    query.bindValue(":payment_id", paymentId);

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return {};
    }

    QList<Attachment> attachments;
    while (query.next()) {
        attachments.append(fromQuery(query));
    }

    return attachments;
};

QList<Attachment> SQLiteAttachmentRepository::getByContact(qint64 contactId, QString* error) {
    QSqlQuery query(m_db);

    query.prepare(R"(
        SELECT a.id, a.payment_id, a.file_name, a.stored_path, a.added_at
        FROM attachments a
        JOIN payments p ON p.id = a.payment_id
        WHERE p.contact_id = :contact_id
    )");

    query.bindValue(":contact_id", contactId);

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return {};
    }

    QList<Attachment> attachments;
    while (query.next()) {
        attachments.append(fromQuery(query));
    }

    return attachments;
};

Attachment SQLiteAttachmentRepository::fromQuery(const QSqlQuery& query) {
    Attachment attachment;

    attachment.setId(query.value("id").toLongLong());
    attachment.setPaymentId(query.value("payment_id").toLongLong());
    attachment.setFileName(query.value("file_name").toString());
    attachment.setStoredPath(query.value("stored_path").toString());
    attachment.setAddedAt(QDateTime::fromString(query.value("added_at").toString(), Qt::ISODate));

    return attachment;
};