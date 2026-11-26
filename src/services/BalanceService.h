#pragma once

#include <QHash>
#include <QString>

#include "domain/valueobjects/Money.h"

struct AppServices;

struct BalanceSummary {
    Money owedToMe;
    Money iOwe;
    Money net;
};

class BalanceService {
public:
    explicit BalanceService(AppServices& services);

    Money balanceForContact(qint64 contactId, QString* error = nullptr);
    QHash<qint64, Money> balancesByContact(QString* error = nullptr);
    BalanceSummary summary(QString* error = nullptr);

private:
    AppServices& m_services;
};