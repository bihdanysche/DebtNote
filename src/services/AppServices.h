#pragma once

class ContactService;
class PaymentService;
class BalanceService;
class AttachmentService;

struct AppServices {
    ContactService* contactService = nullptr;
    PaymentService* paymentService = nullptr;
    BalanceService* balanceService = nullptr;
    AttachmentService* attachmentService = nullptr;
};