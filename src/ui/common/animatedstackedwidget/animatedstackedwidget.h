#pragma once

#include <qparallelanimationgroup.h>
#include <qstackedwidget.h>
#include <qwidget.h>
class AnimatedStackedWidget : public QStackedWidget {
    Q_OBJECT;

public:
    explicit AnimatedStackedWidget(QWidget* parent = nullptr);
    void slideTo(int index, bool isForward = true);
private:
    QParallelAnimationGroup *animGroup = nullptr;
};