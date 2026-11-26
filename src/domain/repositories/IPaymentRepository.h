#pragma once

#include <QList>
#include <QString>
#include <optional>

#include "domain/entities/Contact.h"
#include "domain/entities/Payment.h"

class IPaymentRepository {
public:
    virtual ~IPaymentRepository() = default;

    virtual std::optional<qint64> add(const Payment& payment, QString* error = nullptr) = 0;
    virtual bool update(const Payment& payment, QString* error = nullptr) = 0;
    virtual bool remove(qint64 id, QString* error = nullptr) = 0;

    virtual std::optional<Payment> getById(qint64 id, QString* error = nullptr) = 0;
    virtual QList<Payment> getByContact(qint64 contactId, QString* error = nullptr) = 0;
    virtual QList<Payment> getAll(QString* error = nullptr) = 0;
    
    virtual QHash<Contact, Money> balancesByContact(QString* error) = 0;
};