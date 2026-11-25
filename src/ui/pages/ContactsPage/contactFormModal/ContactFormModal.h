#pragma once

#include "domain/entities/Contact.h"
#include "ui/common/dialogBase/DialogBase.h"
#include <qpoint.h>

class ContactService;
class Toast;
class QLineEdit;
class QLabel;
class QPushButton;
class QTextEdit;

class MainWindow;

class ContactFormModal : public DialogBase {
    Q_OBJECT

public:
    ContactFormModal(MainWindow* mv);

    void openForCreate();
    void openForEdit(const Contact& contact);
private:
    void fillFields(const Contact& contact);
    void submit();

    ContactService* m_contactsService;
    Toast* m_toast;
    Contact m_contact;
    bool m_editing = false;

    QLabel* m_titleLabel;
    QLineEdit* m_firstNameEdit;
    QLineEdit* m_lastNameEdit;
    QTextEdit* m_noteEdit;
    QPushButton* m_submitButton;
    QPushButton* m_cancelButton;
};