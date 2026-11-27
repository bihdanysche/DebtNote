#pragma once

#include "ui/common/dialogBase/DialogBase.h"
#include <functional>
#include <qboxlayout.h>
#include <qframe.h>
#include <qlabel.h>
#include <qobject.h>
#include <qparallelanimationgroup.h>
#include <qpropertyanimation.h>
#include <qpushbutton.h>
#include <qtmetamacros.h>
#include <qvariantanimation.h>
#include <qwidget.h>
#include <QDialog>
#include <QParallelAnimationGroup>

struct DialogParams {
    QString title = "Діалог";
    QString desc = "Опис";
    QString confirmBtnText = "Так";
    QString cancelBtnText = "Ні";
};

class MainWindow;

class ConfirmDialog : public DialogBase {
    Q_OBJECT

public:
    explicit ConfirmDialog(MainWindow* mv);
    void showDialog(const DialogParams& params, std::function<void(int)> callback);
private:
    QLabel *titleLbl, *descLbl;
    QPushButton *confirmBtn, *cancelBtn;
};