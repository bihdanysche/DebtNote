#include "PaymentFormModal.h"

#include <QComboBox>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QShortcut>
#include <QVBoxLayout>

#include "services/PaymentService.h"
#include "ui/common/dialogBase/DialogBase.h"
#include "ui/common/toasts/toast.h"
#include "ui/mainwindow.h"

PaymentFormModal::PaymentFormModal(MainWindow* mv)
    : DialogBase(mv)
    , m_paymentsService(mv->services().paymentService)
    , m_toast(mv->uiInstances().toast)
{
    setProperty("modal", "paymentForm");
    innerFrame->setFixedWidth(440);

    auto* layout = new QVBoxLayout(innerFrame);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(6);

    m_titleLabel = new QLabel(innerFrame);
    m_titleLabel->setObjectName("ModalTitle");
    layout->addWidget(m_titleLabel);
    layout->addSpacing(10);

    auto addField = [&](const QString& caption, QWidget* widget) {
        auto* label = new QLabel(caption, innerFrame);
        label->setProperty("role", "field");
        layout->addWidget(label);
        layout->addWidget(widget);
    };

    m_typeCombo = new QComboBox(innerFrame);
    m_typeCombo->addItem("Я дав у борг", static_cast<int>(PaymentDirection::Lent));
    m_typeCombo->addItem("Я взяв у борг", static_cast<int>(PaymentDirection::Borrowed));
    m_typeCombo->addItem("Мені повернули борг", static_cast<int>(PaymentDirection::Borrowed));
    m_typeCombo->addItem("Я повернув борг", static_cast<int>(PaymentDirection::Lent));
    addField("Тип операції", m_typeCombo);

    m_hintLabel = new QLabel(innerFrame);
    m_hintLabel->setProperty("role", "hint");
    m_hintLabel->setWordWrap(true);
    layout->addWidget(m_hintLabel);
    layout->addSpacing(8);

    m_amountEdit = new QDoubleSpinBox(innerFrame);
    m_amountEdit->setDecimals(2);
    m_amountEdit->setRange(0.0, 999999999.99);
    m_amountEdit->setSuffix(" грн");
    m_amountEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    addField("Сума *", m_amountEdit);
    layout->addSpacing(8);

    m_dateTimeEdit = new QDateTimeEdit(innerFrame);
    m_dateTimeEdit->setCalendarPopup(true);
    m_dateTimeEdit->setDisplayFormat("dd.MM.yyyy HH:mm");
    addField("Дата й час", m_dateTimeEdit);
    layout->addSpacing(8);

    m_commentEdit = new QLineEdit(innerFrame);
    addField("Коментар", m_commentEdit);
    layout->addSpacing(16);

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
    layout->addLayout(buttonsLayout);

    connect(m_typeCombo, &QComboBox::currentIndexChanged, this, &PaymentFormModal::updateHint);
    connect(m_cancelButton, &QPushButton::clicked, this, [this]() { hide(); });
    connect(m_submitButton, &QPushButton::clicked, this, &PaymentFormModal::submit);
    connect(m_commentEdit, &QLineEdit::returnPressed, this, &PaymentFormModal::submit);

    auto* escape = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(escape, &QShortcut::activated, this, [this]() { hide(); });
}

void PaymentFormModal::openForCreate(qint64 contactId) {
    m_editing = false;
    m_payment = Payment();
    m_payment.setContactId(contactId);

    m_titleLabel->setText("Новий платіж");
    m_submitButton->setText("Створити");
    fillFields(m_payment);

    show();
    m_amountEdit->setFocus();
}

void PaymentFormModal::openForEdit(const Payment& payment) {
    m_editing = true;
    m_payment = payment;

    m_titleLabel->setText("Редагування платежу");
    m_submitButton->setText("Зберегти");
    fillFields(m_payment);

    show();
    m_amountEdit->setFocus();
    m_amountEdit->selectAll();
}

void PaymentFormModal::fillFields(const Payment& payment) {
    const int index = m_typeCombo->findData(static_cast<int>(payment.direction()));
    m_typeCombo->setCurrentIndex(index < 0 ? 0 : index);
    updateHint();

    m_amountEdit->setValue(payment.amount().toHryvnias());
    m_dateTimeEdit->setDateTime(payment.dateTime());
    m_commentEdit->setText(payment.comment());
}

void PaymentFormModal::updateHint() {
    switch (m_typeCombo->currentIndex()) {
    case 0:
        m_hintLabel->setText("Контакт буде винен вам більше.");
        break;
    case 1:
        m_hintLabel->setText("Ви будете винні контакту більше.");
        break;
    case 2:
        m_hintLabel->setText("Борг контакту перед вами зменшиться.");
        break;
    case 3:
        m_hintLabel->setText("Ваш борг перед контактом зменшиться.");
        break;
    }
}

void PaymentFormModal::submit() {
    Payment payment = m_payment;
    payment.setDirection(static_cast<PaymentDirection>(m_typeCombo->currentData().toInt()));
    payment.setAmount(Money::fromHryvnias(m_amountEdit->value()));
    payment.setDateTime(m_dateTimeEdit->dateTime());
    payment.setComment(m_commentEdit->text().trimmed());

    QString error;
    const bool ok = m_editing
        ? m_paymentsService->update(payment, &error)
        : m_paymentsService->add(payment, &error);

    if (!ok) {
        m_toast->makeToast(error);
        return;
    }

    m_toast->makeToast(m_editing ? "Платіж оновлено" : "Платіж створено");
    hide();
}