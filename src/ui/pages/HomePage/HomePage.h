#pragma once

#include <QWidget>
#include <qwidget.h>

class MainWindow;

class HomePage : public QWidget {
    Q_OBJECT;

public:
    explicit HomePage(MainWindow* mainWindow);
};