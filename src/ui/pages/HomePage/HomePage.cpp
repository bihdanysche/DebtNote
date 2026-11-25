#include "HomePage.h"
#include "ui/mainwindow.h"
#include <qboxlayout.h>
#include <qlabel.h>

HomePage::HomePage(MainWindow* mv)
    : QWidget(mv)
{
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setContentsMargins(10, 10, 10, 10);
    lay->setSpacing(0);

    QLabel *tempLbl = new QLabel("This is a home page!");
    lay->addWidget(tempLbl);
    lay->addStretch();
}