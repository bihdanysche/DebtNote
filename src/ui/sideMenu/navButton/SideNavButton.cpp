#include "SideNavButton.h"

#include <QIcon>
#include <QLabel>
#include <QStyle>
#include <QVBoxLayout>

SideNavButton::SideNavButton(const SideNavButtonInfo& info, QWidget* parent)
    : QPushButton(parent)
{
    setObjectName("SideNavButton");
    setFixedSize(85, 85);
    setCursor(Qt::PointingHandCursor);
    setProperty("selected", false);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 10, 4, 6);
    layout->setSpacing(2);

    auto* iconLabel = new QLabel(this);
    iconLabel->setPixmap(QIcon(info.iconPath).pixmap(28, 28));
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    auto* titleLabel = new QLabel(info.title, this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    layout->addWidget(iconLabel);
    layout->addWidget(titleLabel);
}

void SideNavButton::setSelected(bool selected) {
    setProperty("selected", selected);

    style()->unpolish(this);
    style()->polish(this);

    for (QWidget* child : findChildren<QWidget*>()) {
        child->style()->unpolish(child);
        child->style()->polish(child);
    }
}