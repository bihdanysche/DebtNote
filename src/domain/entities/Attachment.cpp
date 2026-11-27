#include "Attachment.h"

Attachment::Attachment(
    qint64 paymentId, const QString& fileName,
    const QString& storedPath, const QDateTime& addedAt,
    qint64 id
)
    : m_id(id)
    , m_paymentId(paymentId)
    , m_fileName(fileName)
    , m_storedPath(storedPath)
    , m_addedAt(addedAt)
{}

QStringList Attachment::validate() const {
    QStringList errors;

    if (m_paymentId <= 0) {
        errors << "Не вибрано платіж";
    }
    if (m_fileName.trimmed().isEmpty()) {
        errors << "Не вказано назву файлу";
    }
    if (m_storedPath.trimmed().isEmpty()) {
        errors << "Не вказано шлях до файлу";
    }

    return errors;
}