#pragma once

#include <QFrame>

class Contact;
class Money;
class QLabel;
class QMouseEvent;
class QEnterEvent;

class ActiveContactItem : public QFrame {
    Q_OBJECT

public:
    explicit ActiveContactItem(
        const Contact& contact,
        const Money& balance,
        QWidget* parent = nullptr
    );

signals:
    void showContactRequested();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
};