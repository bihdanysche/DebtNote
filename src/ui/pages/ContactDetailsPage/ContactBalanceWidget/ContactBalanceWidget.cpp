#include "ContactBalanceWidget.h"
#include "domain/valueobjects/Money.h"
#include "services/BalanceService.h"
#include "ui/common/toasts/toast.h"
#include "ui/mainwindow.h"
#include <qboxlayout.h>
#include <qstyle.h>

ContactBalanceWidget::ContactBalanceWidget(MainWindow* mv)
    : m_toast(mv->uiInstances().toast)
    , m_balanceService(mv->services().balanceService)
{
    setObjectName("BalanceContainer");

    auto* balanceLayout = new QVBoxLayout(this);
    balanceLayout->setContentsMargins(16, 14, 16, 14);
    balanceLayout->setSpacing(6);

    auto* balanceTitle = new QLabel("Поточний баланс", this);
    balanceTitle->setProperty("role", "section");

    m_balanceAmount = new QLabel(Money().toString(), this);
    m_balanceAmount->setObjectName("BalanceAmount");

    m_balanceDescription = new QLabel("", this);
    m_balanceDescription->setObjectName("BalanceDescription");

    balanceLayout->addWidget(balanceTitle);
    balanceLayout->addWidget(m_balanceAmount);
    balanceLayout->addWidget(m_balanceDescription);
}

void ContactBalanceWidget::reload() {
    if (m_contactId <= 0) {
        reset();
        return;
    }

    QString error;
    Money balance = m_balanceService->balanceForContact(m_contactId, &error);

    if (!error.isEmpty()) {
        m_toast->makeToast(error);
        reset();
        return;
    }

    double amount = balance.toHryvnias();
    double absoluteAmount = std::abs(amount);

    m_balanceAmount->setText(
        balance.toString()
    );

    if (amount > 0) {
        this->setProperty("balance", "positive");
        m_balanceAmount->setProperty("balance", "positive");
        m_balanceDescription->setText("Ця людина винна вам");
    }
    else if (amount < 0) {
        this->setProperty("balance", "negative");
        m_balanceAmount->setProperty("balance", "negative");
        m_balanceDescription->setText("Ви винні цій людині");
    }
    else {
        this->setProperty("balance", "neutral");
        m_balanceAmount->setProperty("balance", "neutral");
        m_balanceDescription->setText("Взаєморозрахунки закриті");
    }

    this->style()->unpolish(this);
    this->style()->polish(this);
    m_balanceAmount->style()->unpolish(m_balanceAmount);
    m_balanceAmount->style()->polish(m_balanceAmount);
}

void ContactBalanceWidget::reset() {
    m_balanceAmount->setText("??? грн");
    this->setProperty("balance", "neutral");
    m_balanceAmount->setProperty("balance", "neutral");
    m_balanceDescription->setText("Взаєморозрахунки закриті");

    this->style()->unpolish(this);
    this->style()->polish(this);
    m_balanceAmount->style()->unpolish(m_balanceAmount);
    m_balanceAmount->style()->polish(m_balanceAmount);
}