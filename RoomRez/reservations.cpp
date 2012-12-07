#include "reservations.h"
#include "ui_reservations.h"

void QstringToTime(QString time, QTime &from , QTime &to );

QFile Reservations::sm_fileData("reservation.dat");
QTextStream Reservations::sm_cout(stdout);

QFile Reservations::building_file("Buildings.dat");
Reservations::Reservations(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Reservations)
{
    ui->setupUi(this);

    sm_fileData.open(QIODevice::ReadOnly);

    QTextStream readFile(&sm_fileData);

    // read line by line and insert into a QStringList
    // from QFile documentation
    while (!readFile.atEnd())
    {
            m_listReservationEntries << readFile.readLine();
    }

    sm_fileData.close();

    // read buildings
    building_file.open(QIODevice::ReadOnly);

    QTextStream readFile1(&building_file);

    // read line by line and insert into a QStringList
    // from QFile documentation
    QList  <QString> line;
    while (!readFile1.atEnd())
    {
       QString abbrev, desc;

       line.clear();
       line = readFile1.readLine().split('\t');
       abbrev = line[0];
       desc = line[1];
       ui->Buildings->addItem(line[0]);
       Building.insert(line[0],line[1]);
    }

    building_file.close();
    for(int i =1; i<10;i++){
        ui->RoomNumber->addItem(QString::number(i));
    }

}

Reservations::~Reservations()
{
    sm_fileData.open(QIODevice::WriteOnly);

    QTextStream writeFile(&sm_fileData);

    // Write every line in the QStringList to the file sm_fileData by
    // reading and popping the top of the list each time.
    while ( !m_listReservationEntries.isEmpty() )
    {
            writeFile << m_listReservationEntries.front() << endl;
            m_listReservationEntries.pop_front();
    }

    sm_fileData.close();

    delete ui;
}

void Reservations::addRoomRezEntry(QString name, QString date, QString time)
{
    // define variables
    QTime itemFrom, itemTo , newFrom, newTo;
    // check for time error
    QstringToTime(time, newFrom, newTo);
    if(newFrom>newTo){
        cout<<"Starting time cannot be bigger than ending time, try again!"<<endl;
        return;
    }


    // convert the QString date into a QDate
    QDate newDate = QDate::fromString(date, "MM/dd/yy");
    // Invalid input checking
    // If the date entered is of valid year/month/date
    if (!newDate.isValid())
    {
            sm_cout << "The date " << date << " is invalid" << endl;
            return;
    }

    if (!newFrom.isValid()||!newTo.isValid())
    {
            sm_cout << "The Time your entered is invalid"<< endl;
            return;
    }
    if(newFrom==newTo){
        sm_cout << "The Time your entered is invalid"<< endl;
        return;
    }
    QDate CurrDate = QDate::currentDate();
    //Check to see if date entered is the year 2012 or 2013
     if (newDate.addYears(100)>CurrDate.addYears(1))//||(newDate < newDate.currentDate()))    // cannot reserve in the past
    {
            sm_cout << "The date has to be no more than 1 year in the future" << endl;
            cout <<"The Current date is: "<<QDate::currentDate().toString().toStdString()<<endl;
            cout<<"Try Again!"<<endl;
            ui->Console->setText("The date has to be no more than 1 year in the future");
            return;
    }
     if(newDate.addYears(100)<CurrDate){

         sm_cout << "You cannot book a room in the past" << endl;
         ui->Console->setText("You cannot book a room in the past");
         return;
     }

     if(newFrom.operator <(QTime::currentTime())&&newDate.addYears(100)==CurrDate){
         cout<<"You cannot reserve a room in the past!"<<endl;
         return;

     }
    int posInsert = m_listReservationEntries.size();

    // Loop through each element, keeping track of an index variable
    // Compares names and inserts the new reservation in chronological order
    for ( int i = 0; i < m_listReservationEntries.size(); i++)
    {
            // get the name, date, time, conf
            QString item = m_listReservationEntries[i];
            //extract just the name
            QString tempName = item.split("===")[0];
            QString tempDate = item.split("===")[1];
            QString tempTime = item.split("===")[2];
            QDate itemDate = QDate::fromString(tempDate, "MM/dd/yy");
            // declare needed parameters


            QstringToTime(tempTime , itemFrom , itemTo);
            QstringToTime(time, newFrom, newTo);


            // check date

            if ( newDate>itemDate ) // compare dates
                {
                        posInsert = i;
                        break;
                }


            // check if the same name is already on the list
            if ( !(QString::compare(name, tempName , Qt::CaseInsensitive) )) // compare names
            {
                //sameNameFound = true;
                // if same name is found, check the date

                try{
                if(newDate==itemDate){

                    if(((itemFrom<=newFrom)&&(itemTo>=newTo))||((itemFrom>=newFrom)&&(itemFrom<=newTo))||
                            ((itemTo>=newFrom)&&(newTo>=itemTo))||((itemFrom==newFrom)&&(newTo==itemTo))||
                            ((itemFrom>newFrom)&&(itemTo<newTo))){
                        throw 1;

                    }
                    if(newFrom>itemTo){
                        posInsert = i;
                       break;
                    }
                    //else{
                      //  posInsert = i;      // specify location
                        //break;
                    //}
                }

             }

                catch (int e){
                        // room is already booked
                        cout<<"         ***********Time conflict***********"<<endl;
                        cout<<"Room: "<< name.toStdString()<<" is booked on: "<<tempDate.toStdString()<<endl;
                        cout<<"     from: "<<itemFrom.toString().toStdString()<<" to: "<<itemTo.toString().toStdString()<<endl;
                        cout<<"         ***********Please choose another date, or time slot!***********"<<endl;
                        ui->Console->setText("already booked");
                        return;
                    }

                }
            }





    // generate confirmation number
    int ConfirmationNumber = generateConfNumber();
    // insert new calendar entry with correct index position
    QString ConfNum = QString::number(ConfirmationNumber);
    QString newReservationEntry = name + "===" +date+"==="+time+"==="+ConfNum;
    m_listReservationEntries.insert(posInsert, newReservationEntry);        // inserts in ascending order by date
    ui->Console->clear();
    QString msg = "Room :" + name + " has been booked." + " Your confirmation number is: "+QString::number(ConfirmationNumber);
    ui->Console->setText(msg);
    sm_cout << "Room :" << name << " has been booked." << " Your confirmation number is: "<<ConfirmationNumber<< endl;
}



/*	This function takes in either a confirmation number
        and removes the matching entry in the string list.
*/
void Reservations::deleteRoomRezEntry(QString confirmationNumber)
{


        //Loop through each element to see if the confirmation number equals
        //any of the entries and delete entry that matches
        for (int i=0;i<m_listReservationEntries.size();i++)
        {
                QString item = m_listReservationEntries[i];
                QString Conf = item.split("===")[3];
                if(Conf.toInt() == confirmationNumber.toInt()){
                    // found a matching reservation number
                    m_listReservationEntries.removeAt(i);       // remove reservation
                    cout<<"*******Reservation has been removed******"<<endl;
                    ui->Console->setText("*******Reservation has been removed******");
                    return;
                }
        }

        // if not found
        cout<<"Reservation: "<< confirmationNumber.toStdString()<<" was not found!"<<endl;
        ui->Console->setText("******Reservation: "+confirmationNumber+"was not found!******");

}

/*	This function takes in a date from the command line.
        It should print out entries.
*/
void Reservations::printRoomRezEntries(QString dateString)
{
        cout << "printCalendarEntry is not implemented yet..." << endl;
        cout << "should print out entries for date " << dateString.toStdString() << endl;
}

void Reservations::printRoomRezEntries()
{
    if(m_listReservationEntries.empty())
    {
        cout<<"No reservations has been made, list is empty!"<<endl;
        return;
    }
        foreach (QString str, m_listReservationEntries)
        {
                cout << formatLine(str).toStdString() << endl;
        }
}
//help menu
void Reservations::printRoomRezHelp(){
    cout<<"         Help Menu"<<endl;
    cout<<"program name: roomRez"<<endl<<endl;
    cout<<"     -man  , prints program manual"<<endl<<endl;
    cout<<"     -help , prints help menu"<<endl<<endl;
    cout<<"     -add -r name -d mm/dd/yy -t hh:mm-hh:mm  , adds reservation"<<endl<<endl;
    cout<<"     -del conf# , delets specified reservation"<<endl<<endl;
    cout<<".    -List -d mm/dd/yy , prints reservations on that date"<<endl<<endl;
    cout<<"     -List -r name , prints reservations under that name"<<endl<<endl;
    cout<<"     - no arguments ,prints reservation list"<<endl<<endl;
}




// manual
void Reservations::printRoomRezMan()
{

    cout<<"                     HELP MENU"<<endl<<endl<<endl;
    cout<<"NAME"<<endl;
    cout<<"         ./roomRez"<<endl<<endl;
    cout<<"DESCRIPTION"<<endl;
    cout<<"        ./roomRez [OPTIONS] [-r] [RoomName] [-d] [mm/dd/yy] [-t] [hh:mm-hh:mm]"<<endl<<endl;
    cout<<"         This is a room reservation program. Passing no arguments"<<endl;
    cout<<"         will list the reservations"<<endl;
    cout<<"         the required fields -name,date,time- has to be in double quotes."<<endl;
    cout<<"         e.g, ./roomRez -add -r <RoomName> -d <11/11/12> -t <12:10-3:20> "<<endl;
    cout<<"         this example will book a room on that date at that time "<<endl;
    cout<<"         and returns a confirmation numer"<<endl<<endl;
    cout<<"OPTIONS"<<endl;
    cout<<"         -add , adds a room reservation"<<endl<<endl;
    cout<<"         e.g, ./roomRez -add -r <RoomName> -d <11/11/12> -t <12:10-3:20> "<<endl;
    cout<<"         where <field> represents double qoutes"<<endl;
    cout<<"         this example will book a room on that date at that time"<<endl;
    cout<<"         and returns a confirmation number"<<endl<<endl;
    cout<<"         -del , this will delete the room reservation "<<endl<<endl;
    cout<<"         if the confirmation number specified is valid"<<endl;
    cout<<"         e.g, ./roomRez -del confirmationNumber "<<endl;
    cout<<"         this should delete that reservation"<<endl<<endl;
    cout<<"         -List:"<<endl<<endl;
    cout<<"                 -d , will list reservations by date"<<endl<<endl;
    cout<<"                 -r , will list reservations by name"<<endl<<endl;
    cout<<"         e.g, ./roomRez -List -d <mm/dd/yy> "<<endl;
    cout<<"         where <field> represents double qoutes"<<endl<<endl;
    cout<<"         -no arguments, this will list all the reservations that has been made"<<endl<<endl;
    cout<<"         e.g, ./roomRez"<<endl<<endl;
    cout<<"AUTHOR"<<endl;
    cout<<"         Written by Zeid Ayssa"<<endl<<endl;
    cout<<"REPORTING BUGS"<<endl;
    cout<<"         Report ./roomRez bugs to azeid@vt.edu"<<endl<<endl;
}



void Reservations::printRoomRezByDate(QString date)
{

        bool found=false;

        QDate newDate = QDate::fromString(date, "MM/dd/yy");
        if(!newDate.isValid()){

            cout<<"Invalid Date"<<endl;
            return;
        }
        //Loop through each element
        for (int i=0;i<m_listReservationEntries.size();i++)
        {
                QString item = m_listReservationEntries[i];
                QString strDate = item.split("===")[1];
                QDate itemDate = QDate::fromString(strDate, "MM/dd/yy");


                if(newDate == itemDate){
                    // found a matching dates
                    found=true;
                    QString currentName = item.split("===")[0];
                    QString currentDate = item.split("===")[1];
                    QString currentTime = item.split("===")[2];
                    QString currentConf = item.split("===")[3];
                    QTime timeFrom,timeTo;
                    QstringToTime(currentTime,timeFrom,timeTo);
                    cout<<currentConf.toStdString()<<": "<<currentName.toStdString();
                    cout<<" is reserved from "<<timeFrom.toString().toStdString()<<" to "<<timeTo.toString().toStdString();
                    cout<<" on "<<currentDate.toStdString()<<endl;

                }
        }

        if(!found){
            cout<<"There are no room reservations on: "<<date.toStdString()<<endl;
        }

}

void Reservations::printRoomRezByName(QString name)
{

        bool found=false;
        //Loop through each element
        for (int i=0;i<m_listReservationEntries.size();i++)
        {
                QString item = m_listReservationEntries[i];
                QString newName = item.split("===")[0];

                if(!(QString::compare(newName,name,Qt::CaseInsensitive))){
                    // found a matching dates
                    found=true;
                    QString currentName = item.split("===")[0];
                    QString currentDate = item.split("===")[1];
                    QString currentTime = item.split("===")[2];
                    QString currentConf = item.split("===")[3];
                    QTime timeFrom,timeTo;
                    QstringToTime(currentTime,timeFrom,timeTo);
                    cout<<currentConf.toStdString()<<": "<<currentName.toStdString();
                    cout<<" is reserved from "<<timeFrom.toString().toStdString()<<" to "<<timeTo.toString().toStdString();
                    cout<<" on "<<currentDate.toStdString()<<endl;
                }
        }

        if(!found){
            cout<<"There are no room reservations under: "<<name.toStdString()<<endl;
        }

}QString Reservations::formatLine(QString nameDateTimeConf)
{
        QStringList element = nameDateTimeConf.split("===");
        QString name = element[0]; // Extract name
        QString date = element[1]; // Extract date
        QString time = element[2]; // Extract time
        QString conf = element[3]; // Extract confirmation number
        QString timeFrom = time.split("-")[0];
        QString timeTo = time.split("-")[1];

        QString output;
        QTextStream os(&output);

        os << name << " is booked at " << date
           << " from " << timeFrom<<" to "<<timeTo<<" conf: "<<conf;

        return output;
}

// Helper functions
// splits the time to needed formats
void QstringToTime(QString time, QTime &from , QTime &to ){

    QString tempFrom = time.split("-")[0];
    QString tempTo = time.split("-")[1];
    from = QTime::fromString(tempFrom, "hh:mm");
    to = QTime::fromString(tempTo, "hh:mm");

}

// generate reservation number
int Reservations::generateConfNumber(){

    int conf;
    conf = rand() % 123456 + 100000 ;       // generate random numbers from 100000 to 123456
    int iterator=0;
    int currentConf;
    /* TODO: check if rand() returns a confirmation number that has been assigned */
    for ( iterator = 0; iterator < m_listReservationEntries.size(); iterator++)
    {
        // get the name, date, time, conf
        QString item = m_listReservationEntries[iterator];
        // check if the confirmation number has been assigned
        QString tempConf = item.split("===")[3];
        currentConf = tempConf.toInt();
        if(currentConf == conf)
        {
            // generate another number and check again
            conf = 100000%((rand() % 123456 + 1000000)/QTime::currentTime().msec()) ;       // generate random numbers from 100000 to 123456
            iterator=0;    // reset the for loop to check again
        }

    }

    //if the function is over, then I know that I have generated a unique confirmation number

    return conf;

}



void Reservations::on_ReserveButton_clicked()
{
    if(TimeInputFrom.isNull()||TimeInputTo.isNull()||DateInput.isNull()){
         ui->Console->setText("Invalid Date or Time");
         qDebug()<<TimeInputFrom;
         qDebug()<<TimeInputTo;
         qDebug()<<DateInput;
    }
    else{

        QString name;
        name = ui->Buildings->currentText()+" "+ ui->RoomNumber->currentText();
        QString dateformat = "MM/dd/yy";
        QString date =  DateInput.toString(dateformat);

        QString timeformat = "hh:mm";
        QString timefrom = TimeInputFrom.toString(timeformat);
        QString timeto = TimeInputTo.toString(timeformat);
        QString time = timefrom+"-"+timeto;
        //ui->Console->setText(time);
        addRoomRezEntry(name,date,time);
    }

}

void Reservations::on_calendar_clicked(const QDate &date)
{
    DateInput = date;
}



void Reservations::on_FromTime_timeChanged(const QTime &date)
{
    TimeInputFrom = date;
}

void Reservations::on_ToTime_timeChanged(const QTime &date)
{
    TimeInputTo = date;
}

void Reservations::on_DeleteRez_clicked()
{
    QString conf = ui->ConfirmationNumber->text();
    deleteRoomRezEntry(conf);
}
