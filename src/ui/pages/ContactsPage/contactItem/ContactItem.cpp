#include "ContactItem.h"

#include <QContextMenuEvent>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <qapplication.h>
#include <qstyle.h>

ContactItem::ContactItem(const Contact& contact, QWidget* parent)
    : QFrame(parent)
{
    setObjectName("ContactItem");
    setMinimumHeight(56);
    setCursor(Qt::PointingHandCursor);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(16, 0, 16, 0);

    auto* nameLabel = new QLabel(contact.displayName(), this);
    nameLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    auto* arrowLabel = new QLabel(">", this);
    arrowLabel->setObjectName("ContactItemArrow");
    arrowLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    layout->addWidget(nameLabel, 1);
    layout->addWidget(arrowLabel);
}

void ContactItem::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && underMouse()) {
        emit requestedView();
    }
    
    QFrame::mouseReleaseEvent(event);
}

void ContactItem::setHovered(bool hovered) {
    if (property("hovered").toBool() == hovered) return;
    setProperty("hovered", hovered);
    style()->unpolish(this);
    style()->polish(this);
}

void ContactItem::enterEvent(QEnterEvent *event) {
    setHovered(true);
    QFrame::enterEvent(event);
}

void ContactItem::leaveEvent(QEvent *event) {
    setHovered(false);
    QFrame::leaveEvent(event);
}

void ContactItem::contextMenuEvent(QContextMenuEvent* event) {
    setHovered(false);
    emit requestedContextMenu();
}