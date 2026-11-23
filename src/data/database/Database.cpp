#include "Database.h"

#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QStringList>

constexpr auto kConnectionName = "debtnote";

Database::~Database() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

QString Database::defaultPath() {
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dir);
    return dir + "/debtnote.db";
}

bool Database::open(const QString& filePath, QString* error) {
    m_db = QSqlDatabase::addDatabase("QSQLITE", kConnectionName);
    m_db.setDatabaseName(filePath);

    if (!m_db.open()) {
        if (error) *error = m_db.lastError().text();
        return false;
    }

    return execute("PRAGMA foreign_keys = ON", error);
}

bool Database::createSchema(QString* error) {
    QFile file(":/sql/schema.sql");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (error) *error = "Failed to read schema.sql file.";
        return false;
    }

    const QString script = QString::fromUtf8(file.readAll());
    const QStringList statements = script.split(';');

    for (const QString& statement : statements) {
        if (statement.trimmed().isEmpty()) {
            continue;
        }
        if (!execute(statement, error)) {
            return false;
        }
    }

    return true;
}

bool Database::execute(const QString& sql, QString* error) {
    QSqlQuery query(m_db);
    if (!query.exec(sql)) {
        if (error) *error = query.lastError().text();
        return false;
    }
    return true;
}

bool Database::isEmpty() const {
    return m_db.tables().isEmpty();
}