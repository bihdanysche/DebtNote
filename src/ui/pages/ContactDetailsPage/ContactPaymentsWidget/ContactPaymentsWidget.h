#pragma once

#include <QWidget>
#include <qboxlayout.h>

class MainWindow;
class Toast;
class ConfirmDialog;
class PaymentFormModal;
class PaymentService;

class Payment;

class ContactPaymentsWidget : public QWidget {
    Q_OBJECT;
public:
    explicit ContactPaymentsWidget(MainWindow *mv);
    void reload();
    void setContactId(qint64 contactId) { m_contactId = contactId; reload(); };
    void clear();
private:
    qint64 m_contactId = -1;
    
    Toast* m_toast;
    ConfirmDialog* m_dialog;
    PaymentFormModal* m_paymentForm;

    PaymentService* m_paymentsService;

    QVBoxLayout* m_paymentsLayout;

    void onEditRequested(const Payment& payment);
    void onDeleteRequested(const Payment& payment);
    void onAddRequested();
};