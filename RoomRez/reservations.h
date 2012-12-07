#ifndef RESERVATIONS_H
#define RESERVATIONS_H

#include <QMainWindow>
#include <iostream>
#include <QDate>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QDebug>
#include <string.h>
#include <QStringList>
#include <qstd.h>
#include <QHash>
#include <utils_export.h>
#include <list>
using namespace std;
using namespace qstd;

namespace Ui {
class Reservations;
}

class Reservations : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Reservations(QWidget *parent = 0);
    ~Reservations();
    void addRoomRezEntry(QString name, QString date, QString time);
    void deleteRoomRezEntry(QString confirmationNumber);
    void printRoomRezEntries(QDate date);
    void printRoomRezEntries(QString name);
    int  generateConfNumber();
    void printRoomRezEntries();
    void printRoomRezHelp();
    void printRoomRezMan();
    void printRoomRezByDate(QString date);
    void printRoomRezByName(QString name);
    
private slots:


    void on_ReserveButton_clicked();

    void on_calendar_clicked(const QDate &date);



    void on_FromTime_timeChanged(const QTime &date);

    void on_ToTime_timeChanged(const QTime &date);

    void on_DeleteRez_clicked();

private:
    //file which the constructor reads and destructor writes to
    static QFile sm_fileData,building_file;
    //directs all output streamed into here, to stdout
    static QTextStream sm_cout;
    //keeps a list of all entries read from the sm_fileData
    QStringList	m_listReservationEntries;
    QHash < QString, QString>  Building;
    QDate DateInput;
    QTime TimeInputFrom,TimeInputTo;
    // line formater
    QString formatLine(QString nameDateHour);
    Ui::Reservations *ui;
};

#endif // RESERVATIONS_H
