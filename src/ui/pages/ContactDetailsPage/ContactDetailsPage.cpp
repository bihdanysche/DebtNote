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

#include "PaymentFormModal/PaymentFormModal.h"
#include "PaymentItem/PaymentItem.h"
#include "domain/entities/Contact.h"
#include "domain/entities/Payment.h"
#include "services/ContactService.h"
#include "services/PaymentService.h"
#include "ui/common/toasts/toast.h"
#include "ui/mainwindow.h"

ContactDetailsPage::ContactDetailsPage(MainWindow* mv)
    : QWidget(mv)
    , m_contactsService(mv->services().contactService)
    , m_paymentsService(mv->services().paymentService)
    , m_toast(mv->uiInstances().toast)
    , m_paymentForm(new PaymentFormModal(mv))
{
    setProperty("page", "contactDetails");

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(12);

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

    mainLayout->addSpacing(12);

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

void ContactDetailsPage::onPaymentEditRequested(const Payment& payment) {
    m_paymentForm->openForEdit(payment);
}

void ContactDetailsPage::onPaymentDeleteRequested(const Payment& payment) {
    const auto answer = QMessageBox::question(
        this, "Видалення платежу",
        "Видалити цей платіж?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (answer != QMessageBox::Yes) {
        return;
    }

    QString error;
    if (!m_paymentsService->remove(payment.id(), &error)) {
        m_toast->makeToast(error);
        return;
    }

    m_toast->makeToast("Платіж видалено");
}