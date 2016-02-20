#include "inputwindow.h"
#include "ui_inputwindow.h"

InputWindow::InputWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InputWindow)
{
    ui->setupUi(this);
}

InputWindow::~InputWindow()
{
    delete ui;
}
