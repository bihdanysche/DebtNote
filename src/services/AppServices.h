#pragma once

class ContactService;
class PaymentService;

struct AppServices {
    ContactService* contactService = nullptr;
    PaymentService* paymentService = nullptr;
};