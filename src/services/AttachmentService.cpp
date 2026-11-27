#include "AttachmentService.h"

#include <QDebug>
#include <QFileInfo>

AttachmentService::AttachmentService(IAttachmentRepository& repo, IFileStorage& storage, QObject* parent)
    : QObject(parent)
    , m_repo(repo)
    , m_storage(storage)
{}

bool AttachmentService::attach(qint64 paymentId, const QString& sourcePath, QString* error) {
    if (paymentId <= 0) {
        if (error) *error = "Платіж ще не збережено";
        return false;
    }

    QString storageError;
    const std::optional<QString> storedPath = m_storage.copyIn(sourcePath, &storageError);
    if (!storedPath) {
        qWarning() << "AttachmentService::attach failed: " << storageError;
        if (error) *error = storageError;
        return false;
    }

    const Attachment attachment(paymentId, QFileInfo(sourcePath).fileName(), *storedPath);

    const QStringList problems = attachment.validate();
    if (!problems.isEmpty()) {
        m_storage.removeCopy(*storedPath);
        if (error) *error = problems.join("\n");
        return false;
    }

    QString dbError;
    if (!m_repo.add(attachment, &dbError)) {
        qWarning() << "AttachmentService::attach failed: " << dbError;
        m_storage.removeCopy(*storedPath);
        if (error) *error = "Помилка бази даних.";
        return false;
    }

    emit attachmentsChanged(paymentId);
    return true;
}

bool AttachmentService::remove(qint64 id, QString* error) {
    QString dbError;
    const std::optional<Attachment> attachment = m_repo.getById(id, &dbError);

    if (!attachment) {
        if (!dbError.isEmpty()) {
            qWarning() << "AttachmentService::remove failed: " << dbError;
            if (error) *error = "Помилка бази даних.";
        } else if (error) {
            *error = "Вкладення не знайдено";
        }
        return false;
    }

    if (!m_repo.remove(id, &dbError)) {
        qWarning() << "AttachmentService::remove failed: " << dbError;
        if (error) *error = "Помилка бази даних.";
        return false;
    }

    removeFiles(QList<Attachment>{*attachment});

    emit attachmentsChanged(attachment->paymentId());
    return true;
}

std::optional<Attachment> AttachmentService::getById(qint64 id, QString* error) {
    QString dbError;
    const std::optional<Attachment> attachment = m_repo.getById(id, &dbError);

    if (!attachment && !dbError.isEmpty()) {
        qWarning() << "AttachmentService::getById failed: " << dbError;
        if (error) *error = "Помилка бази даних.";
    }

    return attachment;
}

QList<Attachment> AttachmentService::getByPayment(qint64 paymentId, QString* error) {
    QString dbError;
    const QList<Attachment> attachments = m_repo.getByPayment(paymentId, &dbError);

    if (!dbError.isEmpty()) {
        qWarning() << "AttachmentService::getByPayment failed: " << dbError;
        if (error) *error = "Помилка бази даних.";
        return {};
    }

    return attachments;
}

QList<Attachment> AttachmentService::getByContact(qint64 contactId, QString* error) {
    QString dbError;
    const QList<Attachment> attachments = m_repo.getByContact(contactId, &dbError);

    if (!dbError.isEmpty()) {
        qWarning() << "AttachmentService::getByContact failed: " << dbError;
        if (error) *error = "Помилка бази даних.";
        return {};
    }

    return attachments;
}

void AttachmentService::removeFiles(const QList<Attachment>& attachments) {
    for (const Attachment& attachment : attachments) {
        QString storageError;
        if (!m_storage.removeCopy(attachment.storedPath(), &storageError)) {
            qWarning() << "AttachmentService::removeFiles failed: " << attachment.storedPath() << storageError;
        }
    }
}