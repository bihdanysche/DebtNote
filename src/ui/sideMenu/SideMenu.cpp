#include "SideMenu.h"
#include "navButton/SideNavButton.h"

#include <QVBoxLayout>
#include <qlabel.h>

SideMenu::SideMenu(const QList<SideNavButtonInfo>& buttons, QWidget* parent)
    : QFrame(parent)
{
    setObjectName("SideMenu");
    setFixedWidth(140);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    layout->addWidget(logoSection());

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

QWidget* SideMenu::logoSection() {
    QWidget* fr = new QWidget();
    fr->setObjectName("sidemenuLogo");

    QVBoxLayout* lay = new QVBoxLayout(fr);
    lay->setContentsMargins(15,15,15,15);
    lay->setSpacing(0);

    QLabel* icn = new QLabel(fr);
    icn->setObjectName("icn");
    icn->setPixmap(
        QIcon(":/icons/app.png")
            .pixmap(QSize(55, 55))
    );
    lay->addWidget(icn, 0, Qt::AlignHCenter);

    lay->addSpacing(5);

    QLabel* appNameLbl = new QLabel("Debt Note", fr);
    appNameLbl->setObjectName("appName");
    lay->addWidget(appNameLbl, 0, Qt::AlignHCenter);
    
    // fr->setSizePolicy(
    //     QSizePolicy::Preferred, 
    //     QSizePolicy::Preferred
    // );
    // fr->adjustSize();
    return fr;
}