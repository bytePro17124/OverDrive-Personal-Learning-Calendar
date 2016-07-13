#include "help.h"
#include "ui_help.h"
#include "InputWindow.h"
#include <QDebug>

extern bool helpOpen;

help::help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::help)
{
    ui->setupUi(this);
}

help::~help()
{
    if (helpOpen == true) {
      //  qDebug() << "settings helpOpen to false";
        helpOpen = false;
    }
  //  qDebug() << "deleting help ui";
    delete ui;
}
