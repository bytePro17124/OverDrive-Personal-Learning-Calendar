#include "learnitem.h"

LearnItem::LearnItem()
{
    item_name = "";
    priority = 0;
}

QString getName() const {
    return item_name;
}
int getPriority() const{
    return priority;
}

void setName(const QString& n) {
    item_name = n;
}

void setPriority(const int& p) {
    priority = p;
}
