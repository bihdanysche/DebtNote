#include "ContactService.h"
#include "domain/entities/Contact.h"
#include "domain/repositories/IContactRepository.h"
#include "services/ErrorMessages.h"
#include <optional>
#include <qcontainerfwd.h>
#include <qcoreapplication.h>
#include <qlist.h>
#include <qlogging.h>
#include <qobject.h>
#include <qtypes.h>

ContactService::ContactService(IContactRepository& repo, QObject* parent)
    : m_repo(repo), QObject(parent)
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