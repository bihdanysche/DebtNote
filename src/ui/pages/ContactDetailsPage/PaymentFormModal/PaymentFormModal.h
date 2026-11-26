#pragma once

#include "domain/entities/Payment.h"
#include "ui/common/dialogBase/DialogBase.h"

class MainWindow;
class PaymentService;
class Toast;
class QComboBox;
class QDateTimeEdit;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QPushButton;

class PaymentFormModal : public DialogBase {
    Q_OBJECT

public:
    explicit PaymentFormModal(MainWindow* mv);

    void openForCreate(qint64 contactId);
    void openForEdit(const Payment& payment);

private:
    void fillFields(const Payment& payment);
    void updateHint();
    void submit();

    PaymentService* m_paymentsService;
    Toast* m_toast;
    Payment m_payment;
    bool m_editing = false;

    QLabel* m_titleLabel;
    QLabel* m_hintLabel;
    QComboBox* m_typeCombo;
    QDoubleSpinBox* m_amountEdit;
    QDateTimeEdit* m_dateTimeEdit;
    QLineEdit* m_commentEdit;
    QPushButton* m_submitButton;
    QPushButton* m_cancelButton;
};