#pragma once

#include <QDateTime>
#include <QString>
#include <QStringList>

class Attachment {
public:
    Attachment() = default;
    Attachment(qint64 paymentId, const QString& fileName,
               const QString& storedPath,
               const QDateTime& addedAt = QDateTime::currentDateTime(),
               qint64 id = 0);

    qint64 id() const { return m_id; }
    void setId(qint64 id) { m_id = id; }

    qint64 paymentId() const { return m_paymentId; }
    void setPaymentId(qint64 paymentId) { m_paymentId = paymentId; }

    const QString& fileName() const { return m_fileName; }
    void setFileName(const QString& fileName) { m_fileName = fileName; }

    const QString& storedPath() const { return m_storedPath; }
    void setStoredPath(const QString& storedPath) { m_storedPath = storedPath; }

    const QDateTime& addedAt() const { return m_addedAt; }
    void setAddedAt(const QDateTime& addedAt) { m_addedAt = addedAt; }

    QStringList validate() const;

private:
    qint64 m_id = 0;
    qint64 m_paymentId = 0;
    QString m_fileName;
    QString m_storedPath;
    QDateTime m_addedAt = QDateTime::currentDateTime();
};