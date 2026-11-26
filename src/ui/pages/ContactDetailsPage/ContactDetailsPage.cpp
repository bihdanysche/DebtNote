#include "ContactDetailsPage.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSize>
#include <QVBoxLayout>
#include <optional>
#include <qstyle.h>

#include "PaymentFormModal/PaymentFormModal.h"
#include "PaymentItem/PaymentItem.h"
#include "domain/entities/Contact.h"
#include "domain/entities/Payment.h"
#include "services/BalanceService.h"
#include "services/ContactService.h"
#include "services/PaymentService.h"
#include "ui/common/confirmDialog/ConfirmDialog.h"
#include "ui/common/toasts/toast.h"
#include "ui/mainwindow.h"

ContactDetailsPage::ContactDetailsPage(MainWindow* mv)
    : QWidget(mv)
    , m_contactsService(mv->services().contactService)
    , m_paymentsService(mv->services().paymentService)
    , m_toast(mv->uiInstances().toast)
    , m_dialog(mv->uiInstances().confirmDialog)
    , m_paymentForm(new PaymentFormModal(mv))
    , m_balanceService(mv->services().balanceService)
{
    setProperty("page", "contactDetails");

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(12);

    // Header
    auto* headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(0);

    m_titleLabel = new QLabel("", this);
    m_titleLabel->setObjectName("PageTitle");

    auto* moreButton = new QPushButton(this);
    moreButton->setObjectName("IconButton");
    moreButton->setProperty("variant", "secondary");
    moreButton->setFixedSize(40, 40);
    moreButton->setIcon(QIcon(":/icons/menu.svg"));
    moreButton->setIconSize(QSize(20, 20));
    moreButton->setCursor(Qt::PointingHandCursor);

    headerLayout->addWidget(m_titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(moreButton);
    mainLayout->addLayout(headerLayout);

    // Note section
    m_noteContainer = new QFrame(this);
    m_noteContainer->setObjectName("NoteContainer");

    auto* noteLayout = new QVBoxLayout(m_noteContainer);
    noteLayout->setContentsMargins(12, 12, 12, 12);
    noteLayout->setSpacing(6);

    auto* noteTitle = new QLabel("Нотатка", m_noteContainer);
    noteTitle->setProperty("role", "section");

    m_noteLabel = new QLabel("", m_noteContainer);
    m_noteLabel->setObjectName("NoteContent");
    m_noteLabel->setWordWrap(true);

    noteLayout->addWidget(noteTitle);
    noteLayout->addWidget(m_noteLabel);

    m_noteContainer->hide();
    mainLayout->addWidget(m_noteContainer);

    // Balance section
    m_balanceContainer = new QFrame(this);
    m_balanceContainer->setObjectName("BalanceContainer");

    auto* balanceLayout = new QVBoxLayout(m_balanceContainer);
    balanceLayout->setContentsMargins(16, 14, 16, 14);
    balanceLayout->setSpacing(6);

    auto* balanceTitle = new QLabel("Поточний баланс", m_balanceContainer);
    balanceTitle->setProperty("role", "section");

    m_balanceAmount = new QLabel("0,00 ₴", m_balanceContainer);
    m_balanceAmount->setObjectName("BalanceAmount");

    m_balanceDescription = new QLabel("", m_balanceContainer);
    m_balanceDescription->setObjectName("BalanceDescription");

    balanceLayout->addWidget(balanceTitle);
    balanceLayout->addWidget(m_balanceAmount);
    balanceLayout->addWidget(m_balanceDescription);

    mainLayout->addWidget(m_balanceContainer);

    mainLayout->addSpacing(12);
    
    // Payments
    auto* paymentsHeader = new QHBoxLayout();
    paymentsHeader->setContentsMargins(0, 0, 0, 0);

    auto* paymentsTitle = new QLabel("Платежі", this);
    paymentsTitle->setProperty("role", "section");

    auto* addPaymentButton = new QPushButton(QIcon(":/icons/plus.png"), "Новий платіж", this);
    addPaymentButton->setIconSize(QSize(16, 16));
    addPaymentButton->setCursor(Qt::PointingHandCursor);

    paymentsHeader->addWidget(paymentsTitle);
    paymentsHeader->addStretch();
    paymentsHeader->addWidget(addPaymentButton);
    mainLayout->addLayout(paymentsHeader);

    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout->addWidget(scrollArea, 1);

    auto* container = new QWidget();
    m_paymentsLayout = new QVBoxLayout(container);
    m_paymentsLayout->setContentsMargins(0, 0, 8, 0);
    m_paymentsLayout->setSpacing(8);
    scrollArea->setWidget(container);

    connect(m_contactsService, &ContactService::contactsChanged, this, [this]() {
        if (isVisible()) {
            reload();
        }
    });
    connect(m_paymentsService, &PaymentService::paymentsChanged, this, [this]() {
        if (isVisible()) {
            reloadPayments();
            reloadBalance();
        }
    });
    connect(moreButton, &QPushButton::pressed, this, [this, moreButton]() {
        emit requestedContactCtxMenu(m_contact, moreButton);
    });
    connect(addPaymentButton, &QPushButton::clicked, this, &ContactDetailsPage::onAddPaymentRequested);
}

void ContactDetailsPage::showContact(qint64 id) {
    m_contactId = id;
    m_contact = Contact();
    reload();
}

void ContactDetailsPage::clearView() {
    m_titleLabel->setText("[Не завантажено]");
    m_noteLabel->setText("");
    m_noteContainer->hide();
    clearPayments();
}

void ContactDetailsPage::reload() {
    clearView();

    if (m_contactId <= 0) {
        return;
    }

    QString error;
    const std::optional<Contact> contact = m_contactsService->getById(m_contactId, &error);

    if (!contact) {
        if (!error.isEmpty()) {
            m_toast->makeToast(error);
        } else {
            m_toast->makeToast("Контакт з таким ID не знайдено.");
        }
        return;
    }

    m_titleLabel->setText(contact->displayName());
    m_noteLabel->setText(contact->note());
    m_noteContainer->setVisible(!contact->note().isEmpty());
    m_contact = *contact;

    reloadPayments();
    reloadBalance();
}

void ContactDetailsPage::clearPayments() {
    while (QLayoutItem* item = m_paymentsLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->hide();
            widget->deleteLater();
        }
        delete item;
    }
}

void ContactDetailsPage::reloadPayments() {
    clearPayments();

    if (m_contactId <= 0) {
        return;
    }

    QString error;
    const QList<Payment> payments = m_paymentsService->getByContact(m_contactId, &error);

    if (!error.isEmpty()) {
        m_toast->makeToast(error);
        return;
    }

    QWidget* parent = m_paymentsLayout->parentWidget();

    if (payments.isEmpty()) {
        auto* emptyLabel = new QLabel("Платежів поки немає", parent);
        emptyLabel->setProperty("role", "empty");
        m_paymentsLayout->addWidget(emptyLabel);
    } else {
        for (const Payment& payment : payments) {
            auto* item = new PaymentItem(payment, parent);
            m_paymentsLayout->addWidget(item);

            connect(item, &PaymentItem::requestedEdit, this, [this, payment]() {
                onPaymentEditRequested(payment);
            });
            connect(item, &PaymentItem::requestedDelete, this, [this, payment]() {
                onPaymentDeleteRequested(payment);
            });
        }
    }

    m_paymentsLayout->addStretch();
}

void ContactDetailsPage::onAddPaymentRequested() {
    if (m_contactId <= 0) {
        return;
    }
    m_paymentForm->openForCreate(m_contactId);
}

void ContactDetailsPage::resetBalance() {
    m_balanceAmount->setText("??? грн");
    m_balanceContainer->setProperty("balance", "neutral");
    m_balanceAmount->setProperty("balance", "neutral");
    m_balanceDescription->setText("Взаєморозрахунки закриті");

    m_balanceContainer->style()->unpolish(m_balanceContainer);
    m_balanceContainer->style()->polish(m_balanceContainer);
    m_balanceAmount->style()->unpolish(m_balanceAmount);
    m_balanceAmount->style()->polish(m_balanceAmount);
}

void ContactDetailsPage::reloadBalance() {
    if (m_contactId <= 0) {
        resetBalance();
        return;
    }

    QString error;
    Money balance = m_balanceService->balanceForContact(m_contactId, &error);

    if (!error.isEmpty()) {
        m_toast->makeToast(error);
        resetBalance();
        return;
    }

    double amount = balance.toHryvnias();
    double absoluteAmount = std::abs(amount);

    m_balanceAmount->setText(
        balance.toString()
    );

    if (amount > 0) {
        m_balanceContainer->setProperty("balance", "positive");
        m_balanceAmount->setProperty("balance", "positive");
        m_balanceDescription->setText("Ця людина винна вам");
    }
    else if (amount < 0) {
        m_balanceContainer->setProperty("balance", "negative");
        m_balanceAmount->setProperty("balance", "negative");
        m_balanceDescription->setText("Ви винні цій людині");
    }
    else {
        m_balanceContainer->setProperty("balance", "neutral");
        m_balanceAmount->setProperty("balance", "neutral");
        m_balanceDescription->setText("Взаєморозрахунки закриті");
    }

    m_balanceContainer->style()->unpolish(m_balanceContainer);
    m_balanceContainer->style()->polish(m_balanceContainer);
    m_balanceAmount->style()->unpolish(m_balanceAmount);
    m_balanceAmount->style()->polish(m_balanceAmount);
}

void ContactDetailsPage::onPaymentEditRequested(const Payment& payment) {
    m_paymentForm->openForEdit(payment);
}

void ContactDetailsPage::onPaymentDeleteRequested(const Payment& payment) {
    DialogParams params;
    params.title = "Видалення платежу";
    params.desc = "Видалити цей платіж?";
    
    m_dialog->showDialog(params, [this, payment](int answ) {
        if (answ != 1) return;

        QString error;
        if (!m_paymentsService->remove(payment.id(), &error)) {
            m_toast->makeToast(error);
            return;
        }

        m_toast->makeToast("Платіж видалено");
    });
}