#include "PaymentItem.h"

#include <QContextMenuEvent>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QVBoxLayout>

PaymentItem::PaymentItem(const Payment& payment, QWidget* parent)
    : QFrame(parent)
{
    setProperty("item", "payment");
    setMinimumHeight(64);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(16, 10, 16, 10);
    layout->setSpacing(12);

    auto* textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);

    const bool hasComment = !payment.comment().isEmpty();
    auto* commentLabel = new QLabel(hasComment ? payment.comment() : "Без коментаря", this);
    commentLabel->setProperty("role", "comment");
    commentLabel->setProperty("muted", !hasComment);
    commentLabel->setWordWrap(true);

    auto* dateLabel = new QLabel(payment.dateTime().toString("dd.MM.yyyy HH:mm"), this);
    dateLabel->setProperty("role", "date");

    textLayout->addWidget(commentLabel);
    textLayout->addWidget(dateLabel);

    const Money signedAmount = payment.signedAmount();
    const QString amountText = signedAmount.isPositive()
        ? "+" + signedAmount.toString()
        : signedAmount.toString();

    auto* amountLabel = new QLabel(amountText, this);
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