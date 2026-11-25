#pragma once

#include <QFrame>
#include <qhash.h>
#include <qlist.h>
#include <qmap.h>
#include <qobject.h>
#include <qtypes.h>

struct SideNavButtonInfo {
    QString title;
    QString iconPath;
    int id;
};

class SideNavButton;

class SideMenu : public QFrame {
    Q_OBJECT;
public:
    explicit SideMenu(const QList<SideNavButtonInfo>& buttons, QWidget* parent = nullptr);
    void setSelected(int id);
signals:
    void switchToScreen(int id);
private:
    QHash<int, SideNavButton*> m_buttons;
};