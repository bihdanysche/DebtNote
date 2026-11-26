#pragma once

#include <QHash>
#include <QString>

#include "domain/entities/Contact.h"
#include "domain/repositories/IPaymentRepository.h"
#include "domain/valueobjects/Money.h"

struct AppServices;

struct BalanceSummary {
    Money owedToMe;
    Money iOwe;
    Money net;
};

class BalanceService {
public:
    explicit BalanceService(IPaymentRepository& paymentRepo, AppServices& services);

    Money balanceForContact(qint64 contactId, QString* error = nullptr);
    QHash<Contact, Money> balancesByContact(QString* error = nullptr);
    BalanceSummary summary(QString* error = nullptr);

private:
    IPaymentRepository& m_paymentRepo;
    AppServices& m_services;
};