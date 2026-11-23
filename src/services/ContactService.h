#pragma once

#include "domain/entities/Contact.h"
#include "domain/repositories/IContactRepository.h"
#include <qcoreapplication.h>
#include <qlist.h>
#include <qobject.h>
#include <qtmetamacros.h>

class ContactService : public QObject {
    Q_OBJECT;

public:
    explicit ContactService(IContactRepository& repo, QObject* parent = nullptr);

    bool add(Contact& contact, QString* error = nullptr);
    QList<Contact> getAll(QString* error = nullptr);

signals:
    void contactsChanged();

private:
    IContactRepository& m_repo;
};