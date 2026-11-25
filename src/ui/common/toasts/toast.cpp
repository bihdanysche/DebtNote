#include "toast.h"
#include <qboxlayout.h>
#include <qframe.h>
#include <qlabel.h>
#include <qnamespace.h>
#include <qpropertyanimation.h>
#include <qpushbutton.h>
#include <qtimer.h>
#include <qwindowdefs.h>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

Toast::Toast(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* centerLayout = new QVBoxLayout(this);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(0);

    QFrame* toastsCont = new QFrame();
    mainLay = new QVBoxLayout(toastsCont);
    mainLay->setSizeConstraint(QLayout::SetMinimumSize);
    mainLay->setContentsMargins(0, 0, 0, 0);
    mainLay->setSpacing(5);

    centerLayout->addStretch();
    centerLayout->addWidget(toastsCont, 0, Qt::AlignHCenter);
    centerLayout->addStretch();

    setAttribute(Qt::WA_StyledBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setObjectName("toasts_cont");

    setGeometry(parentWidget()->rect());
}

void Toast::makeToast(QString text, int durationms) {
    raise();
    QFrame* f = new QFrame();
    f->setProperty("class", "toast");
    f->setMaximumWidth(500);

    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(f);
    f->setGraphicsEffect(effect);
    effect->setOpacity(0.0);

    QVBoxLayout* l = new QVBoxLayout(f);
    l->setContentsMargins(20, 13, 20, 13);
    l->setSpacing(0);

    QLabel* titleLbl = new QLabel(text);
    titleLbl->setProperty("class", "toast_text");
    titleLbl->setWordWrap(true);
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setMinimumHeight(titleLbl->sizeHint().height());
    l->addWidget(titleLbl);

    mainLay->addWidget(f, 0, Qt::AlignHCenter);

    QPropertyAnimation* anim = new QPropertyAnimation(effect, "opacity", f);
    anim->setDuration(200);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::KeepWhenStopped);
    
    QTimer::singleShot(durationms, f, [this,anim,f,effect](){
        anim->setStartValue(1.0);
        anim->setEndValue(0.0);
        
        connect(anim, &QPropertyAnimation::finished, f, [this, f]() {
            mainLay->removeWidget(f);
            f->deleteLater();
        });

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    });
}