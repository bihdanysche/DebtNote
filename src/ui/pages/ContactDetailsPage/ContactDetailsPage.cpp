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

#include "domain/entities/Contact.h"
#include "services/ContactService.h"
#include "services/PaymentService.h"
#include "ui/common/toasts/toast.h"
#include "ui/mainwindow.h"

ContactDetailsPage::ContactDetailsPage(MainWindow* mv)
    : QWidget(mv)
    , m_toast(mv->uiInstances().toast)
    , m_contactsService(mv->services().contactService)
    , m_paymentsService(mv->services().paymentService)
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
    m_balanceWidget = new ContactBalanceWidget(mv);
    mainLayout->addWidget(m_balanceWidget);

    mainLayout->addSpacing(12);
    
    // Payments
    m_paymentsWidget = new ContactPaymentsWidget(mv);
    mainLayout->addWidget(m_paymentsWidget, 1);

    connect(m_contactsService, &ContactService::contactsChanged, this, [this]() {
        if (isVisible()) {
            reload();
        }
    });
    connect(m_paymentsService, &PaymentService::paymentsChanged, this, [this]() {
        if (isVisible()) {
            m_paymentsWidget->setContactId(m_contactId);
            m_balanceWidget->setContactId(m_contactId);
        }
    });
    connect(moreButton, &QPushButton::pressed, this, [this, moreButton]() {
        emit requestedContactCtxMenu(m_contact, moreButton);
    });
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
    m_paymentsWidget->clear();
    m_balanceWidget->reset();
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

    m_paymentsWidget->setContactId(m_contactId);
    m_balanceWidget->setContactId(m_contactId);
}