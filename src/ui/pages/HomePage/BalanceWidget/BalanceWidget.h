#pragma once

#include <QWidget>

class MainWindow;
class BalanceService;
class Toast;
class QLabel;

class BalanceWidget : public QWidget {
    Q_OBJECT

public:
    explicit BalanceWidget(MainWindow* mv);

    void reload();

private:
    BalanceService* m_balanceService;
    Toast* m_toast;

    QLabel* m_owedToMeAmount;
    QLabel* m_owedToMeDescription;

    QLabel* m_iOweAmount;
    QLabel* m_iOweDescription;

    void reset();
};