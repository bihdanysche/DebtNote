#pragma once

#include "domain/entities/Contact.h"
#include "domain/repositories/IContactRepository.h"
#include <QSqlDatabase>
#include <QSqlQuery>

class SQLiteContactRepository: public IContactRepository {
public:
    explicit SQLiteContactRepository(QSqlDatabase db);

    std::optional<qint64> add(Contact& contact, QString* error = nullptr) override;
    bool update(const Contact& contact, QString* error = nullptr) override;
    bool remove(qint64 id, QString* error = nullptr) override;

    std::optional<Contact> getById(qint64 id, QString* error = nullptr) override;
    QList<Contact> getAll(QString* error = nullptr) override;
private:
    static Contact fromQuery(const QSqlQuery& query);
    QSqlDatabase m_db;
};