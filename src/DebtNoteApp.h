// DebtNoteApp.h
#include <memory>
#include <QApplication>
#include "data/database/Database.h"
#include "services/AppServices.h"

class SQLiteContactRepository;
class ContactService;
class MainWindow;

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

    AppServices m_appServices;
    std::unique_ptr<MainWindow> m_window;
};