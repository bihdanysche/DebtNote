#include "BalanceWidget.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "ui/mainwindow.h"
#include "services/BalanceService.h"
#include "ui/common/toasts/toast.h"

BalanceWidget::BalanceWidget(MainWindow* mv)
    : QWidget(mv)
    , m_balanceService(mv->services().balanceService)
    , m_toast(mv->uiInstances().toast)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(10);

    QLabel* sectionTitle = new QLabel("Ваші фінанси", this);
    sectionTitle->setProperty("role", "section");

    mainLayout->addWidget(sectionTitle);

    // UP row
    QHBoxLayout* cardsLayout = new QHBoxLayout();
    cardsLayout->setContentsMargins(0, 0, 0, 0);
    cardsLayout->setSpacing(12);

    // Owed to me
    QFrame* owedToMeCard = new QFrame(this);
    owedToMeCard->setObjectName("SummaryCard");
    owedToMeCard->setProperty("variant", "positive");

    QVBoxLayout* owedToMeLayout = new QVBoxLayout(owedToMeCard);
    owedToMeLayout->setContentsMargins(16, 14, 16, 14);
    owedToMeLayout->setSpacing(6);

    QLabel* owedToMeTitle = new QLabel("Мені винні", owedToMeCard);
    owedToMeTitle->setProperty("role", "cardTitle");

    m_owedToMeAmount = new QLabel("0,00 ₴", owedToMeCard);
    m_owedToMeAmount->setObjectName("SummaryAmount");

    m_owedToMeDescription = new QLabel("Очікується на повернення", owedToMeCard);
    m_owedToMeDescription->setObjectName("SummaryDescription");

    owedToMeLayout->addWidget(owedToMeTitle);
    owedToMeLayout->addWidget(m_owedToMeAmount);
    owedToMeLayout->addWidget(m_owedToMeDescription);

    // I owe
    QFrame* iOweCard = new QFrame(this);
    iOweCard->setObjectName("SummaryCard");
    iOweCard->setProperty("variant", "negative");

    QVBoxLayout* iOweLayout = new QVBoxLayout(iOweCard);
    iOweLayout->setContentsMargins(16, 14, 16, 14);
    iOweLayout->setSpacing(6);

    QLabel* iOweTitle = new QLabel("Я винен", iOweCard);
    iOweTitle->setProperty("role", "cardTitle");

    m_iOweAmount = new QLabel("0,00 ₴", iOweCard);
    m_iOweAmount->setObjectName("SummaryAmount");

    m_iOweDescription = new QLabel("Потрібно повернути", iOweCard);
    m_iOweDescription->setObjectName("SummaryDescription");

    iOweLayout->addWidget(iOweTitle);
    iOweLayout->addWidget(m_iOweAmount);
    iOweLayout->addWidget(m_iOweDescription);

    cardsLayout->addWidget(owedToMeCard);
    cardsLayout->addWidget(iOweCard);

    mainLayout->addLayout(cardsLayout);
}

void BalanceWidget::reset() {
    m_owedToMeAmount->setText("—");
    m_iOweAmount->setText("—");

    m_owedToMeDescription->setText("Недоступно");
    m_iOweDescription->setText("Недоступно");
}

void BalanceWidget::reload() {
    QString error;

    BalanceSummary summary = m_balanceService->summary(&error);

    if (!error.isEmpty()) {
        m_toast->makeToast(error, 2500);
        reset();
        return;
    }

    m_owedToMeAmount->setText(summary.owedToMe.toString());
    m_iOweAmount->setText(summary.iOwe.toString());
}