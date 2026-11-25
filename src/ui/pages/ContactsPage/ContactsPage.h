#pragma once

#include <QWidget>
#include <qobject.h>
#include <qpoint.h>
#include "domain/entities/Contact.h"

class ContactService;
class QVBoxLayout;

class MainWindow;
class Toast;
class ConfirmDialog;
class ContactFormModal;

class ContactsPage : public QWidget {
    Q_OBJECT

public:
    explicit ContactsPage(MainWindow* mainWindow);
    void showContactItemMenu(const Contact& contact, QWidget* parent, bool fromDetailsPage = false);

signals:
    void contactSelected(qint64 id);

private:
    void reload();
    void clearList();
    void addContactItem(const Contact& contact);
    void makeHint(const QString& hint);

    void onEditRequested(const Contact& contact);
    void onDeleteRequested(const Contact& contact);
    void onAddRequested();

    ContactService *m_contactsService;
    Toast *m_toast;
    ConfirmDialog *m_dialog;
    QVBoxLayout *m_listLayout = nullptr;

    ContactFormModal *m_contactModal;
};