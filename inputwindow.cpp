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
#include <QHostInfo>
#include <QVector>

extern QVector<LearnItem> learnlist;
extern QString FullCalendar;

InputWindow::InputWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InputWindow)
{
    ui->setupUi(this);
    dtStamp.setDate(QDate::currentDate());
    dtStamp.setTime(QTime::currentTime());
    qDebug() << QDate::currentDate().weekNumber();
    ui->combo_WeekOfYearNumber1to52->setCurrentIndex(QDate::currentDate().weekNumber()); //<-wish i would have saw this last night
    //dont worry about setting the year for now, its fine.

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
        QString filename2 = (ui->lineEdit_csvPathForSchedule->text());
        qDebug() << "Your schedule file is " << filename2;
        QFile calfile(filename2);
        if (!calfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << calfile.errorString();
        }
        QString AllCalData = calfile.readAll();
        qDebug() << AllCalData;
        if (calfile.isOpen()) calfile.close();

        int freedays = AllCalData.count("FREE");     //Find the number of free days
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
        ui->textBrowser_InfoAboutSchedule->setText(FullCalendar);
        if(FullCalendar.size() > 10) {
            QMessageBox success;
            success.setText("Okay looks processed. <br>Try the create CSV button.");
            success.setWindowTitle("Good To Go");
            success.exec();
            learnlist.clear();
        }
    }


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
    //  format of the times:  yyyyMMddTHHmmss

   // std::vector<QString> DescriptionOfItem;
    QVector<QDateTime> StartTimeOfItem;
    QVector<QString> SummaryOfItem;

    QString NewScheduleFileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), tr("ics File (*.ics)"));
    QFile iCalFile(NewScheduleFileName);
    if (!iCalFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Unfinished business"), tr(".ics file has not been saved."));
        return;
    }
    QTextStream out(&iCalFile);

    //figure out which Monday we are starting on - DONE in test for weeks 10 and 11 in 2016


    //loop to go through the string and stop at item found or newline
    QDate tmpdatesetter;
    QTime tmptimesetter(0, 0, 0);
    QTime defaultTime(0, 0, 0);
    QString tmpItemDescription;
    for (int i = 1; i <= 24; i++) {
        for (int DayOfWeek = 0; DayOfWeek < 7; DayOfWeek++) {

            //for each day of the week at this time slot check if anything is scheduled
            // 0 = Monday

            tmpItemDescription = FullCalendar.section(',', ((DayOfWeek +((i-1)*7)+8)), ((DayOfWeek +((i-1)*7)+8)));

            if (!tmpItemDescription.contains("BUSY")) {

                tmpdatesetter = startingMonday.addDays(DayOfWeek); //set day of week

                switch (i) { //set time that item starts
                case 1: tmptimesetter = defaultTime;
                    break;
                default:
                    tmptimesetter = defaultTime;
                    tmptimesetter = tmptimesetter.addSecs(60*60*(i-1));
                }
                SummaryOfItem.push_back(tmpItemDescription);

                QDateTime tmpStartTime;
                tmpStartTime.setDate(tmpdatesetter);
                tmpStartTime.setTime(tmptimesetter);
                StartTimeOfItem.push_back(tmpStartTime);
            }
        }
    }
    //if item found, save it and the start time to vector maps
    //if newline is found, increment stamp and move to next lie to process
    //after end, loop vevent and valarm and populate the 4 fields.
    //test

    //This part creates the actual openable file for Outlook or iCalendar

    //default header for ics file - testing currently
    out << "BEGIN:VCALENDAR\n"
        << "PRODID:-//Microsoft Corporation//Outlook 15.0 MIMEDIR//EN\n"
        << "VERSION:2.0\n"
        << "METHOD:PUBLISH\n";
    //out << "X-MS-OLK-FORCEINSPECTOROPEN:TRUE\n";
    QString timezone = "\"Eastern Standard Time\"";
    //Time Zone - Eastern Only for initial testing, user chosen later
    out << "BEGIN:VTIMEZONE\n"
        << "TZID:Eastern Standard Time\n"
        << "BEGIN:STANDARD\n"
        << "DTSTART:16011104T020000\n"
        << "RRULE:FREQ=YEARLY;BYDAY=1SU;BYMONTH=11\n"
        << "TZOFFSETFROM:-0400\n"
        << "TZOFFSETTO:-0500\n"
        << "END:STANDARD\n";

    //Daylight Savings Time Specifications in Eastern time zone
    out << "BEGIN:DAYLIGHT\n"
        << "DTSTART:16010311T020000\n"
        << "RRULE:FREQ=YEARLY;BYDAY=2SU;BYMONTH=3\n"
        << "TZOFFSETFROM:-0500\n"
        << "TZOFFSETTO:-0400\n"
        << "END:DAYLIGHT\n"
        << "END:VTIMEZONE\n";

    //The Events Part - Loop for each event to schedule //duration 1hr to start
   // auto j = SummaryOfItem.size();
    for (int i = 0, j = SummaryOfItem.size(); i < j; i++) {
    out << "BEGIN:VEVENT\n"
        << "CLASS:PUBLIC\n";
    QDateTime utcStamp(QDateTime::currentDateTimeUtc());
    out << "CREATED:" << utcStamp.toString("yyyyMMddTHHmmsszzzZ") << "\n"
        //<< "DESCRIPTION:" /* << optional description*/ << "\n"
        //<< "DTEND;TZID=" /* << learn-item date-time end*/ << "\n"
        << "DTSTAMP:" << utcStamp.toString("yyyyMMddTHHmmsszzzZ") << "\n"
        << "DTSTART;TZID=" << timezone << ":" << StartTimeOfItem[i].toString("yyyyMMddTHHmmss") /* << learn-item date-time start*/ << "\n"
        << "DURATION:PT1H0M0S\n"
        << "LAST-MODIFIED:" << utcStamp.toString("yyyyMMddTHHmmsszzzZ") << "\n"
        << "PRIORITY:0\n"
        << "SEQUENCE:0\n"
        << "SUMMARY;LANGUAGE=en-us:" << SummaryOfItem[i] /* << learn-item name here */ << "\n"
        << "TRANSP:TRANSPARENT\n";
    out << "UID:" << utcStamp.toString("yyyyMMddTHHmmsszzzZ") << "@" << QHostInfo::localHostName() << "\n";

    //Alert type and setup - must be part of VEVENT so make part of loop 10 min popup for now
    out << "BEGIN:VALARM\n"
        << "TRIGGER:-PT10M\n"
        << "ACTION:DISPLAY\n"
        << "END:VALARM\n"
        << "END:VEVENT\n";          //end loop
    }

    out << "END:VCALENDAR\n";       //finish out calendar file


    qDebug() << "Wrote to " << NewScheduleFileName;
    iCalFile.close();
    QMessageBox success;
    success.setText("Test File has been written to your chosen location.");
    success.setWindowTitle("Good To Go");
    success.exec();

}

void InputWindow::on_combo_WeekOfYearNumber1to52_currentIndexChanged(int index)
{
    startingMonday = setMonday(index);
}

QDate InputWindow::setMonday(const int & week, const int & year)
{
    //use dayofweek(1-7:mon-sun) and day(1-31:dayofmonth) to help calculate
        //the proper monday for any given year.
    int m, d;
    switch (week) {
    case 10:
        m = 3;
        d = 7;
        break;
    case 11:
        m = 3;
        d = 14;
        break;
    default:;
    }
    QDate tmpdate(year, m, d);
    return tmpdate;
}

void InputWindow::on_combo_Year_1eq2016_currentIndexChanged(const QString &arg1)
{
    //check if it is a 53 monday year and update the week combobox accordingly

    int tmpyear = arg1.toInt();
    switch (tmpyear) {
    case 2018:
        if (ui->combo_WeekOfYearNumber1to52->count() == 52) {
            ui->combo_WeekOfYearNumber1to52->addItem("Week 53");
        }
        break;
    case 2024:
        if (ui->combo_WeekOfYearNumber1to52->count() == 52) {
            ui->combo_WeekOfYearNumber1to52->addItem("Week 53");
        }
        break;
    default:
        if (ui->combo_WeekOfYearNumber1to52->count() > 52) {
            qDebug() << "Trying to remove week 53 if it exists";
            ui->combo_WeekOfYearNumber1to52->removeItem(ui->combo_WeekOfYearNumber1to52->count()-1);
        }
    }
}
