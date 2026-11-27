#pragma once

#include <QString>
#include <optional>

class IFileStorage {
public:
    virtual ~IFileStorage() = default;

    virtual std::optional<QString> copyIn(const QString& sourcePath, QString* error = nullptr) = 0;
    virtual bool removeCopy(const QString& storedPath, QString* error = nullptr) = 0;
};