#include "ActiveContactsWidget.h"

#include "ActiveContactItem/ActiveContactItem.h"

#include "ui/mainwindow.h"
#include "services/BalanceService.h"
#include "ui/common/toasts/toast.h"

#include "domain/entities/Contact.h"
#include "domain/valueobjects/Money.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QHash>

ActiveContactsWidget::ActiveContactsWidget(MainWindow* mv)
    : QWidget(mv)
    , m_balanceService(mv->services().balanceService)
    , m_toast(mv->uiInstances().toast)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(10);

    QLabel* sectionTitle = new QLabel("Активні контакти", this);
    sectionTitle->setProperty("role", "section");

    mainLayout->addWidget(sectionTitle);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_itemsContainer = new QWidget(m_scrollArea);

    m_itemsLayout = new QVBoxLayout(m_itemsContainer);
    m_itemsLayout->setContentsMargins(0, 0, 0, 0);
    m_itemsLayout->setSpacing(8);

    m_scrollArea->setWidget(m_itemsContainer);

    mainLayout->addWidget(m_scrollArea);

    reload();
}

void ActiveContactsWidget::reload() {
    clearItems();

    QString error;
    const QHash<Contact, Money> balances =
        m_balanceService->balancesByContact(&error);

    if (!error.isEmpty()) {
        m_toast->makeToast(error);
        return;
    }

    if (balances.isEmpty()) {
        QLabel* emptyLabel = new QLabel(
            "Поки що немає активних контактів",
            m_itemsContainer
        );

        emptyLabel->setProperty("role", "empty");
        emptyLabel->setAlignment(Qt::AlignCenter);

        m_itemsLayout->addWidget(emptyLabel);
        return;
    }

    for (auto it = balances.constBegin(); it != balances.constEnd(); ++it) {
        const Contact& contact = it.key();
        const Money& balance = it.value();

        ActiveContactItem* item = new ActiveContactItem(
            contact,
            balance,
            m_itemsContainer
        );

        connect(
            item,
            &ActiveContactItem::showContactRequested,
            this,
            [this, id = contact.id()]() {
                emit showContactRequested(id);
            }
        );

        m_itemsLayout->addWidget(item);
    }

    m_itemsLayout->addStretch();
}

void ActiveContactsWidget::clearItems() {
    while (m_itemsLayout->count() > 0) {
        QLayoutItem* item = m_itemsLayout->takeAt(0);

        if (item->widget()) {
            delete item->widget();
        }

        delete item;
    }
}