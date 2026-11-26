#include "SQLitePaymentRepository.h"
#include "data/repositories/SQLiteContactRepository.h"
#include "domain/valueobjects/PaymentType.h"

#include <QSqlError>
#include <QVariant>

SQLitePaymentRepository::SQLitePaymentRepository(QSqlDatabase db)
    : m_db(db)
{}

std::optional<qint64> SQLitePaymentRepository::add(const Payment& payment, QString* error) {
    QSqlQuery query(m_db);

    query.prepare(R"(
        INSERT INTO payments (contact_id, amount, type, date_time, comment)
        VALUES (:contact_id, :amount, :type, :date_time, :comment)
    )");

    query.bindValue(":contact_id", payment.contactId());
    query.bindValue(":amount", payment.amount().kopecks());
    query.bindValue(":type", static_cast<int>(payment.type()));
    query.bindValue(":date_time", payment.dateTime().toString(Qt::ISODate));
    query.bindValue(":comment", payment.comment());

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return std::nullopt;
    }

    return query.lastInsertId().toLongLong();
};

bool SQLitePaymentRepository::update(const Payment& payment, QString* error) {
    QSqlQuery query(m_db);

    query.prepare(R"(
        UPDATE payments
        SET contact_id = :contact_id,
            amount = :amount,
            type = :type,
            date_time = :date_time,
            comment = :comment
        WHERE id = :id
    )");

    query.bindValue(":contact_id", payment.contactId());
    query.bindValue(":amount", payment.amount().kopecks());
    query.bindValue(":type", static_cast<int>(payment.type()));
    query.bindValue(":date_time", payment.dateTime().toString(Qt::ISODate));
    query.bindValue(":comment", payment.comment());
    query.bindValue(":id", payment.id());

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return false;
    }

    return true;
};

bool SQLitePaymentRepository::remove(qint64 id, QString* error) {
    QSqlQuery query(m_db);

    query.prepare(R"(
        DELETE FROM payments
        WHERE id = :id
    )");

    query.bindValue(":id", id);

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return false;
    }

    return true;
};

std::optional<Payment> SQLitePaymentRepository::getById(qint64 id, QString* error) {
    QSqlQuery query(m_db);

    query.prepare(R"(
        SELECT id, contact_id, amount, type, date_time, comment
        FROM payments
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

QList<Payment> SQLitePaymentRepository::getByContact(qint64 contactId, QString* error) {
    QSqlQuery query(m_db);

    query.prepare(R"(
        SELECT id, contact_id, amount, type, date_time, comment
        FROM payments
        WHERE contact_id = :contact_id
        ORDER BY date_time DESC, id DESC
    )");

    query.bindValue(":contact_id", contactId);

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return {};
    }

    QList<Payment> payments;
    while (query.next()) {
        payments.append(fromQuery(query));
    }

    return payments;
};

QList<Payment> SQLitePaymentRepository::getAll(QString* error) {
    QSqlQuery query(m_db);

    query.prepare(R"(
        SELECT id, contact_id, amount, type, date_time, comment
        FROM payments
        ORDER BY date_time DESC, id DESC
    )");

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return {};
    }

    QList<Payment> payments;
    while (query.next()) {
        payments.append(fromQuery(query));
    }

    return payments;
};

QHash<Contact, Money> SQLitePaymentRepository::balancesByContact(QString* error) {
    QHash<Contact, Money> result;
    QSqlQuery query(m_db);

    if (!query.exec(R"(
        SELECT
            c.id,
            c.first_name,
            c.last_name,
            c.note,
            c.created_at,
            COALESCE(SUM(
                CASE
                    WHEN p.type IN (0, 3) THEN p.amount
                    WHEN p.type IN (1, 2) THEN -p.amount
                    ELSE 0
                END
            ), 0) AS balance
        FROM contacts c
        LEFT JOIN payments p ON p.contact_id = c.id
        GROUP BY c.id
    )")) {
        const QString msg = query.lastError().text();
        qWarning() << "SQLitePaymentRepository::balancesByContact failed:" << msg;
        if (error) *error = msg;
        return {};
    }

    while (query.next()) {
        Contact contact = SQLiteContactRepository::fromQuery(query);
        Money balance(query.value("balance").toLongLong());

        result.insert(contact, balance);
    }

    return result;
}

Payment SQLitePaymentRepository::fromQuery(const QSqlQuery& query) {
    Payment payment;

    payment.setId(query.value("id").toLongLong());
    payment.setContactId(query.value("contact_id").toLongLong());
    payment.setAmount(Money(query.value("amount").toLongLong()));
    payment.setType(static_cast<PaymentType>(query.value("type").toInt()));
    payment.setDateTime(QDateTime::fromString(query.value("date_time").toString(), Qt::ISODate));
    payment.setComment(query.value("comment").toString());

    return payment;
};