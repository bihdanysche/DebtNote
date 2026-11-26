#pragma once

class ContactService;
class PaymentService;
class BalanceService;

struct AppServices {
    ContactService* contactService = nullptr;
    PaymentService* paymentService = nullptr;
    BalanceService* balanceService = nullptr;
};