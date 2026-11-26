#pragma once

#include <QDateTime>
#include <QString>
#include <QStringList>

#include "domain/valueobjects/Money.h"
#include "domain/valueobjects/PaymentType.h"

class Payment {
public:
    Payment() = default;
    Payment(qint64 contactId,
            const Money& amount,
            PaymentType direction = PaymentType::Lent,
            const QDateTime& dateTime = QDateTime::currentDateTime(),
            const QString& comment = QString(),
            qint64 id = 0);

    qint64 id() const { return m_id; }
    void setId(qint64 id) { m_id = id; }

    qint64 contactId() const { return m_contactId; }
    void setContactId(qint64 contactId) { m_contactId = contactId; }

    const Money& amount() const { return m_amount; }
    void setAmount(const Money& amount) { m_amount = amount; }

    PaymentType type() const { return m_type; }
    void setType(PaymentType type) { m_type = type; }

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
    PaymentType m_type = PaymentType::Lent;
    QDateTime m_dateTime = QDateTime::currentDateTime();
    QString m_comment;
};