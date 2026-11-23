#include "SQLiteContactRepository.h"
#include "domain/entities/Contact.h"
#include <qcoreapplication.h>
#include <qlist.h>
#include <qnamespace.h>
#include <qsqlquery.h>
#include <QSqlError>

SQLiteContactRepository::SQLiteContactRepository(QSqlDatabase db)
    : m_db(db)
{ }

std::optional<qint64> SQLiteContactRepository::add(Contact& contact, QString* error) {
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO contacts (id, first_name, last_name, note, created_at)
        VALUES (:id, :first_name, :last_name, :note, :created_at)
    )");

    query.bindValue(":first_name", contact.firstName());
    query.bindValue(":last_name", contact.lastName());
    query.bindValue(":note", contact.note());
    query.bindValue(":created_at", contact.createdAt().toString(Qt::ISODate));

    if (!query.exec()) {
        if (error) *error = query.lastError().text();
        return std::nullopt;
    }

    return query.lastInsertId().toLongLong();
};

bool SQLiteContactRepository::update(const Contact& contact, QString* error) {
    return false;
};

bool SQLiteContactRepository::remove(qint64 id, QString* error) {
    return false;
};

std::optional<Contact> SQLiteContactRepository::getById(qint64 id, QString* error) {
    std::optional<Contact> c;
    return c;
};

QList<Contact> SQLiteContactRepository::getAll(QString* error) {
    QList<Contact> result;
    QSqlQuery query(m_db);

    query.exec(R"(
        SELECT id, first_name, last_name, note, created_at
        FROM contacts
    )");

    if (!query.exec()) {
        const QString msg = query.lastError().text();
        qWarning() << "SQLiteContactRepository::getAll failed: " << msg;
        if (error) *error = msg;
        return {};
    }

    while (query.next()) {
        Contact contact = fromQuery(query);
        result.append(contact);
    }

    return result;
};

Contact SQLiteContactRepository::fromQuery(const QSqlQuery& query) {
    Contact contact;

    contact.setId(query.value(0).toLongLong());
    contact.setFirstName(query.value(1).toString());
    contact.setLastName(query.value(2).toString());
    contact.setNote(query.value(3).toString());
    contact.setCreatedAt(QDateTime::fromString(query.value(4).toString(), Qt::ISODate));

    return contact;
}