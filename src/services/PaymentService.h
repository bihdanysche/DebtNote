#pragma once

#include <QList>
#include <QObject>
#include <optional>
#include <qcontainerfwd.h>

#include "domain/entities/Payment.h"
#include "domain/repositories/IPaymentRepository.h"

class AttachmentService;

class PaymentService : public QObject {
    Q_OBJECT

public:
    explicit PaymentService(IPaymentRepository& repo, AttachmentService& attachments, QObject* parent = nullptr);

    bool add(const Payment& payment, const QStringList& newFiles = {}, QString* error = nullptr);
    bool update(const Payment& payment, const QStringList& newFiles = {}, const QList<qint64>& removedIds = {}, QString* error = nullptr);
    bool remove(qint64 id, QString* error = nullptr);

    std::optional<Payment> getById(qint64 id, QString* error = nullptr);
    QList<Payment> getByContact(qint64 contactId, QString* error = nullptr);
    QList<Payment> getAll(QString* error = nullptr);

signals:
    void paymentAdded(const Payment& payment);
    void paymentUpdated(const Payment& payment);
    void paymentRemoved(qint64 paymentId, qint64 contactId);
    void paymentsChanged();

private:
    void syncAttachments(qint64 paymentId, const QStringList& newFiles, const QList<qint64>& removedIds, QString* error);

    IPaymentRepository& m_repo;
    AttachmentService& m_attachments;
};