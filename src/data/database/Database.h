#pragma once

#include <QString>
#include <QSqlDatabase>

class Database {
public:
    Database() = default;
    ~Database();

    bool open(const QString& filePath, QString* error = nullptr);
    bool createSchema(QString* error = nullptr);

    QSqlDatabase connection() const { return m_db; }
    bool isEmpty() const;

    static QString defaultPath();

private:
    bool execute(const QString& sql, QString* error);

    QSqlDatabase m_db;
};