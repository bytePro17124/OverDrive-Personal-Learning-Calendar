#include "inputwindow.h"
#include <QApplication>
#include "learnitem.h"
#include <QVector>
#include <QString>

QVector<LearnItem> learnlist;
QString FullCalendar;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InputWindow w;
    w.show();

    return a.exec();
}

