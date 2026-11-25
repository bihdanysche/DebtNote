#pragma once

#include <functional>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qtmetamacros.h>
#include <qwidget.h>
#include <QDialog>

class Toast : public QWidget {
    Q_OBJECT

public:
    explicit Toast(QWidget* parent = nullptr);
    void makeToast(QString text, int durationms = 2500);
private:
    QVBoxLayout* mainLay;
};