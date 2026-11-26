#pragma once

#include "domain/entities/Contact.h"
#include <QWidget>

class MainWindow;
class ContactService;
class Toast;
class QLabel;
class QVBoxLayout;
class QFrame;

class Payment;
class PaymentService;
class BalanceService;
class PaymentFormModal;
class ConfirmDialog;

class ContactDetailsPage : public QWidget {
    Q_OBJECT

public:
    explicit ContactDetailsPage(MainWindow* mv);

    void showContact(qint64 id);

signals:
    void requestedContactCtxMenu(const Contact& contact, QWidget* parent);

private:
    void reload();
    void clearView();

    Contact m_contact;

    ContactService* m_contactsService;
    Toast* m_toast;
    ConfirmDialog* m_dialog;
    qint64 m_contactId = 0;

    QFrame* m_noteContainer;
    QLabel* m_titleLabel;
    QLabel* m_noteLabel;
    QVBoxLayout* m_paymentsLayout;

    BalanceService* m_balanceService;

    QFrame* m_balanceContainer;
    QLabel* m_balanceAmount;
    QLabel* m_balanceDescription;

    void reloadBalance();
    void resetBalance();

    void reloadPayments();
    void clearPayments();
    void onAddPaymentRequested();
    void onPaymentEditRequested(const Payment& payment);
    void onPaymentDeleteRequested(const Payment& payment);

    PaymentService* m_paymentsService;
    PaymentFormModal* m_paymentForm = nullptr;
};