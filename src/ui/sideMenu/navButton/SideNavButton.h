#pragma once

#include "ui/sideMenu/SideMenu.h"
#include <QPushButton>
#include <qwidget.h>

class SideNavButton : public QPushButton {
    Q_OBJECT;

public:
    explicit SideNavButton(const SideNavButtonInfo& info, QWidget* parent = nullptr);
    void setSelected(bool selected);
};