#pragma once

#include <qboxlayout.h>
#include <qframe.h>
#include <qparallelanimationgroup.h>
#include <qpropertyanimation.h>
#include <qtmetamacros.h>
#include <qwidget.h>

class DialogBase : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal overlayOpacity READ overlayOpacity WRITE setOverlayOpacity)

public:
    explicit DialogBase(QWidget* parent = nullptr);

    qreal overlayOpacity() const { return m_opacity; }
    void setOverlayOpacity(qreal opacity);
private:
    qreal m_opacity = 1.0;

    QPropertyAnimation *overlayAnim;
    QVariantAnimation *layoutAnim;
    QParallelAnimationGroup* animsGroup;

    void initAnims();
    void playAnims(bool in);
    void hideWithAnim();
protected:
    void show();
    void hide();
    void paintEvent(QPaintEvent *) override;

    QVBoxLayout *overlayLayout;
    QFrame *innerFrame;
};