#include "LocalFileStorage.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QUuid>

LocalFileStorage::LocalFileStorage(const QString& baseDir)
    : m_baseDir(QDir::cleanPath(QDir(baseDir).absolutePath()))
{}

QString LocalFileStorage::defaultDir() {
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/attachments";
}

std::optional<QString> LocalFileStorage::copyIn(const QString& sourcePath, QString* error) {
    const QFileInfo source(sourcePath);

    if (!source.exists() || !source.isFile()) {
        if (error) *error = "Файл не знайдено";
        return std::nullopt;
    }

    if (!QDir().mkpath(m_baseDir)) {
        if (error) *error = "Не вдалося створити папку для вкладень";
        return std::nullopt;
    }

    QString name = QUuid::createUuid().toString(QUuid::WithoutBraces);
    if (!source.suffix().isEmpty()) {
        name += "." + source.suffix();
    }

    const QString target = m_baseDir + "/" + name;

    if (!QFile::copy(source.absoluteFilePath(), target)) {
        if (error) *error = "Не вдалося скопіювати файл";
        return std::nullopt;
    }

    return target;
}

bool LocalFileStorage::removeCopy(const QString& storedPath, QString* error) {
    const QString path = QDir::cleanPath(QFileInfo(storedPath).absoluteFilePath());

    if (!path.startsWith(m_baseDir + "/")) {
        if (error) *error = "Файл знаходиться поза сховищем застосунку";
        return false;
    }

    if (!QFile::exists(path)) {
        return true;
    }

    if (!QFile::remove(path)) {
        if (error) *error = "Не вдалося видалити файл";
        return false;
    }

    return true;
}