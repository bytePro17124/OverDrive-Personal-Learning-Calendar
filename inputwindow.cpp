#include "inputwindow.h"
#include "ui_inputwindow.h"
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include "learnitem.h"
#include "availabilitycalendar.h"
#include <random>
#include <QTextStream>
#include <QDateTime>

extern QVector<LearnItem> learnlist;
extern QString FullCalendar;

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
    //PROCESS LEARN FILE
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
        QFile learnfile(filename);
        if (!learnfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << learnfile.errorString();
        }
        QString AllLearnData = learnfile.readAll();
        qDebug() << AllLearnData;
        if (learnfile.isOpen()) learnfile.close();
            //makes a vector list of all the data in the learnfile provided
        while (!AllLearnData.isEmpty()) {
            QString tmpname;
            int index = AllLearnData.indexOf(',');
            qDebug() << index;
            tmpname.resize(index);
            for (int i = 0; i < index; i++) { tmpname[i] = AllLearnData[i]; }
            qDebug() << tmpname;
            AllLearnData.replace(0, index+1, "");
            qDebug() << AllLearnData;
            index = AllLearnData.indexOf('\n');
            qDebug() << index;
            QString tmp;
            for (int i = 0; i < index; i++) { tmp[i] = AllLearnData[i]; }
            int tmppriority = tmp.toInt();
            AllLearnData.replace(0, index+1, "");
            qDebug() << tmppriority;
            qDebug() << AllLearnData;
            LearnItem * newitem = new LearnItem;
            newitem->setName(tmpname);
            newitem->setPriority(tmppriority);
            learnlist.push_back(*newitem);
            delete newitem;
        }
        //sort all the items by priority  //BUBBLESORT
        int workingsize = learnlist.size();
        bool swaphappened = false;
        qDebug() << "Sorting...";
        do {
            swaphappened = false;
            for (int i = 1; i < workingsize; i++) {
                if (learnlist[i-1].getPriority() < learnlist[i].getPriority()) {
                    LearnItem tmp;
                    tmp.setName(learnlist[i-1].getName());
                    tmp.setPriority(learnlist[i-1].getPriority());
                    learnlist[i-1].setName(learnlist[i].getName());
                    learnlist[i-1].setPriority(learnlist[i].getPriority());
                    learnlist[i].setName(tmp.getName());
                    learnlist[i].setPriority(tmp.getPriority());
                    qDebug() << "Swap! " << i;
                    swaphappened = true;
                }
            }
        } while (swaphappened);
        for (int i = 0; i < learnlist.size(); i++) {
            qDebug() << learnlist[i].getPriority() << " | " << learnlist[i].getName();
        }
        //PROCESS SCHEDULE FILE
 //       AvailabilityCalendar calendar[7][24];
        QString filename2 = (ui->lineEdit_csvPathForSchedule->text());
        qDebug() << "Your schedule file is " << filename2;
        QFile calfile(filename2);
        if (!calfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << calfile.errorString();
        }
        QString AllCalData = calfile.readAll();
        qDebug() << AllCalData;
        if (calfile.isOpen()) calfile.close();

        int freedays = AllCalData.count("FREE");     //Find the number of free daysW
        qDebug() << "Free Days: " << freedays;
        if (learnlist.size() > freedays) {    //If there are less free days than there are learnitems, cut some least important learn items
            for (int i = 0; i < freedays; i++) {
                learnlist.pop_back();
            }
        }
        for (int i = 0; i < learnlist.size(); i++) {
            int randomarea = rand() % freedays + 1;
            qDebug() << "Random Area: " << randomarea;
            int currentpos = 0;
            for (int j = 0; j < randomarea; j++) {
                if (currentpos == 0) currentpos = AllCalData.indexOf("FREE");
                else currentpos = AllCalData.indexOf("FREE", currentpos+4);
            }
            AllCalData.replace(currentpos, 4, learnlist[i].getName());
            freedays--;
        }
        qDebug() << AllCalData;

        do {
            if (freedays > 0) {
                int tmp = (learnlist.size() / 2);  //take the remaining and assign the top half again to any free space
                for (int i = 0; i < tmp; i++) {
                    learnlist.pop_back();
                }
                qDebug() << " top half of learn list";
                for (int i = 0; i < learnlist.size(); i++) {
                    qDebug() << learnlist[i].getPriority() << " | " << learnlist[i].getName();
                }
                if (learnlist.size() > freedays) {  //If there are less free days than there are learnitems, cut some least important learn items
                    for (int i = 0; i < freedays; i++) {
                        learnlist.pop_back();
                    }
                }
                for (int i = 0; i < learnlist.size(); i++) {
                    int randomarea = rand() % freedays + 1;
                    qDebug() << "Random Area: " << randomarea;
                    int currentpos = 0;
                    for (int j = 0; j < randomarea; j++) {
                        if (currentpos == 0) currentpos = AllCalData.indexOf("FREE");
                        else currentpos = AllCalData.indexOf("FREE", currentpos+4);
                    }
                    AllCalData.replace(currentpos, 4, learnlist[i].getName());
                    freedays--;
                }
            }
            qDebug() << AllCalData;
        } while (freedays > 0);   //repeat till none left
        //we will now have a filled out availability map
        FullCalendar = AllCalData;
        if(FullCalendar.size() > 10) {
            QMessageBox success;
            success.setText("Okay looks processed. <br>Try the create CSV button.");
            success.setWindowTitle("Good To Go");
            success.exec();
        }
    }

//    QDateTime dateNow;
//    dateNow.setDate(QDate::currentDate());
//    dateNow.setTime(QTime::currentTime());m
//    qDebug() << "Today's Date: " << dateNow.toString();

}

void InputWindow::on_button_MakeSchedule_released()
{
    QString NewSchedule = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
                tr("CSV File (*.csv)"));
    QFile CSVFile(NewSchedule);
    if(!CSVFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Unfinished business"), tr("*csv file has not been saved."));
        return;
    }
    QTextStream out(&CSVFile);
    out << FullCalendar;
    qDebug() << "Wrote to " << NewSchedule;
    CSVFile.close();
    QMessageBox success;
    success.setText("File has been written to your chosen location.");
    success.setWindowTitle("Good To Go");
    success.exec();

}

void InputWindow::on_button_MakeiCalFile_released()
{
    QString NewSchedule = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), tr("ics File (*.ics)"));
    QFile iCalFile(NewSchedule);
    if (!iCalFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Unfinished business"), tr("*ics file has not been saved."));
        return;
    }
    QTextStream out(&iCalFile);

    out << "BEGIN:VCALENDAR\n"
        << "METHOD:PUBLISH\n"
        << "VERSION:2.0\n"
        << "PROID:-\n";


                        //loop this part for each event
    out << "BEGIN:VEVENT\n"
        << "SUMMARY:";
        //name of event then a newline
    out << "testsummary\n";   //test

    out << "UID:";
        //uniqueID then newline
    out << "123456789\n";     //test

    out << "DTSTART:";
        //start time then a newline
    out << "20160310T090000\n";

    out << "DTEND:";
        //end time then a newline
    out << "20160310T100000\n";

    out << "BEING:VALARM\n"
        << "TRIGGER:-PT15M\n"
        << "REPEAT:1\n"
        << "DURATION:PT15M\n"
        << "ACTION:DISPLAY\n";

    out << "END:VEVENT\n";
                        //end loop

        //finish out calendar file
    out << "END:VCALENDAR\n";


    qDebug() << "Wrote to " << NewSchedule;
    iCalFile.close();
    QMessageBox success;
    success.setText("Test File has been written to your chosen location.");
    success.setWindowTitle("Good To Go");
    success.exec();

}
