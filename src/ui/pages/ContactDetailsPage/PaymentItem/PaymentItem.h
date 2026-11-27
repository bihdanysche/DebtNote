#pragma once

#include <QFrame>
#include <qobject.h>

#include "domain/entities/Attachment.h"
#include "domain/entities/Payment.h"

class PaymentItem : public QFrame {
    Q_OBJECT

public:
    PaymentItem(const Payment& payment, const QList<Attachment>& attachments, QWidget* parent = nullptr);

signals:
    void requestedViewAttachment(QString path);
    void requestedEdit();
    void requestedDelete();

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
};