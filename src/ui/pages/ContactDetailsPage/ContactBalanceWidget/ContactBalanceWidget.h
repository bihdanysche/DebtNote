#pragma once

#include <QFrame>
#include <qlabel.h>

class MainWindow;
class BalanceService;
class Toast;

class ContactBalanceWidget : public QFrame {
    Q_OBJECT;
public:
    explicit ContactBalanceWidget(MainWindow *mv);
    void reload();
    void reset();
    void setContactId(qint64 contactId) { m_contactId = contactId; reload(); };
private:
    qint64 m_contactId = -1;
    Toast* m_toast;
    BalanceService* m_balanceService;

    QLabel* m_balanceAmount;
    QLabel* m_balanceDescription;
};