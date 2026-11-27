#include "ContactService.h"
#include "domain/entities/Attachment.h"
#include "domain/entities/Contact.h"
#include "domain/repositories/IContactRepository.h"
#include "services/AttachmentService.h"
#include "services/ErrorMessages.h"
#include <optional>
#include <qcontainerfwd.h>
#include <qcoreapplication.h>
#include <qlist.h>
#include <qlogging.h>
#include <qobject.h>
#include <qtypes.h>

ContactService::ContactService(IContactRepository& repo, AttachmentService& attachments, QObject* parent)
    : m_repo(repo), m_attachments(attachments), QObject(parent)
{ }

bool ContactService::add(Contact& contact, QString* error) {
    QStringList problems = contact.validate();

    if (!problems.isEmpty()) {
        if (error) *error = problems[0];
        return false;
    }

    QString dbError;
    std::optional<qint64> id = m_repo.add(contact, &dbError);

    if (!id.has_value()) {
        qWarning() << "ContactService::add failed: " << dbError;
        if (error) *error = kDatabaseError;
        return false;
    }

    contact.setId(id.value());
    emit contactsChanged();
    return true;
}

std::optional<Contact> ContactService::getById(qint64 id, QString* error) {
    QString dbError;
    std::optional<Contact> result = m_repo.getById(id, &dbError);

    if (!dbError.isEmpty()) {
        qWarning() << "ContactService::getById failed: " << dbError;
        if (error) *error = kDatabaseError;
        return {};
    }

    return result;
}

QList<Contact> ContactService::getAll(QString* error) {
    QString dbError;
    QList<Contact> result = m_repo.getAll(&dbError);

    if (!dbError.isEmpty()) {
        qWarning() << "ContactService::getAll failed: " << dbError;
        if (error) *error = kDatabaseError;
        return {};
    }

    return result;
}

bool ContactService::update(const Contact& contact, QString* error) {
    QStringList problems = contact.validate();

    if (!problems.isEmpty()) {
        if (error) *error = problems[0];
        return false;
    }

    QString dbError;

    if (!m_repo.update(contact, &dbError)) {
        if (error) *error = kDatabaseError;
        qWarning() << "ContactService::update failed: " << dbError;
        return false;
    }

    emit contactsChanged();
    return true;
}

bool ContactService::remove(qint64 id, QString* error) {
    QString attachmentsError;
    const QList<Attachment> attachments = m_attachments.getByContact(id, &attachmentsError);
    if (!attachmentsError.isEmpty()) {
        if (error) *error = attachmentsError;
        return false;
    }

    QString dbError;
    if (!m_repo.remove(id, &dbError)) {
        qWarning() << "ContactService::remove failed: " << dbError;
        if (error) *error = "Помилка бази даних.";
        return false;
    }

    m_attachments.removeFiles(attachments);

    emit contactsChanged();
    return true;
}