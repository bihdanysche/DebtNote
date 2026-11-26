#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>

#include "domain/entities/Contact.h"
#include "domain/repositories/IPaymentRepository.h"

class SQLitePaymentRepository : public IPaymentRepository {
public:
    explicit SQLitePaymentRepository(QSqlDatabase db);

    std::optional<qint64> add(const Payment& payment, QString* error = nullptr) override;
    bool update(const Payment& payment, QString* error = nullptr) override;
    bool remove(qint64 id, QString* error = nullptr) override;

    std::optional<Payment> getById(qint64 id, QString* error = nullptr) override;
    QList<Payment> getByContact(qint64 contactId, QString* error = nullptr) override;
    QList<Payment> getAll(QString* error = nullptr) override;

    QHash<Contact, Money> balancesByContact(QString* error) override;
    
    static Payment fromQuery(const QSqlQuery& query);
private:
    QSqlDatabase m_db;
};