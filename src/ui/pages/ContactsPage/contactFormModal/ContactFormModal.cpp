#include "ContactFormModal.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QShortcut>
#include <QVBoxLayout>
#include <QTextEdit>

#include "services/ContactService.h"
#include "ui/common/toasts/toast.h"
#include "ui/mainwindow.h"

ContactFormModal::ContactFormModal(MainWindow* mv)
    : DialogBase(mv)
    , m_contactsService(mv->services().contactService)
    , m_toast(mv->uiInstances().toast)
{
    setProperty("modal", "contactForm");
    innerFrame->setFixedWidth(420);

    auto* layout = new QVBoxLayout(innerFrame);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(6);

    m_titleLabel = new QLabel(innerFrame);
    m_titleLabel->setObjectName("ModalTitle");
    layout->addWidget(m_titleLabel);
    layout->addSpacing(10);

    m_firstNameEdit = new QLineEdit(innerFrame);
    m_lastNameEdit = new QLineEdit(innerFrame);
    m_noteEdit = new QTextEdit(innerFrame);
    m_noteEdit->setMaximumHeight(85);

    auto addField = [&](const QString& caption, QWidget* edit) {
        auto* label = new QLabel(caption, innerFrame);
        label->setProperty("role", "field");
        layout->addWidget(label);
        layout->addWidget(edit);
        layout->addSpacing(8);
    };
    addField("Ім'я", m_firstNameEdit);
    addField("Прізвище", m_lastNameEdit);
    addField("Нотатка:", m_noteEdit);

    auto* buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(8);

    m_cancelButton = new QPushButton("Скасувати", innerFrame);
    m_cancelButton->setProperty("variant", "secondary");
    m_cancelButton->setCursor(Qt::PointingHandCursor);

    m_submitButton = new QPushButton(innerFrame);
    m_submitButton->setCursor(Qt::PointingHandCursor);
    m_submitButton->setDefault(true);

    buttonsLayout->addStretch();
    buttonsLayout->addWidget(m_cancelButton);
    buttonsLayout->addWidget(m_submitButton);
    layout->addSpacing(8);
    layout->addLayout(buttonsLayout);

    connect(m_cancelButton, &QPushButton::clicked, this, [this]() { hide(); });
    connect(m_submitButton, &QPushButton::clicked, this, &ContactFormModal::submit);
}

void ContactFormModal::openForCreate() {
    m_editing = false;
    m_contact = Contact();

    m_titleLabel->setText("Новий контакт");
    m_submitButton->setText("Створити");
    fillFields(m_contact);

    show();
    m_firstNameEdit->setFocus();
}

void ContactFormModal::openForEdit(const Contact& contact) {
    m_editing = true;
    m_contact = contact;

    m_titleLabel->setText("Редагування контакту");
    m_submitButton->setText("Зберегти");
    fillFields(m_contact);

    show();
    m_firstNameEdit->setFocus();
}

void ContactFormModal::fillFields(const Contact& contact) {
    m_firstNameEdit->setText(contact.firstName());
    m_lastNameEdit->setText(contact.lastName());
    m_noteEdit->setText(contact.note());
}

void ContactFormModal::submit() {
    Contact contact = m_contact;
    contact.setFirstName(m_firstNameEdit->text().trimmed());
    contact.setLastName(m_lastNameEdit->text().trimmed());
    contact.setNote(m_noteEdit->toPlainText().trimmed());

    QString error;
    const bool ok = m_editing
        ? m_contactsService->update(contact, &error)
        : m_contactsService->add(contact, &error);

    if (!ok) {
        m_toast->makeToast(error);
        return;
    }

    m_toast->makeToast(m_editing ? "Контакт оновлено" : "Контакт створено");
    hide();
}