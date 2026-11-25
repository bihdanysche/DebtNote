#include "PaymentService.h"

#include <QDebug>

PaymentService::PaymentService(IPaymentRepository& repo, QObject* parent)
    : QObject(parent)
    , m_repo(repo)
{}

bool PaymentService::add(const Payment& payment, QString* error) {
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

    emit paymentAdded(saved);
    emit paymentsChanged();
    return true;
}

bool PaymentService::update(const Payment& payment, QString* error) {
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

    if (!m_repo.remove(id, &dbError)) {
        qWarning() << "PaymentService::remove failed: " << dbError;
        if (error) *error = "Помилка бази даних.";
        return false;
    }

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