#include "mainwindow.h"
#include "domain/entities/Contact.h"
#include "services/AppServices.h"
#include "ui/common/confirmDialog/ConfirmDialog.h"
#include "ui/common/photoOverlay/PhotoOverlay.h"
#include "ui/common/toasts/toast.h"
#include "ui/pages/ContactsPage/ContactsPage.h"
#include "ui/pages/ContactDetailsPage/ContactDetailsPage.h"
#include "ui/pages/HomePage/HomePage.h"
#include "ui/sideMenu/SideMenu.h"
#include <QWidget>
#include <QHBoxLayout>
#include <qboxlayout.h>
#include <QStackedWidget>
#include <qlogging.h>
#include <qstackedwidget.h>

MainWindow::MainWindow(AppServices& services, QWidget *parent)
    : m_services(services), QMainWindow(parent)
{
    setWindowTitle("DebtNote");
    resize(900, 650);

    QWidget* central = new QWidget(this);
    QHBoxLayout* lay = new QHBoxLayout(central);

    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    initUIInstances();
    initSideMenu();
    initStackedWidget();
    
    lay->addWidget(m_sideMenu);
    lay->addWidget(m_stacked, 1);

    setCentralWidget(central);
}

void MainWindow::initSideMenu() {
    m_sideMenu = new SideMenu({
        {
            "Головна", ":/icons/house.svg", static_cast<int>(Screen::Home)
        },
        {
            "Контакти", ":/icons/users-round.svg", static_cast<int>(Screen::Contacts)
        }
    }, this);

    connect(m_sideMenu, &SideMenu::switchToScreen, this, [this](int id) {
        m_stacked->setCurrentIndex(id);
    });
}

void MainWindow::initStackedWidget() {
    m_stacked = new QStackedWidget(this);

    m_homePage = new HomePage(this);
    m_contactsPage = new ContactsPage(this);
    m_contactDetailsPage = new ContactDetailsPage(this);

    m_stacked->addWidget(m_homePage);
    m_stacked->addWidget(m_contactsPage);
    m_stacked->addWidget(m_contactDetailsPage);

    // ContactsPage signals
    connect(m_contactsPage, &ContactsPage::contactSelected, this, [this](qint64 id) {
        m_contactDetailsPage->showContact(id);
        switchToScreen(Screen::ContactDetails);
    });

    // ContactDetailsPage signals
    connect(m_contactDetailsPage, &ContactDetailsPage::requestedContactCtxMenu, this, [this](const Contact& contact, QWidget* parent) {
        m_contactsPage->showContactItemMenu(contact, parent, true);
    });

    // HomePage signals
    connect(m_homePage, &HomePage::viewContactRequested, this, [this](qint64 id) {
        m_contactDetailsPage->showContact(id);
        switchToScreen(Screen::ContactDetails);
    });
}

void MainWindow::initUIInstances() {
    m_uiInstances.confirmDialog = new ConfirmDialog(this);
    m_uiInstances.toast = new Toast(this);
    m_uiInstances.photoOverlay = new PhotoOverlay(this);
}

void MainWindow::switchToScreen(Screen screen) {
    int ind = static_cast<int>(screen);

    m_stacked->setCurrentIndex(ind);
    m_sideMenu->setSelected(ind);
}