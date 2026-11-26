#include "ActiveContactItem.h"

#include "domain/entities/Contact.h"
#include "domain/valueobjects/Money.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QEnterEvent>
#include <qstyle.h>

ActiveContactItem::ActiveContactItem(
    const Contact& contact,
    const Money& balance,
    QWidget* parent
)
    : QFrame(parent)
{
    setObjectName("ContactItem");
    setCursor(Qt::PointingHandCursor);
    setMouseTracking(true);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(14, 12, 14, 12);
    mainLayout->setSpacing(10);

    QLabel* nameLabel = new QLabel(contact.displayName(), this);
    nameLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    QLabel* balanceLabel = new QLabel(this);
    balanceLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    if (balance.isPositive()) {
        balanceLabel->setText("Вам винні " + balance.toString(true));
        balanceLabel->setStyleSheet("color: #27834b;");
    }
    else if (balance.isNegative()) {
        balanceLabel->setText("Ви винні " + balance.toString(true));
        balanceLabel->setStyleSheet("color: #c44747;");
    }
    else {
        balanceLabel->setText("Боргів немає");
        balanceLabel->setStyleSheet("color: #64746a;");
    }

    QLabel* arrowLabel = new QLabel("›", this);
    arrowLabel->setObjectName("ContactItemArrow");
    arrowLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    mainLayout->addWidget(nameLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(balanceLabel);
    mainLayout->addWidget(arrowLabel);
}

void ActiveContactItem::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit showContactRequested();
    }

    QFrame::mousePressEvent(event);
}

void ActiveContactItem::enterEvent(QEnterEvent* event) {
    setProperty("hovered", true);
    style()->unpolish(this);
    style()->polish(this);

    QFrame::enterEvent(event);
}

void ActiveContactItem::leaveEvent(QEvent* event) {
    setProperty("hovered", false);
    style()->unpolish(this);
    style()->polish(this);

    QFrame::leaveEvent(event);
}