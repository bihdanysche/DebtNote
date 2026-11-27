#include "PaymentService.h"
#include "domain/entities/Attachment.h"
#include "services/AttachmentService.h"

#include <QDebug>
#include <qfileinfo.h>

PaymentService::PaymentService(IPaymentRepository& repo, AttachmentService& attachments, QObject* parent)
    : QObject(parent)
    , m_repo(repo)
    , m_attachments(attachments)
{}

bool PaymentService::add(const Payment& payment, const QStringList& newFiles, QString* error) {
    const QStringList problems = payment.validate();
    if (!problems.isEmpty()) {
        if (error) *error = problems.join("\n");
        return false;
    }

    QString dbError;
    const std::optional<qint64> id = m_repo.add(payment, &dbError);
    if (!id) {
        qWarning() << "PaymentService::add failed: " << dbError;
        if (error) *error = "Помилка бази даних.";
        return false;
    }

    Payment saved = payment;
    saved.setId(*id);
    syncAttachments(*id, newFiles, {}, error);

    emit paymentAdded(saved);
    emit paymentsChanged();
    return true;
}

bool PaymentService::update(const Payment& payment, const QStringList& newFiles, const QList<qint64>& removedIds, QString* error) {
    if (payment.id() <= 0) {
        if (error) *error = "Платіж ще не збережено";
        return false;
    }

    const QStringList problems = payment.validate();
    if (!problems.isEmpty()) {
        if (error) *error = problems.join("\n");
        return false;
    }

    QString dbError;
    if (!m_repo.update(payment, &dbError)) {
        qWarning() << "PaymentService::update failed: " << dbError;
        if (error) *error = "Помилка бази даних.";
        return false;
    }

    syncAttachments(payment.id(), newFiles, removedIds, error);

    emit paymentUpdated(payment);
    emit paymentsChanged();
    return true;
}

bool PaymentService::remove(qint64 id, QString* error) {
    QString dbError;
    const std::optional<Payment> payment = m_repo.getById(id, &dbError);

    if (!payment) {
        if (!dbError.isEmpty()) {
            qWarning() << "PaymentService::remove failed: " << dbError;
            if (error) *error = "Помилка бази даних.";
        } else if (error) {
            *error = "Платіж не знайдено";
        }
        return false;
    }

    QString attachmentsError;
    const QList<Attachment> attachments = m_attachments.getByPayment(id, &attachmentsError);
    if (!attachmentsError.isEmpty()) {
        if (error) *error = attachmentsError;
        return false;
    }

    if (!m_repo.remove(id, &dbError)) {
        qWarning() << "PaymentService::remove failed: " << dbError;
        if (error) *error = "Помилка бази даних.";
        return false;
    }
    
    m_attachments.removeFiles(attachments);

    emit paymentRemoved(id, payment->contactId());
    emit paymentsChanged();
    return true;
}

std::optional<Payment> PaymentService::getById(qint64 id, QString* error) {
    QString dbError;
    const std::optional<Payment> payment = m_repo.getById(id, &dbError);

    if (!payment && !dbError.isEmpty()) {
        qWarning() << "PaymentService::getById failed: " << dbError;
        if (error) *error = "Помилка бази даних.";
    }

    return payment;
}

QList<Payment> PaymentService::getByContact(qint64 contactId, QString* error) {
    QString dbError;
    const QList<Payment> payments = m_repo.getByContact(contactId, &dbError);

    if (!dbError.isEmpty()) {
        qWarning() << "PaymentService::getByContact failed: " << dbError;
        if (error) *error = "Помилка бази даних.";
        return {};
    }

    return payments;
}

QList<Payment> PaymentService::getAll(QString* error) {
    QString dbError;
    const QList<Payment> payments = m_repo.getAll(&dbError);

    if (!dbError.isEmpty()) {
        qWarning() << "PaymentService::getAll failed: " << dbError;
        if (error) *error = "Помилка бази даних.";
        return {};
    }

    return payments;
}

void PaymentService::syncAttachments(qint64 paymentId, const QStringList& newFiles, const QList<qint64>& removedIds, QString* error) {
    int failedRemovals = 0;
    QStringList failedFiles;

    for (qint64 id : removedIds) {
        const std::optional<Attachment> attachment = m_attachments.getById(id);
        if (!attachment || attachment->paymentId() != paymentId || !m_attachments.remove(id)) {
            ++failedRemovals;
        }
    }

    for (const QString& path : newFiles) {
        if (!m_attachments.attach(paymentId, path)) {
            failedFiles << QFileInfo(path).fileName();
        }
    }

    if (failedRemovals == 0 && failedFiles.isEmpty()) {
        return;
    }

    QStringList parts;
    if (failedRemovals > 0) {
        parts << QString("Не вдалося видалити вкладень: %1").arg(failedRemovals);
    }
    if (!failedFiles.isEmpty()) {
        parts << "Не вдалося додати файли: " + failedFiles.join(", ");
    }

    if (error) {
        *error = "Платіж збережено, але " + parts.join("; ");
    }
}