#ifndef LEARNITEM_H
#define LEARNITEM_H
#include <QString>



class LearnItem
{
public:
    LearnItem();

    QString getName() const;
    int getPriority() const;

    void setName(const QString&);
    void setPriority(const int&);

private:

    QString item_name;
    int priority;

};

#endif // LEARNITEM_H
