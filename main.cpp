#include "inputwindow.h"
#include <QApplication>
#include "learnitem.h"
#include <QVector>
#include <QString>
#include "help.h"

QVector<LearnItem> learnlist;
QString FullCalendar;
extern bool helpOpen(false);


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    InputWindow w;
    w.show();

    return a.exec();
}

