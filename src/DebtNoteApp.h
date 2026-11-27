#include <memory>
#include <QApplication>
#include "data/database/Database.h"
#include "services/AppServices.h"

class MainWindow;

class SQLiteContactRepository;
class ContactService;

class SQLitePaymentRepository;
class PaymentService;

class SQLiteAttachmentRepository;
class LocalFileStorage;
class AttachmentService;

class BalanceService;

class DebtNoteApp : public QApplication {
public:
    DebtNoteApp(int argc, char* argv[]);
    ~DebtNoteApp() override;

    int initialize();

private:
    bool connectDb();
    void initAppServices();
    void loadAppStyle();
    void loadAppFonts();
    void setAppPalette();

    Database m_db;

    std::unique_ptr<SQLiteContactRepository> m_contactsRepo;
    std::unique_ptr<ContactService> m_contactsService;

    std::unique_ptr<SQLitePaymentRepository> m_paymentsRepo;
    std::unique_ptr<PaymentService> m_paymentService;

    std::unique_ptr<LocalFileStorage> m_fileStorage;
    std::unique_ptr<SQLiteAttachmentRepository> m_attachmentsRepo;
    std::unique_ptr<AttachmentService> m_attachmentsService;

    std::unique_ptr<BalanceService> m_balanceService;

    AppServices m_appServices;
    std::unique_ptr<MainWindow> m_window;
};