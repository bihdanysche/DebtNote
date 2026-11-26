#include "DebtNoteApp.h"

#include <QApplication>
#include <QMessageBox>
#include <qcoreapplication.h>
#include <qdir.h>
#include <qlist.h>
#include <qlogging.h>
#include <qmessagebox.h>
#include "data/database/Database.h"
#include "data/repositories/SQLiteContactRepository.h"
#include "data/repositories/SQLitePaymentRepository.h"
#include "services/AppServices.h"
#include "services/PaymentService.h"
#include "ui/mainwindow.h"
#include "services/ContactService.h"
#include <QFontDatabase>

DebtNoteApp::DebtNoteApp(int argc, char *argv[])
    : QApplication(argc, argv)
{}

int DebtNoteApp::initialize() {
    QCoreApplication::setApplicationName("DebtNote");
    setWindowIcon(QIcon(":/icons/app.png"));

    setAppPalette();
    loadAppStyle();
    loadAppFonts();
    
    if (!connectDb()) {
        return 1;
    }
    
    initAppServices();

    m_window = std::make_unique<MainWindow>(m_appServices);
    m_window->show();

    return 0;
}

bool DebtNoteApp::connectDb() {
    QString err;

    if (!m_db.open(Database::defaultPath(), &err)) {
        QMessageBox::critical(nullptr, "Помилка з базою даних", "Не вдалось підключитись до бази даних.\n Помилка: " + err);
        return false;
    }

    if (m_db.isEmpty() && !m_db.createSchema(&err)) {
        QMessageBox::critical(nullptr, "Помилка з базою даних", "Не вдалось ініціювати базу даних.\n Помилка: " + err);
        return false;
    }

    return true;
}

void DebtNoteApp::initAppServices() {
    m_contactsRepo = std::make_unique<SQLiteContactRepository>(m_db.connection());
    m_contactsService = std::make_unique<ContactService>(*m_contactsRepo);

    m_paymentsRepo = std::make_unique<SQLitePaymentRepository>(m_db.connection());
    m_paymentService = std::make_unique<PaymentService>(*m_paymentsRepo);

    m_appServices.contactService = m_contactsService.get();
    m_appServices.paymentService = m_paymentService.get();
}

void DebtNoteApp::loadAppStyle() {
    QFile styleFile(":/styles/main.qss");

    if (!styleFile.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << "Failed load app styles.";
        return;   
    }

    this->setStyle("Fusion");
    this->setStyleSheet(styleFile.readAll());

    return;
}

void DebtNoteApp::loadAppFonts() {
     const QStringList fonts = {
        ":/fonts/NotoSans-Regular.ttf",
        ":/fonts/NotoSans-Bold.ttf",
        ":/fonts/NotoSans-Italic.ttf"
    };

    for (const QString& path : fonts) {
        if (QFontDatabase::addApplicationFont(path) == -1) {
            qWarning() << "Failed to load font " << path;
        }
    }

    this->setFont(QFont("Noto Sans", 10));
}

void DebtNoteApp::setAppPalette() {
    QPalette p;

    p.setColor(QPalette::Window, QColor("#F3F8F5"));
    p.setColor(QPalette::WindowText, QColor("#1C2B24"));
    p.setColor(QPalette::Base, QColor("#FFFFFF"));
    p.setColor(QPalette::AlternateBase, QColor("#F3F8F5"));
    p.setColor(QPalette::Text, QColor("#1C2B24"));
    p.setColor(QPalette::PlaceholderText, QColor("#A6B5AB"));
    p.setColor(QPalette::Button, QColor("#FFFFFF"));
    p.setColor(QPalette::ButtonText, QColor("#1C2B24"));
    p.setColor(QPalette::ToolTipBase, QColor("#1C2B24"));
    p.setColor(QPalette::ToolTipText, QColor("#FFFFFF"));
    p.setColor(QPalette::Highlight, QColor("#2E8B57"));
    p.setColor(QPalette::HighlightedText, QColor("#FFFFFF"));
    p.setColor(QPalette::Link, QColor("#2E8B57"));
    p.setColor(QPalette::Disabled, QPalette::Text, QColor("#A6B5AB"));
    p.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#A6B5AB"));
    
    this->setPalette(p);
}

DebtNoteApp::~DebtNoteApp() = default;