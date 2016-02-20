#include "inputwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InputWindow w;
    w.show();

    return a.exec();
}
