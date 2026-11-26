#include "Payment.h"

Payment::Payment(
    qint64 contactId, const Money& amount,
    PaymentType type,
    const QDateTime& dateTime,
    const QString& comment,
    qint64 id
)
    : m_id(id),
      m_contactId(contactId),
      m_amount(amount),
      m_type(type),
      m_dateTime(dateTime),
      m_comment(comment)
{}

Money Payment::signedAmount() const {
    switch (m_type) {
        case PaymentType::Lent:
        case PaymentType::PaidBack:
            return m_amount;
        case PaymentType::Borrowed:
        case PaymentType::ReceivedBack:
            return -m_amount;
        }
    return Money();
}

QStringList Payment::validate() const {
    QStringList errors;

    if (m_contactId <= 0) {
        errors << "Не прив'язано до контакту";
    }
    if (!m_amount.isPositive()) {
        errors << "Сума має бути більшою за нуль";
    }
    if (!m_dateTime.isValid()) {
        errors << "Некоректна дата й час";
    }

    return errors;
}