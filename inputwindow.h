#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QMainWindow>

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

private:
    Ui::InputWindow *ui;
};

#endif // INPUTWINDOW_H
