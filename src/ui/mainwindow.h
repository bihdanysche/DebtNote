#pragma once

#include "services/AppServices.h"
#include <QMainWindow>

class SideMenu;
class QStackedWidget;

class HomePage;
class ContactsPage;
class ContactDetailsPage;

enum class Screen { Home, Contacts, ContactDetails };

class ConfirmDialog;
class Toast;
class PhotoOverlay;

struct UIInstances {
    ConfirmDialog *confirmDialog;
    Toast *toast;
    PhotoOverlay *photoOverlay;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(AppServices& services, QWidget *parent = nullptr);
    UIInstances uiInstances() { return m_uiInstances; };
    AppServices services() { return m_services; };
private:
    SideMenu *m_sideMenu;
    QStackedWidget *m_stacked;
    AppServices &m_services;
    UIInstances m_uiInstances;

    HomePage *m_homePage;
    ContactsPage *m_contactsPage;
    ContactDetailsPage *m_contactDetailsPage;

    void initSideMenu();
    void initStackedWidget();
    void initUIInstances();

    void switchToScreen(Screen screen);
};