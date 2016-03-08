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

    void on_actionExit_triggered();

    void on_button_ProcessData_released();

    void on_button_MakeSchedule_released();

    void on_button_MakeiCalFile_released();


    void on_calendarWidget_DateOfStart_selectionChanged();

private:
    Ui::InputWindow *ui;
    QDate startingMonday;
    QDateTime dtStamp;


};

#endif // INPUTWINDOW_H
