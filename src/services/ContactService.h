#pragma once

#include "domain/entities/Contact.h"
#include "domain/repositories/IContactRepository.h"
#include <qcoreapplication.h>
#include <qlist.h>
#include <qobject.h>
#include <qtmetamacros.h>

class AttachmentService;

class ContactService : public QObject {
    Q_OBJECT;

public:
    explicit ContactService(IContactRepository& repo, AttachmentService& attachments, QObject* parent = nullptr);

    bool add(Contact& contact, QString* error = nullptr);
    bool update(const Contact& contact, QString* error = nullptr);
    bool remove(qint64 id, QString* error = nullptr);
    std::optional<Contact> getById(qint64 id, QString* error = nullptr);
    QList<Contact> getAll(QString* error = nullptr);

signals:
    void contactsChanged();
    void contactRemoved(qint64 id);

private:
    IContactRepository& m_repo;
    AttachmentService& m_attachments;
};