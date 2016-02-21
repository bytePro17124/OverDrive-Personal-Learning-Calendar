#include "inputwindow.h"
#include <QApplication>
#include "learnitem.h"
#include <QVector>

const int DAYS = 7;
const int HOURS = 24;
LearnItem learnschedule[DAYS][HOURS];

enum DayOfTheWeek { MONDAY = 0, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY };
DayOfTheWeek day_column;

enum HourOfTheDay { MIDNIGHT = 0, ONEAM, TWOAM, THREEAM, FOURAM, FIVEAM, SIXAM, SEVENAM, \
                  EIGHTAM, NINEAM, TENAM, ELEVENAM, NOON, ONEPM, TWOPM, THREEPM, FOURPM, \
                  FIVEPM, SIXPM, SEVENPM, EIGHTPM, NINEPM, TENPM, ELEVENPM };
HourOfTheDay hour_row;

QVector<LearnItem> learnlist;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InputWindow w;
    w.show();

    return a.exec();
}

