#pragma once

#include <qcontainerfwd.h>
#include <qcoreapplication.h>
#include <qdatetime.h>
#include <qobject.h>
#include <qtypes.h>
#include <QDateTime>

class Contact {
public:
    Contact() = default;
    explicit Contact(
        const QString& firstName,
        const QString& lastName="",
        const QString& note="",
        const QDateTime& createdAt=QDateTime::currentDateTime(),
        qint64 id=0
    );

    QString displayName() const;
    QStringList validate() const;
    bool isValid();

    qint64 id() const { return m_id; };
    void setId(const qint64 id) { m_id = id; };

    const QString& firstName() const { return m_firstName; };
    void setFirstName(const QString& firstName) { m_firstName = firstName; };

    const QString& lastName() const { return m_lastName; };
    void setLastName(const QString& lastName) { m_lastName = lastName; };

    const QString& note() const { return m_note; };
    void setNote(const QString& note) { m_note = note; };

    const QDateTime& createdAt() const { return m_createdAt; };
    void setCreatedAt(const QDateTime& createdAt) { m_createdAt = createdAt; };
private:
    qint64 m_id = 0;
    QString m_firstName;
    QString m_lastName;
    QString m_note;
    QDateTime m_createdAt;
};