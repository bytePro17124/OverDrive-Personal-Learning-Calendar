#include "howto.h"

howto::howto(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::howto)
{
    ui->setupUi(this);
}

howto::~howto()
{
    delete ui;
}
