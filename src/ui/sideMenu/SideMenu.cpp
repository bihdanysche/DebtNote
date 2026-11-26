#include "SideMenu.h"
#include "navButton/SideNavButton.h"

#include <QVBoxLayout>

SideMenu::SideMenu(const QList<SideNavButtonInfo>& buttons, QWidget* parent)
    : QFrame(parent)
{
    setObjectName("SideMenu");
    setFixedWidth(120);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    for (const SideNavButtonInfo& info : buttons) {
        auto* button = new SideNavButton(info, this);
        m_buttons.insert(info.id, button);
        layout->addWidget(button);

        connect(button, &QPushButton::clicked, this, [this, id = info.id]() {
            setSelected(id);
            emit switchToScreen(id);
        });
    }
    layout->addStretch();

    if (!buttons.isEmpty()) {
        setSelected(buttons.first().id);
    }
}

void SideMenu::setSelected(int id) {
    for (auto it = m_buttons.cbegin(); it != m_buttons.cend(); ++it) {
        it.value()->setSelected(it.key() == id);
    }
}