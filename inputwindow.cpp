#include "inputwindow.h"
#include "ui_inputwindow.h"
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include "learnitem.h"

extern LearnItem learnschedule;
extern QVector<LearnItem> learnlist;

InputWindow::InputWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InputWindow)
{
    ui->setupUi(this);
}

InputWindow::~InputWindow()
{
    delete ui;
}

void InputWindow::on_lineEdit_csvPathForItems_selectionChanged()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open CSV"),\
                                            "c:/users", tr("CSV Files(*.csv)"));
    ui->lineEdit_csvPathForItems->setText(filename);
}


void InputWindow::on_lineEdit_csvPathForSchedule_selectionChanged()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open CSV"),\
                                            "c:/users", tr("CSV Files(*.csv)"));
    ui->lineEdit_csvPathForSchedule->setText(filename);
}


void InputWindow::on_button_Quit_released()
{
    qApp->quit();
}

void InputWindow::on_actionExit_triggered()
{
    qApp->quit();
}

void InputWindow::on_button_ProcessData_released()
{
    //test if there is valid data in both fields and then process a schedule

    if (ui->lineEdit_csvPathForItems->text().size() == 0) {
        QMessageBox invalid;
        invalid.setText("Be sure to pick an Item File");
        invalid.setWindowTitle("Cannot Continue");
        invalid.exec();
    } else if (ui->lineEdit_csvPathForSchedule->text().size() == 0) {
        QMessageBox invalid;
        invalid.setText("Be sure to pick an Schedule File");
        invalid.setWindowTitle("Cannot Continue");
        invalid.exec();
    } else {
        QString filename = (ui->lineEdit_csvPathForItems->text());
        qDebug() << "Your item file is " << filename;
        QString filename2 = (ui->lineEdit_csvPathForSchedule->text());
        qDebug() << "Your schedule file is " << filename2;

        QFile learnfile(filename);
        if (!learnfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << learnfile.errorString();
        }
        while (!learnfile.atEnd()) {
            LearnItem* tmp = new LearnItem;
            tmp->setName(learnfile.readLine());
            qDebug() << tmp->getName();
        }

    }
}
