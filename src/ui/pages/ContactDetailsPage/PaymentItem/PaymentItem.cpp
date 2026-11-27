#include "PaymentItem.h"
#include "ui/pages/ContactDetailsPage/PaymentFormModal/AttachmentPicker/AttachmentTile/AttachmentTile.h"

#include <QContextMenuEvent>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QVBoxLayout>

PaymentItem::PaymentItem(const Payment& payment, const QList<Attachment>& attachments, QWidget* parent)
    : QFrame(parent)
{
    setProperty("item", "payment");
    setMinimumHeight(64);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(16, 10, 16, 10);
    layout->setSpacing(12);

    auto* textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);

    auto* typeLabel = new QLabel(paymentTypeTitle(payment.type()), this);
    typeLabel->setProperty("role", "type");

    auto* dateLabel = new QLabel(payment.dateTime().toString("dd.MM.yyyy HH:mm"), this);
    dateLabel->setProperty("role", "date");

    if (!attachments.isEmpty()) {
        auto* tilesLayout = new QHBoxLayout();
        tilesLayout->setContentsMargins(0, 6, 0, 0);
        tilesLayout->setSpacing(8);

        for (const Attachment& attachment : attachments) {
            auto *tile = new AttachmentTile(attachment.storedPath(), attachment.fileName(), this, false);
            tilesLayout->addWidget(tile);
            connect(tile, &AttachmentTile::viewRequested, this, [this, attachment]{
                emit requestedViewAttachment(attachment.storedPath());
            });
        }
        
        tilesLayout->addStretch();

        textLayout->addLayout(tilesLayout);
    }

    textLayout->addWidget(typeLabel);

    if (!payment.comment().isEmpty()) {
        auto* commentLabel = new QLabel(payment.comment(), this);
        commentLabel->setProperty("role", "comment");
        commentLabel->setWordWrap(true);
        textLayout->addWidget(commentLabel);
    }

    textLayout->addWidget(dateLabel);

    const Money signedAmount = payment.signedAmount();
    
    auto* amountLabel = new QLabel(payment.amount().toString(), this);
    amountLabel->setProperty("role", "amount");
    amountLabel->setProperty("balance", signedAmount.isPositive() ? "positive" : "negative");

    layout->addLayout(textLayout, 1);
    layout->addWidget(amountLabel, 0, Qt::AlignVCenter);
}

void PaymentItem::contextMenuEvent(QContextMenuEvent* event) {
    QMenu menu(this);
    QAction* editAction = menu.addAction(QIcon(":/icons/edit.png"), "Змінити");
    menu.addSeparator();
    QAction* deleteAction = menu.addAction(QIcon(":/icons/delete.png"), "Видалити");

    const QAction* chosen = menu.exec(event->globalPos());

    if (chosen == editAction) {
        emit requestedEdit();
    } else if (chosen == deleteAction) {
        emit requestedDelete();
    }
}