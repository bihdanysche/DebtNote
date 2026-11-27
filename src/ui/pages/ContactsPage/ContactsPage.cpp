#include "ContactsPage.h"

#include <QLabel>
#include <QLayoutItem>
#include <QMessageBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <qcoreevent.h>
#include <qlogging.h>
#include <qmenu.h>
#include <qobject.h>
#include <qpoint.h>
#include <qwidget.h>

#include "contactItem/ContactItem.h"
#include "domain/entities/Contact.h"
#include "services/ContactService.h"
#include "ui/common/confirmDialog/ConfirmDialog.h"
#include "ui/common/toasts/toast.h"
#include "ui/mainwindow.h"
#include "ui/pages/ContactsPage/contactFormModal/ContactFormModal.h"
#include "ui/utils/utils.h"

ContactsPage::ContactsPage(MainWindow* mv)
    : QWidget(mv), m_contactsService(mv->services().contactService),
    m_dialog(mv->uiInstances().confirmDialog), m_toast(mv->uiInstances().toast),
    m_contactModal(new ContactFormModal(mv))
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    auto* headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(0, 0, 0, 0);

    auto* title = new QLabel("Контакти", this);
    title->setObjectName("PageTitle");

    auto* addButton = new QPushButton(Utils::recolorIcon(QIcon(":/icons/plus.svg"), QColor::fromRgb(255, 255, 255)), "Новий контакт", this);
    addButton->setIconSize(QSize(16, 16));
    addButton->setCursor(Qt::PointingHandCursor);

    headerLayout->addWidget(title);
    headerLayout->addStretch();
    headerLayout->addWidget(addButton);
    mainLayout->addLayout(headerLayout);

    connect(addButton, &QPushButton::clicked, this, &ContactsPage::onAddRequested);

    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout->addWidget(scrollArea, 1);

    auto* container = new QWidget();
    m_listLayout = new QVBoxLayout(container);
    m_listLayout->setContentsMargins(0, 0, 8, 0);
    m_listLayout->setSpacing(8);
    scrollArea->setWidget(container);

    connect(m_contactsService, &ContactService::contactsChanged, this, &ContactsPage::reload);
    reload();
}

void ContactsPage::reload() {
    clearList();

    QString error;
    const QList<Contact> contacts = m_contactsService->getAll(&error);

    if (!error.isEmpty()) {
        m_toast->makeToast(error);
        makeHint("Не вдалося завантажити контакти.");
    } else if (contacts.isEmpty()) {
        makeHint("Контактів поки немає.");
    } else {
        for (const Contact& contact : contacts) {
            addContactItem(contact);
        }
    }

    m_listLayout->addStretch();
}

void ContactsPage::makeHint(const QString& hint) {
    auto* lbl = new QLabel(hint, this);
    lbl->setObjectName("Hint");
    m_listLayout->addWidget(lbl);
}

void ContactsPage::clearList() {
    while (QLayoutItem* item = m_listLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->hide();
            widget->deleteLater();
        }
        delete item;
    }
}

void ContactsPage::addContactItem(const Contact& contact) {
    auto* item = new ContactItem(contact, this);
    m_listLayout->addWidget(item);

    const qint64 id = contact.id();
    connect(item, &ContactItem::requestedContextMenu, this, [this, contact, item]() { 
        showContactItemMenu(contact, item);
    });
    connect(item, &ContactItem::requestedView, this, [this, id]() { emit contactSelected(id); });
}

void ContactsPage::onEditRequested(const Contact& contact) {
    m_contactModal->openForEdit(contact);
}

void ContactsPage::onAddRequested() {
    m_contactModal->openForCreate();
}

void ContactsPage::onDeleteRequested(const Contact& contact) {
    DialogParams params = {};
    params.title = "Видалення контакту";
    params.desc = QString("Ви впевнені, що хочете видалити контакт \"%1\" разом з усіма його платежами?")
                        .arg(contact.displayName());
   
    m_dialog->showDialog(params, [this, &contact](int answer) {
        if (answer != 1) return;

        QString error;
        if (!m_contactsService->remove(contact.id(), &error)) {
            m_toast->makeToast("Не вдалося видалити. " + error);
            return;
        }

        m_toast->makeToast("Контакт успішно видалено!");
    });
}

void ContactsPage::showContactItemMenu(const Contact& contact, QWidget* parent, bool fromDetailsPage) {
    QMenu menu(parent);

    menu.setAttribute(Qt::WA_TranslucentBackground);
    menu.setWindowFlags(menu.windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);

    QAction* viewAction = nullptr;
    if (!fromDetailsPage) viewAction = menu.addAction(QIcon(":/icons/view.svg"), "Переглянути");
    QAction* editAction = menu.addAction(QIcon(":/icons/edit.svg"), "Змінити");
    menu.addSeparator();
    QAction* deleteAction = menu.addAction(QIcon(":/icons/delete.svg"), "Видалити");
    
    const QAction* chosen = menu.exec(QCursor::pos() + QPoint(12, 20));

    if (chosen == viewAction && viewAction != nullptr) {
        emit contactSelected(contact.id());
    } else if (chosen == editAction) {
        onEditRequested(contact);
    } else if (chosen == deleteAction) {
        onDeleteRequested(contact);
    }
}