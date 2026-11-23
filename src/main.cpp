#include <QApplication>
#include <QMessageBox>
#include <qcoreapplication.h>
#include <qlist.h>
#include <qlogging.h>
#include "data/database/Database.h"
#include "data/repositories/SQLiteContactRepository.h"
#include "domain/entities/Contact.h"
#include "mainwindow.h"
#include "services/ContactService.h"

int main(int argc, char *argv[])
{
    qDebug() << "MAIN STARTED\n";
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("DebtNote");

    Database db;
    QString err;

    if (!db.open(Database::defaultPath(), &err)) {
        QMessageBox::critical(nullptr, "Не вдалось підключитись до бази даних.", err);
        return 1;
    }

    if (db.isEmpty() && !db.createSchema(&err)) {
        QMessageBox::critical(nullptr, "Не вдалось ініціювати базу даних.", err);
        return 1;
    }

    SQLiteContactRepository contactsRepo(db.connection());
    ContactService contactsService(contactsRepo);

    MainWindow window;
    window.show();

    return app.exec();
}