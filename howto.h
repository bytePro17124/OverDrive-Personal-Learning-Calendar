#ifndef HOWTO_H
#define HOWTO_H

#include<QMainWindow>


namespace Ui {
class howto;
}

class howto : public QMainWindow
{
    Q_OBJECT

public:
    explicit howto(QWidget *parent = 0);
    ~howto();

private:
    Ui::howto *ui;

};

#endif // HOWTO_H
