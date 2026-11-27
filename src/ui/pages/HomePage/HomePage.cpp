#include "HomePage.h"

#include <QVBoxLayout>
#include <QLabel>

#include "services/ContactService.h"
#include "services/PaymentService.h"
#include "ui/mainwindow.h"
#include "BalanceWidget/BalanceWidget.h"
#include "ui/pages/HomePage/ActiveContactsWidget/ActiveContactsWidget.h"

HomePage::HomePage(MainWindow* mv)
    : QWidget(mv)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel* title = new QLabel("Головна", this);
    title->setObjectName("PageTitle");

    m_balanceWidget = new BalanceWidget(mv);
    m_activeContactsWidget = new ActiveContactsWidget(mv);

    mainLayout->addWidget(title);
    mainLayout->addWidget(m_balanceWidget);
    mainLayout->addWidget(m_activeContactsWidget, 1);

    // Connect to updates
    reload();
    auto *paymentService = mv->services().paymentService;
    auto *contactService = mv->services().contactService;
    connect(paymentService, &PaymentService::paymentsChanged, this, [this](){
        reload();
    });
    connect(contactService, &ContactService::contactsChanged, this, [this](){
        reload();
    });
    connect(m_activeContactsWidget, &ActiveContactsWidget::showContactRequested, this, &HomePage::viewContactRequested);
}

void HomePage::reload() {
    m_balanceWidget->reload();
    m_activeContactsWidget->reload();
}