#pragma once

#include "domain/entities/Contact.h"
#include <QWidget>

class MainWindow;
class ContactService;
class Toast;
class QLabel;
class QVBoxLayout;
class QFrame;


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

    ContactService* m_contactsService;
    Toast* m_toast;
    qint64 m_contactId = 0;

    QFrame* m_noteContainer;
    QLabel* m_titleLabel;
    QLabel* m_noteLabel;
    QVBoxLayout* m_paymentsLayout;
};