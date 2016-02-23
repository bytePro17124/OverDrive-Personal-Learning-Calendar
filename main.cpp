#include "inputwindow.h"
#include <QApplication>
#include "learnitem.h"
#include <QVector>

QVector<LearnItem> learnlist;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InputWindow w;
    w.show();

    return a.exec();
}

