
#include "ContactDetailsPage.h"

#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QIcon>
#include <QSize>
#include <optional>
#include <qcursor.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qpoint.h>
#include <qpushbutton.h>

#include "domain/entities/Contact.h"
#include "services/ContactService.h"
#include "ui/common/toasts/toast.h"
#include "ui/mainwindow.h"

ContactDetailsPage::ContactDetailsPage(MainWindow* mv)
    : QWidget(mv)
    , m_contactsService(mv->services().contactService)
    , m_toast(mv->uiInstances().toast)
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

    // Note
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

    auto* paymentsTitle = new QLabel("Платежі", this);
    paymentsTitle->setProperty("role", "section");
    mainLayout->addWidget(paymentsTitle);

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

    auto* emptyLabel = new QLabel("Платежів поки немає", container);
    emptyLabel->setProperty("role", "empty");

    m_paymentsLayout->addWidget(emptyLabel);
    m_paymentsLayout->addStretch();

    mainLayout->addStretch();

    connect(m_contactsService, &ContactService::contactsChanged, this, [this]() {
        if (isVisible()) {
            reload();
        }
    });
    connect(moreButton, &QPushButton::pressed, this, [this, moreButton](){
        emit requestedContactCtxMenu(m_contact, moreButton);
    });
}

void ContactDetailsPage::showContact(qint64 id) {
    m_contactId = id;
    m_contact = Contact();
    reload();
}

void ContactDetailsPage::clearView() {
    m_titleLabel->setText("-");
    m_noteLabel->setText("");
    m_noteContainer->hide();
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
}
