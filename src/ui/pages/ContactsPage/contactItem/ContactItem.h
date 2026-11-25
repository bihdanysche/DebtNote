#pragma once

#include <QFrame>
#include "domain/entities/Contact.h"

class ContactItem : public QFrame {
    Q_OBJECT

public:
    explicit ContactItem(const Contact& contact, QWidget* parent = nullptr);
signals:
    void requestedView();
    void requestedContextMenu();
private:
    void setHovered(bool hovered);
protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
};