#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QMainWindow>
#include <QDate>

namespace Ui {
class InputWindow;
}

class InputWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit InputWindow(QWidget *parent = 0);
    ~InputWindow();

private slots:
    void on_lineEdit_csvPathForItems_selectionChanged();

    void on_lineEdit_csvPathForSchedule_selectionChanged();

    void on_button_Quit_released();

    void on_actionExit_triggered();

    void on_button_ProcessData_released();

    void on_button_MakeSchedule_released();

    void on_button_MakeiCalFile_released();

    void on_combo_WeekOfYearNumber1to52_currentIndexChanged(int index);

    void on_combo_Year_1eq2016_currentIndexChanged(const QString &arg1);

private:
    Ui::InputWindow *ui;
    QDate startingMonday;
    QDateTime dtStamp;
    int year_int;
    int month_int;
    int day_int;
    QDate setMonday(const int&, const int&);
};

#endif // INPUTWINDOW_H
