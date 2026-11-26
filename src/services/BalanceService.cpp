#include "BalanceService.h"

#include <QList>
#include <qhashfunctions.h>
#include <qlogging.h>

#include "services/AppServices.h"
#include "services/ErrorMessages.h"
#include "services/PaymentService.h"

BalanceService::BalanceService(IPaymentRepository& paymentRepo, AppServices& services)
    : m_services(services), m_paymentRepo(paymentRepo)
{}

Money BalanceService::balanceForContact(qint64 contactId, QString* error) {
    Money balance;

    const QList<Payment> payments = m_services.paymentService->getByContact(contactId, error);
    for (const Payment& payment : payments) {
        balance += payment.signedAmount();
    }

    return balance;
}

QHash<Contact, Money> BalanceService::balancesByContact(QString* error) {
    QString msg;
    
    auto result = m_paymentRepo.balancesByContact(&msg);

    if (!msg.isEmpty()) {
        if (error) *error = kDatabaseError;
        qWarning() << "BalanceService::balancesByContact failed: " << msg;
        return {};
    }

    return result;
}

BalanceSummary BalanceService::summary(QString* error) {
    BalanceSummary result;

    const QHash<Contact, Money> balances = balancesByContact(error);
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