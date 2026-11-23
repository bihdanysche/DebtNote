#include "Contact.h"
#include <qcontainerfwd.h>

Contact::Contact(
    const QString& firstName, const QString& lastName,
    const QString& note, const QDateTime& createdAt,
    qint64 id
) : m_firstName(firstName), m_lastName(lastName), m_note(note), m_createdAt(createdAt), m_id(id)
{};

QString Contact::displayName() const {
    if (m_lastName.isEmpty()) {
        return m_firstName;
    }

    QString result = m_firstName + " " + m_lastName;
    return result;
}

QStringList Contact::validate() const {
    QStringList errors;

    if (m_firstName.trimmed().isEmpty()) {
        errors << "Ім'я не може бути порожнім!";
    }

    if (!m_createdAt.isValid()) {
        errors << "Некоректна дата й час.";
    }

    return errors;
}

bool Contact::isValid() {
    return validate().isEmpty();
}