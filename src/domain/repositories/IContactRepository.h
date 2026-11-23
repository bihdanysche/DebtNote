#pragma once

#include "domain/entities/Contact.h"
#include <optional>
#include <qcoreapplication.h>
#include <qlist.h>
#include <qtypes.h>

class IContactRepository {
public:
    virtual ~IContactRepository() = default;

    virtual std::optional<qint64> add(Contact& contact, QString* error = nullptr) = 0;
    virtual bool update(const Contact& contact, QString* error = nullptr) = 0;
    virtual bool remove(qint64 id, QString* error = nullptr) = 0;

    virtual std::optional<Contact> getById(qint64 id, QString* error = nullptr) = 0;
    virtual QList<Contact> getAll(QString* error = nullptr) = 0;
};