#include "inputwindow.h"
#include "ui_inputwindow.h"
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
//#include <QDebug>
#include <QFile>
#include "learnitem.h"
#include "availabilitycalendar.h"
#include <random>
#include <QTextStream>
#include <QDateTime>
#include <QHostInfo>
#include <QVector>
#include <QCalendarWidget>


extern QVector<LearnItem> learnlist;
extern QString FullCalendar;

InputWindow::InputWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InputWindow)
{
    ui->setupUi(this);
    dtStamp.setDate(QDate::currentDate());
    dtStamp.setTime(QTime::currentTime());
}

InputWindow::~InputWindow()
{
    delete ui;
}

void InputWindow::on_lineEdit_csvPathForItems_selectionChanged()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open CSV that has your Learn Items"),\
                                                    "c:/users", tr("CSV Files(*.csv)"));
    ui->lineEdit_csvPathForItems->setText(filename);
}


void InputWindow::on_lineEdit_csvPathForSchedule_selectionChanged()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open CSV that has your Availability"),\
                                                    "c:/users", tr("CSV Files(*.csv)"));
    ui->lineEdit_csvPathForSchedule->setText(filename);
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
        invalid.setText("Be sure to pick an LearnFile (above)");
        invalid.setWindowTitle("Cannot Continue");
        invalid.exec();
    } else if (ui->lineEdit_csvPathForSchedule->text().size() == 0) {
        QMessageBox invalid;
        invalid.setText("Be sure to pick an Schedule File (above)");
        invalid.setWindowTitle("Cannot Continue");
        invalid.exec();
    } else {
        FullCalendar.clear();
        QString filename = (ui->lineEdit_csvPathForItems->text());
//        qDebug() << "Your item file is " << filename;
        QFile learnfile(filename);
        if (!learnfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << learnfile.errorString();
        }
        QString AllLearnData = learnfile.readAll();
        //     qDebug() << AllLearnData;
        if (learnfile.isOpen()) learnfile.close();
        //makes a vector list of all the data in the learnfile provided
        while (!AllLearnData.isEmpty()) {
            QString tmpname;
            int index = AllLearnData.indexOf(',');
            //   qDebug() << index;
            tmpname.resize(index);
            for (int i = 0; i < index; i++) { tmpname[i] = AllLearnData[i]; }
            //     qDebug() << tmpname;
            AllLearnData.replace(0, index+1, "");
            //      qDebug() << AllLearnData;
            index = AllLearnData.indexOf('\n');
            //      qDebug() << index;
            QString tmp;
            for (int i = 0; i < index; i++) { tmp[i] = AllLearnData[i]; }
            int tmppriority = tmp.toInt();
            AllLearnData.replace(0, index+1, "");
            //        qDebug() << tmppriority;
            //       qDebug() << AllLearnData;
            LearnItem * newitem = new LearnItem;
            newitem->setName(tmpname);
            newitem->setPriority(tmppriority);
            learnlist.push_back(*newitem);
            delete newitem;
        }
        //sort all the items by priority  //BUBBLESORT
        int workingsize = learnlist.size();
        bool swaphappened = false;
//        qDebug() << "Sorting...";
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
                    //  qDebug() << "Swap! " << i;
                    swaphappened = true;
                }
            }
        } while (swaphappened);
//        qDebug() << "Sorting Complete";
//        for (int i = 0; i < learnlist.size(); i++) {
//            qDebug() << learnlist[i].getPriority() << " | " << learnlist[i].getName();
//        }
        //PROCESS SCHEDULE FILE
        QString filename2 = (ui->lineEdit_csvPathForSchedule->text());
//        qDebug() << "Your availablity file is " << filename2;
        QFile calfile(filename2);
        if (!calfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << calfile.errorString();
        }
        QString AllCalData = calfile.readAll();
        //    qDebug() << AllCalData;
        if (calfile.isOpen()) calfile.close();

        int freedays = AllCalData.count("FREE");     //Find the number of free days
//        qDebug() << "Free Days: " << freedays;
//        qDebug() << "Size of LearnFile: " << learnlist.size();
        while (learnlist.size() > freedays) {   //If there are less free days than there are learnitems, cut some least important learn items
            learnlist.pop_back();
        }
//        qDebug() << "Size of LearnFile after sizing to Free Days: " << learnlist.size();
//        qDebug() << "Randomizing LearnItems onto your available time slots.";
        for (int i = 0; i < learnlist.size(); i++) {
            int randomarea = rand() % freedays + 1;
            //      qDebug() << "Random Area: " << randomarea;
            int currentpos = 0;
            for (int j = 0; j < randomarea; j++) {
                if (currentpos == 0) currentpos = AllCalData.indexOf("FREE");
                else currentpos = AllCalData.indexOf("FREE", currentpos+4);
            }
            AllCalData.replace(currentpos, 4, learnlist[i].getName());
            freedays--;
        }
        // qDebug() << AllCalData;

        do {
            if (freedays > 0) {
                int tmp = (learnlist.size() / 2);  //take the remaining and assign the top half again to any free space
                for (int i = 0; i < tmp; i++) {
                    learnlist.pop_back();
                }
//                qDebug() << "  ->>Top half of learn list:";
//                for (int i = 0; i < learnlist.size(); i++) {
//                    qDebug() << learnlist[i].getPriority() << " | " << learnlist[i].getName();
//                }
                if (learnlist.size() > freedays) {  //If there are less free days than there are learnitems, cut some least important learn items
                    for (int i = 0; i < freedays; i++) {
                        learnlist.pop_back();
                    }
                }
                for (int i = 0; i < learnlist.size(); i++) {
                    int randomarea = rand() % freedays + 1;
                    //    qDebug() << "Random Area: " << randomarea;
                    int currentpos = 0;
                    for (int j = 0; j < randomarea; j++) {
                        if (currentpos == 0) currentpos = AllCalData.indexOf("FREE");
                        else currentpos = AllCalData.indexOf("FREE", currentpos+4);
                    }
                    AllCalData.replace(currentpos, 4, learnlist[i].getName());
                    freedays--;
                }
            }
            //  qDebug() << AllCalData;
        } while (freedays > 0);   //repeat till none left
        //we will now have a filled out availability map
        FullCalendar = AllCalData;
//        ui->textBrowser_InfoAboutSchedule->setText(FullCalendar);   /item removed
        if(FullCalendar.size() > 10) {
            QMessageBox success;
            success.setText("Okay looks like the file processed just fine.");
            success.setWindowTitle("Good To Go");
            success.exec();
            learnlist.clear();
        }
    }
//    qDebug() << "Completed making the schedule.";
}

//void InputWindow::on_button_MakeSchedule_released()
//{
//    if (FullCalendar.size() < 20) {
//        QMessageBox process;
//        process.setText("Be sure to process your data first.");
//        process.setWindowTitle("Wait");
//        process.exec();
//        return;
//    } else {
//        QString NewSchedule = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
//                                                           tr("CSV File (*.csv)"));
//        QFile CSVFile(NewSchedule);
//        if(!CSVFile.open(QIODevice::WriteOnly)) {
//            QMessageBox::critical(this, tr("Unfinished business"), tr("*csv file has not been saved."));
//            return;
//        }
//        QTextStream out(&CSVFile);
//        out << FullCalendar;
//        qDebug() << "Wrote to " << NewSchedule;
//        CSVFile.close();
//        QMessageBox success;
//        success.setText("File has been written to your chosen location.");
//        success.setWindowTitle("Good To Go");
//        success.exec();
//    }

//}

void InputWindow::on_button_MakeiCalFile_released()
{
    //  format of the times:  yyyyMMddTHHmmss
    if (FullCalendar.size() < 20) {
        QMessageBox process;
        process.setText("Be sure to process your data first.");
        process.setWindowTitle("Wait");
        process.exec();
        return;
    } else if (startingMonday.isNull()){
        QMessageBox process;
        process.setText("Be sure to select a date first.");
        process.setWindowTitle("Wait");
        process.exec();
        return;
    } else {
        QVector<QDateTime> StartTimeOfItem;
        QVector<QString> SummaryOfItem;
        QDate tmpdatesetter;
        QTime tmptimesetter(0, 0, 0);
        QTime defaultTime(0, 0, 0);
        QString tmpLearnItemTitle;
        for (int i = 1; i <= 24; i++) {   //go through 24 hours
            for (int DayOfWeek = 0; DayOfWeek < 7; DayOfWeek++) {  //go through 7 days a week per hour block
                //for each day of the week at this time slot check if anything is scheduled
                // 0 = Monday here
                tmpLearnItemTitle = FullCalendar.section(',', (8 + ((i-1)*7) + DayOfWeek), (8 + ((i-1)*7) + DayOfWeek));
                if (!tmpLearnItemTitle.contains("BUSY")) {
                    tmpdatesetter = startingMonday.addDays(DayOfWeek); //startingMonday must be correct for this to work.
                    switch (i) { //set time that item starts
                    case 1: tmptimesetter = defaultTime;
                        break;
                    default:
                        tmptimesetter = defaultTime;
                        tmptimesetter = tmptimesetter.addSecs(60*60*(i-1));
                    }
                    if (DayOfWeek == 6) {  //to correct the sunday grabbing dates problem
                        int removestart = tmpLearnItemTitle.lastIndexOf('\n');
                        tmpLearnItemTitle.remove(removestart, 5);
                    }
                    SummaryOfItem.push_back(tmpLearnItemTitle);
                    QDateTime tmpStartTime;
                    tmpStartTime.setDate(tmpdatesetter);
                    tmpStartTime.setTime(tmptimesetter);
                    StartTimeOfItem.push_back(tmpStartTime);
                }
            }
        }
        //This part creates the actual openable file for Outlook or iCalendar
        QString NewScheduleFileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), tr("ics File (*.ics)"));
        QFile iCalFile(NewScheduleFileName);
        if (!iCalFile.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, tr("Unfinished business"), tr(".ics file has not been saved."));
            return;
        }
        QTextStream out(&iCalFile);
        //default header for ics file - testing currently
        out << "BEGIN:VCALENDAR\n"
            << "PRODID:-//Created-By-OverDrive-Personal-Learning-Calendar\n"
            << "VERSION:2.0\n"
            << "METHOD:PUBLISH\n";
        //out << "X-MS-OLK-FORCEINSPECTOROPEN:TRUE\n";  //this is for single item files only
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

        //The Events Part - Loop for each event to schedule //duration 1hr for now
        for (int i = 0, j = SummaryOfItem.size(); i < j; i++) {
            out << "BEGIN:VEVENT\n"
                << "CLASS:PUBLIC\n";
            QDateTime utcStamp(QDateTime::currentDateTimeUtc());
            out << "CREATED:" << utcStamp.toString("yyyyMMddTHHmmsszzzZ") << "\n"
                   //<< "DESCRIPTION:" /* << optional description*/ << "\n"     //optional
                   //<< "DTEND;TZID=" /* << learn-item date-time end*/ << "\n"   //optional (we use DURATION: instead)
                << "DTSTAMP:" << utcStamp.toString("yyyyMMddTHHmmsszzzZ") << "\n"
                << "DTSTART;TZID=" << timezone << ":" << StartTimeOfItem[i].toString("yyyyMMddTHHmmss") << "\n"
                << "DURATION:PT1H0M0S\n"
                << "LAST-MODIFIED:" << utcStamp.toString("yyyyMMddTHHmmsszzzZ") << "\n"
                << "PRIORITY:0\n"
                << "SEQUENCE:0\n"
                << "SUMMARY;LANGUAGE=en-us:" << SummaryOfItem[i] << "\n"
                << "TRANSP:TRANSPARENT\n";
            out << "UID:" << utcStamp.toString("yyyyMMddTHHmmsszzzZ") << "@" << QHostInfo::localHostName() << "\n";

            //Alert type and setup - must be part of VEVENT so make part of loop 10 min popup for now
            out << "BEGIN:VALARM\n"
                << "TRIGGER:-PT10M\n"
                << "ACTION:DISPLAY\n"
                << "END:VALARM\n"
                << "END:VEVENT\n";
        }
        out << "END:VCALENDAR\n";       //finish out calendar file


   //     qDebug() << "Wrote to " << NewScheduleFileName;
        iCalFile.close();
        QMessageBox success;
        success.setText("Test File has been written to your chosen location.");
        success.setWindowTitle("Good To Go");
        success.exec();
    }
}

void InputWindow::on_calendarWidget_DateOfStart_selectionChanged()
{
    //snap to Monday
    if (ui->calendarWidget_DateOfStart->selectedDate().dayOfWeek() != Qt::Monday) {
        ui->calendarWidget_DateOfStart->setSelectedDate(ui->calendarWidget_DateOfStart->selectedDate().addDays(-1));
    }
    if (ui->calendarWidget_DateOfStart->selectedDate().dayOfWeek() == Qt::Monday) {
        startingMonday = ui->calendarWidget_DateOfStart->selectedDate();
    }
}

void InputWindow::on_actionAbout_triggered()
{
    QMessageBox aboutWindow;
    aboutWindow.setText("<b>OverDrive Personal Learning Calendar r1</b><br>Written for personal use only by<br>  matthewjearly@gmail.com<br>");
    aboutWindow.setWindowTitle("About ODPLC");
    aboutWindow.exec();
}

void InputWindow::on_actionReset_triggered()
{
    learnlist.clear();
    FullCalendar.clear();
//    ui->textBrowser_InfoAboutSchedule->clear();   //item removed
    ui->lineEdit_csvPathForItems->setText("");
    ui->lineEdit_csvPathForSchedule->setText("");

    QMessageBox CompleteMessage;
    CompleteMessage.setText("Reset is complete.");
    CompleteMessage.setWindowTitle("Reset Complete");
    CompleteMessage.exec();
}

void InputWindow::on_pushButton_released()
{

    //open help window until closed
}
