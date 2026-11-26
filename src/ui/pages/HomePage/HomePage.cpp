#include "HomePage.h"

#include <QVBoxLayout>
#include <QLabel>

#include "services/PaymentService.h"
#include "ui/mainwindow.h"
#include "BalanceWidget/BalanceWidget.h"

HomePage::HomePage(MainWindow* mv)
    : QWidget(mv)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel* title = new QLabel("Головна", this);
    title->setObjectName("PageTitle");

    m_balanceWidget = new BalanceWidget(mv);

    mainLayout->addWidget(title);
    mainLayout->addWidget(m_balanceWidget);
    mainLayout->addStretch();

    // Connect to updates
    reload();
    auto *paymentService = mv->services().paymentService;
    connect(paymentService, &PaymentService::paymentsChanged, this, [this](){
        reload();
    });
}

void HomePage::reload() {
    m_balanceWidget->reload();
}