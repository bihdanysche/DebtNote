#include "ConfirmDialog.h"
#include "ui/common/dialogBase/DialogBase.h"
#include "ui/mainwindow.h"

ConfirmDialog::ConfirmDialog(MainWindow* mv)
    : DialogBase(mv)
{
    innerFrame->setMaximumWidth(600);
    innerFrame->setMinimumWidth(500);

    QVBoxLayout* l = new QVBoxLayout(innerFrame);
    l->setContentsMargins(15, 15, 15, 15);
    l->setSpacing(10);

    titleLbl = new QLabel();
    titleLbl->setObjectName("d_title");
    l->addWidget(titleLbl);

    descLbl = new QLabel();
    descLbl->setObjectName("d_desc");
    descLbl->setWordWrap(true);
    l->addWidget(descLbl);

    QFrame* buttonsCont = new QFrame();
    QHBoxLayout* buttonsLay = new QHBoxLayout(buttonsCont);
    buttonsLay->setContentsMargins(0,0,0,0);
    buttonsLay->setSpacing(5);
    buttonsLay->setAlignment(Qt::AlignmentFlag::AlignRight);

    confirmBtn = new QPushButton("Так");
    confirmBtn->setProperty("variant", "primary");
    buttonsLay->addWidget(confirmBtn);

    cancelBtn = new QPushButton("Ні");
    cancelBtn->setProperty("variant", "danger");
    buttonsLay->addWidget(cancelBtn);

    l->addWidget(buttonsCont);
}

void ConfirmDialog::showDialog(const DialogParams& params, std::function<void(int)> callback) {
    titleLbl->setText(params.title);
    descLbl->setText(params.desc);
    confirmBtn->setText(params.confirmBtnText);
    cancelBtn->setText(params.cancelBtnText);

    show();

    disconnect(confirmBtn, nullptr, this, nullptr);
    disconnect(cancelBtn, nullptr, this, nullptr);

    connect(confirmBtn, &QPushButton::clicked, this, [this, callback](){
        hide();
        if (callback) callback(1);
    });

    connect(cancelBtn, &QPushButton::clicked, this, [this, callback](){
        hide();
        if (callback) callback(0);
    });
}