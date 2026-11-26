#include "BalanceService.h"

#include <QList>

#include "domain/entities/Payment.h"
#include "services/AppServices.h"
#include "services/PaymentService.h"

BalanceService::BalanceService(AppServices& services)
    : m_services(services)
{}

Money BalanceService::balanceForContact(qint64 contactId, QString* error) {
    Money balance;

    const QList<Payment> payments = m_services.paymentService->getByContact(contactId, error);
    for (const Payment& payment : payments) {
        balance += payment.signedAmount();
    }

    return balance;
}

QHash<qint64, Money> BalanceService::balancesByContact(QString* error) {
    QHash<qint64, Money> balances;

    const QList<Payment> payments = m_services.paymentService->getAll(error);
    for (const Payment& payment : payments) {
        balances[payment.contactId()] += payment.signedAmount();
    }

    return balances;
}

BalanceSummary BalanceService::summary(QString* error) {
    BalanceSummary result;

    const QHash<qint64, Money> balances = balancesByContact(error);
    for (const Money& balance : balances) {
        if (balance.isPositive()) {
            result.owedToMe += balance;
        } else if (balance.isNegative()) {
            result.iOwe += -balance;
        }
    }

    result.net = result.owedToMe - result.iOwe;
    return result;
}