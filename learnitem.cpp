#include "learnitem.h"

LearnItem::LearnItem()
{
    item_name = "";
    priority = 0;
}

QString LearnItem::getName() const {
    return item_name;
}
int LearnItem::getPriority() const {
    return priority;
}

void LearnItem::setName(const QString& n) {
    item_name = n;
}

void LearnItem::setPriority(const int& p) {
    priority = p;
}
