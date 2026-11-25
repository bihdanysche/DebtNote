#include "DebtNoteApp.h"

int main(int argc, char *argv[]) {
    DebtNoteApp app(argc, argv);

    if (int status = app.initialize(); status != 0) {
        return status;
    }

    return app.exec();
}