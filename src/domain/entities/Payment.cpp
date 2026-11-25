#include "Payment.h"

Payment::Payment(
    qint64 contactId, const Money& amount,
    PaymentDirection direction,
    const QDateTime& dateTime,
    const QString& comment,
    qint64 id
)
    : m_id(id),
      m_contactId(contactId),
      m_amount(amount),
      m_direction(direction),
      m_dateTime(dateTime),
      m_comment(comment)
{}

Money Payment::signedAmount() const {
    return m_direction == PaymentDirection::Lent ? m_amount : -m_amount;
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