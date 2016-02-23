#ifndef AVAILABILITYCALENDAR_H
#define AVAILABILITYCALENDAR_H


class AvailabilityCalendar
{
public:
    AvailabilityCalendar();
    enum DayOfTheWeek { MONDAY = 0, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY };
    enum HourOfTheDay { MIDNIGHT = 0, ONEAM, TWOAM, THREEAM, FOURAM, FIVEAM, SIXAM, SEVENAM, \
                      EIGHTAM, NINEAM, TENAM, ELEVENAM, NOON, ONEPM, TWOPM, THREEPM, FOURPM, \
                      FIVEPM, SIXPM, SEVENPM, EIGHTPM, NINEPM, TENPM, ELEVENPM };
    DayOfTheWeek day_column;
    HourOfTheDay hour_row;
private:
};

#endif // AVAILABILITYCALENDAR_H
