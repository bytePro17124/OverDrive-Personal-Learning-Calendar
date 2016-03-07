#ifndef LEARNITEM_H
#define LEARNITEM_H

#include <QString>

class LearnItem
{
public:
    LearnItem();

    //Accessors
    QString getName() const;
    int getPriority() const;

    //Setters
    void setName(const QString&);
    void setPriority(const int&);



private:

    QString item_name;
    int priority;


};

#endif // LEARNITEM_H
