#pragma once

#include <QWidget>

class MainWindow;
class BalanceService;
class Toast;
class QVBoxLayout;
class QScrollArea;
class QWidget;

class ActiveContactsWidget : public QWidget {
    Q_OBJECT

public:
    explicit ActiveContactsWidget(MainWindow* mv);

    void reload();

signals:
    void showContactRequested(qint64 id);

private:
    BalanceService* m_balanceService;
    Toast* m_toast;

    QScrollArea* m_scrollArea;
    QWidget* m_itemsContainer;
    QVBoxLayout* m_itemsLayout;

    void clearItems();
};