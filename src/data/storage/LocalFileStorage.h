#pragma once

#include "domain/repositories/IFileStorage.h"

class LocalFileStorage : public IFileStorage {
public:
    explicit LocalFileStorage(const QString& baseDir);

    static QString defaultDir();

    std::optional<QString> copyIn(const QString& sourcePath, QString* error = nullptr) override;
    bool removeCopy(const QString& storedPath, QString* error = nullptr) override;

private:
    QString m_baseDir;
};