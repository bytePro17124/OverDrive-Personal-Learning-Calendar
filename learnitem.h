#ifndef LEARNITEM_H
#define LEARNITEM_H

#include <QString>
#include <QDateTime>

class LearnItem
{
public:
    LearnItem();

    //Accessors
    QString getName() const;
    int getPriority() const;
    QString getStartDateAndTime() const;
    QString getEndDateAndTime() const;

    //Setters
    void setName(const QString&);
    void setPriority(const int&);
    void setStartDateAndTime(const QString&);
    void setEndDateAndTime(const QString&);



private:

    QString item_name;
    int priority;
    QDateTime start;
    QDateTime end;

};

#endif // LEARNITEM_H
