#pragma once

#include <QString>

enum class PaymentType {
    Lent = 0,
    Borrowed = 1,
    ReceivedBack = 2,
    PaidBack = 3
};

inline QString paymentTypeTitle(PaymentType type) {
    switch (type) {
    case PaymentType::Lent:
        return "Я дав(-ла) у борг";
    case PaymentType::Borrowed:
        return "Я взяв(-ла) у борг";
    case PaymentType::ReceivedBack:
        return "Мені повернули борг";
    case PaymentType::PaidBack:
        return "Я повернув(-ла) борг";
    }
    return QString();
}