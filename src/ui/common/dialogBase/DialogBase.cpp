#include "DialogBase.h"
#include <qboxlayout.h>
#include <qeasingcurve.h>
#include <qframe.h>
#include <qgraphicseffect.h>
#include <qlabel.h>
#include <qmargins.h>
#include <qnamespace.h>
#include <qpainter.h>
#include <qparallelanimationgroup.h>
#include <qpropertyanimation.h>
#include <qpushbutton.h>
#include <qvariant.h>
#include <qvariantanimation.h>
#include <qwindowdefs.h>
#include <QGraphicsBlurEffect>

DialogBase::DialogBase(QWidget *parent)
    : QWidget(parent)
{
    overlayLayout = new QVBoxLayout(this);
    overlayLayout->setContentsMargins(0, 0, 0, 0);
    overlayLayout->setSpacing(0);
    
    innerFrame = new QFrame();
    innerFrame->setObjectName("d_inner");
    innerFrame->setMaximumWidth(600);
    innerFrame->setMinimumWidth(500);

    overlayLayout->addWidget(innerFrame, 0, Qt::AlignCenter);

    setWindowFlags(Qt::Widget);
    setAttribute(Qt::WA_StyledBackground);
    setObjectName("dialog");
    QWidget::hide();

    initAnims();
}

void DialogBase::initAnims() {
    layoutAnim = new QVariantAnimation(this);
    connect(layoutAnim, &QVariantAnimation::valueChanged, this, [this](const QVariant& qv) {
        int val = qv.toInt();
        int top = val > 0 ? val : 0;
        int bottom = val < 0 ? -val : 0;

        overlayLayout->setContentsMargins(0, top, 0, bottom);
    });
    
    overlayAnim = new QPropertyAnimation(this, "overlayOpacity", this);

    animsGroup = new QParallelAnimationGroup(this);
    animsGroup->addAnimation(layoutAnim);
    animsGroup->addAnimation(overlayAnim);
}

void DialogBase::playAnims(bool in) {
    animsGroup->stop();

    int durms = 400;

    if (in) {
        layoutAnim->setStartValue(-100);
        layoutAnim->setEndValue(0);
        layoutAnim->setEasingCurve(QEasingCurve::OutBounce);

        overlayAnim->setStartValue(0);
        overlayAnim->setEndValue(0.8);
        overlayAnim->setEasingCurve(QEasingCurve::OutExpo);
    } else {
        durms = 200;
        layoutAnim->setStartValue(0);
        layoutAnim->setEndValue(1000);
        layoutAnim->setEasingCurve(QEasingCurve::InExpo);

        overlayAnim->setStartValue(0.8);
        overlayAnim->setEndValue(0);
        overlayAnim->setEasingCurve(QEasingCurve::InExpo);
    }

    layoutAnim->setDuration(durms);
    overlayAnim->setDuration(durms+200);

    animsGroup->start();
}

void DialogBase::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.fillRect(rect(), QColor(0, 0, 0, static_cast<int>(255 * m_opacity)));
}

void DialogBase::setOverlayOpacity(qreal opacity) {
    if (qFuzzyCompare(m_opacity, opacity))
        return;

    m_opacity = opacity;
    update();
}

void DialogBase::show() {
    setGeometry(parentWidget()->rect());
    raise();
    innerFrame->raise();
    QWidget::show();

    playAnims(true);
}

void DialogBase::hide() {
    playAnims(false);
    connect(animsGroup, &QParallelAnimationGroup::finished, this, [this](){
        QWidget::hide();
    }, Qt::SingleShotConnection);
}