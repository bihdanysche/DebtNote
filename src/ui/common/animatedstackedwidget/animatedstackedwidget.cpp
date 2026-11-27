#include "animatedstackedwidget.h"
#include <qanimationgroup.h>
#include <qeasingcurve.h>
#include <qlabel.h>
#include <qnamespace.h>
#include <qpaintdevice.h>
#include <qpainter.h>
#include <qparallelanimationgroup.h>
#include <qpoint.h>
#include <qpropertyanimation.h>
#include <qstackedwidget.h>
#include <qwidget.h>

AnimatedStackedWidget::AnimatedStackedWidget(QWidget* parent)
    : QStackedWidget(parent) {}

void AnimatedStackedWidget::slideTo(int index, bool isForward) {
    if (index == currentIndex()) return;

    if (animGroup != nullptr) {
        animGroup->stop();
        delete animGroup;
        animGroup = nullptr;
    }

    QWidget* next = widget(index);

    int dur = 200;

    animGroup = new QParallelAnimationGroup(this);

    QPropertyAnimation* nextAnim = new QPropertyAnimation(next, "pos", animGroup);
    nextAnim->setStartValue(QPoint(0, isForward ? 60 : -60));
    nextAnim->setEndValue(QPoint(0, 0));
    nextAnim->setDuration(dur);
    nextAnim->setEasingCurve(QEasingCurve::OutCirc);

    animGroup->addAnimation(nextAnim);
    setCurrentIndex(index);

    connect(animGroup, &QParallelAnimationGroup::finished, this, [this, index, next]() {
        animGroup = nullptr;
        next->move(0, 0);
    });

    next->show();
    animGroup->start(QParallelAnimationGroup::DeleteWhenStopped);
}