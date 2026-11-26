#pragma once

#include <QFrame>

#include "domain/entities/Payment.h"

class PaymentItem : public QFrame {
    Q_OBJECT

public:
    explicit PaymentItem(const Payment& payment, QWidget* parent = nullptr);

signals:
    void requestedEdit();
    void requestedDelete();

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
};