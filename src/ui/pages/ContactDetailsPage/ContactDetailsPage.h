#pragma once

#include "domain/entities/Contact.h"
#include "ContactBalanceWidget/ContactBalanceWidget.h"
#include "ContactPaymentsWidget/ContactPaymentsWidget.h"

#include <QWidget>

class MainWindow;
class ContactService;
class Toast;
class QLabel;
class QVBoxLayout;
class QFrame;

class Payment;
class PaymentService;
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
    qint64 m_contactId = 0;

    ContactService* m_contactsService;
    PaymentService* m_paymentsService;

    Toast* m_toast;
    ConfirmDialog* m_dialog;

    QFrame* m_noteContainer;
    QLabel* m_titleLabel;
    QLabel* m_noteLabel;

    ContactBalanceWidget* m_balanceWidget;
    ContactPaymentsWidget* m_paymentsWidget;
};