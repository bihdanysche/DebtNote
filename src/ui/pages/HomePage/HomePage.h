#pragma once

#include <QWidget>

class MainWindow;
class BalanceWidget;
class ActiveContactsWidget;

class HomePage : public QWidget {
    Q_OBJECT

public:
    explicit HomePage(MainWindow* mv);

    void reload();
signals:
    void viewContactRequested(qint64 id);
private:
    BalanceWidget* m_balanceWidget;
    ActiveContactsWidget* m_activeContactsWidget;
};