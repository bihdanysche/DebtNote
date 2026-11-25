#pragma once

#include <QDateTime>
#include <QString>
#include <QStringList>

#include "domain/valueobjects/Money.h"
#include "domain/valueobjects/PaymentDirection.h"

class Payment {
public:
    Payment() = default;
    Payment(qint64 contactId,
            const Money& amount,
            PaymentDirection direction = PaymentDirection::Lent,
            const QDateTime& dateTime = QDateTime::currentDateTime(),
            const QString& comment = QString(),
            qint64 id = 0);

    qint64 id() const { return m_id; }
    void setId(qint64 id) { m_id = id; }

    qint64 contactId() const { return m_contactId; }
    void setContactId(qint64 contactId) { m_contactId = contactId; }

    const Money& amount() const { return m_amount; }
    void setAmount(const Money& amount) { m_amount = amount; }

    PaymentDirection direction() const { return m_direction; }
    void setDirection(PaymentDirection direction) { m_direction = direction; }

    const QDateTime& dateTime() const { return m_dateTime; }
    void setDateTime(const QDateTime& dateTime) { m_dateTime = dateTime; }

    const QString& comment() const { return m_comment; }
    void setComment(const QString& comment) { m_comment = comment; }

    Money signedAmount() const;
    QStringList validate() const;

private:
    qint64 m_id = 0;
    qint64 m_contactId = 0;
    Money m_amount;
    PaymentDirection m_direction = PaymentDirection::Lent;
    QDateTime m_dateTime = QDateTime::currentDateTime();
    QString m_comment;
};