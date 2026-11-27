#include "ContactPaymentsWidget.h"
#include "services/AttachmentService.h"
#include "services/PaymentService.h"
#include "ui/common/confirmDialog/ConfirmDialog.h"
#include "ui/common/photoOverlay/PhotoOverlay.h"
#include "ui/common/toasts/toast.h"
#include "ui/mainwindow.h"
#include "ui/pages/ContactDetailsPage/PaymentFormModal/PaymentFormModal.h"
#include "ui/pages/ContactDetailsPage/PaymentItem/PaymentItem.h"
#include <qboxlayout.h>
#include <qlabel.h>
#include <qlogging.h>
#include <qpushbutton.h>
#include <qscrollarea.h>

ContactPaymentsWidget::ContactPaymentsWidget(MainWindow* mv)
    : QWidget(mv)
    , m_dialog(mv->uiInstances().confirmDialog)
    , m_paymentForm(new PaymentFormModal(mv))
    , m_toast(mv->uiInstances().toast)
    , m_paymentsService(mv->services().paymentService)
    , m_attachmentsService(mv->services().attachmentService)
    , m_photoOverlay(mv->uiInstances().photoOverlay)
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(12);

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

    connect(addPaymentButton, &QPushButton::clicked, this, &ContactPaymentsWidget::onAddRequested);
}

void ContactPaymentsWidget::clear() {
    while (QLayoutItem* item = m_paymentsLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->hide();
            widget->deleteLater();
        }
        delete item;
    }
}

void ContactPaymentsWidget::reload() {
    clear();

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
            QString attachmentsError;
            const QList<Attachment> attachments = m_attachmentsService->getByPayment(payment.id(), &attachmentsError);
            if (!attachmentsError.isEmpty()) {
                m_toast->makeToast(attachmentsError);
            }

            auto* item = new PaymentItem(payment, attachments, parent);
            m_paymentsLayout->addWidget(item);

            connect(item, &PaymentItem::requestedEdit, this, [this, payment]() {
                onEditRequested(payment);
            });
            connect(item, &PaymentItem::requestedDelete, this, [this, payment]() {
                onDeleteRequested(payment);
            });
            connect(item, &PaymentItem::requestedViewAttachment, this, [this](QString path) {
                m_photoOverlay->setImage(path);
            });
        }
    }

    m_paymentsLayout->addStretch();
}

void ContactPaymentsWidget::onAddRequested() {
    if (m_contactId <= 0) {
        return;
    }
    m_paymentForm->openForCreate(m_contactId);
}

void ContactPaymentsWidget::onEditRequested(const Payment& payment) {
    m_paymentForm->openForEdit(payment);
}

void ContactPaymentsWidget::onDeleteRequested(const Payment& payment) {
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