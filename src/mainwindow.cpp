/*
Pending Specimen DB
Copyright (C) 2016  James Mims

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "inc/mainwindow.h"
#include "ui_mainwindow.h"
#include "inc/main.h"
#include "inc/aboutwindow.h"
#include <QMessageBox>
#include <fstream>
#include <QtSql>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionAbout_Pending_Specimen_Database->connect(ui->actionAbout_Pending_Specimen_Database, SIGNAL(triggered(bool)), this, SLOT(openAboutWindow()) );
    ui->pushButton_addSpecimen->connect(ui->pushButton_addSpecimen, SIGNAL(clicked(bool)), this, SLOT(addItem()) );
    ui->pushButton_deleteSpecimen->connect(ui->pushButton_deleteSpecimen, SIGNAL(clicked(bool)), this, SLOT(deleteItem()) );
}

MainWindow::~MainWindow()
{
    delete ui;
    db.close();
    delete sqlTable;
}

void MainWindow::openDB()
{
    // convert QString to char[]
    QByteArray tempArray = dbLocation.toLocal8Bit();
    char * dbLocationChar = tempArray.data();
    if (!std::ifstream(dbLocationChar))
    {
        createDB();
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbLocation);
        db.open();
    }


    sqlTable = new QSqlTableModel(this);
    sqlTable->setTable("Pending");
    sqlTable->setEditStrategy(QSqlTableModel::OnFieldChange);
    sqlTable->select();

    ui->tableView->setModel(sqlTable);
    ui->tableView->show();
}

void MainWindow::setDBLocation( QString dbLocation )
{
   this->dbLocation = dbLocation;
}

void MainWindow::setVersion( QString version )
{
    this->versionString = version;
}

void MainWindow::createDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( dbLocation );
    db.open();
    QSqlQuery query(db.database());
    query.exec("CREATE TABLE Pending"
               " (Worksheet TEXT,"
               " Specimen TEXT,"
               " Machine TEXT,"
               " Employee TEXT,"
               " Date TEXT)");
    query.finish();
}

void MainWindow::openAboutWindow()
{
    AboutWindow aboutWindow;
    aboutWindow.setVersion(this->versionString);
    aboutWindow.exec();
}

void MainWindow::addItem()
{
    // Grab text from line edits
    QString worksheet = ui->lineEdit_worksheet->text();
    QString specimen = ui->lineEdit_specimen->text();
    QString initials = ui->lineEdit_initials->text();
    QString machine = ui->lineEdit_machine->text();

    if( worksheet.isEmpty() || specimen.isEmpty() || initials.isEmpty() || machine.isEmpty() )
    {
        QMessageBox msgBox;
        msgBox.setText( QString("You have not filled out all of the fields.") );
        msgBox.setWindowTitle( QString("Input Error") );
        msgBox.exec();
    }
    else // add to database
    {
        QSqlRecord record;
        record.append(QSqlField("Worksheet", QVariant::String));
        record.append(QSqlField("Specimen", QVariant::String));
        record.append(QSqlField("Machine", QVariant::String));
        record.append(QSqlField("Employee", QVariant::String));
        record.append(QSqlField("Date", QVariant::String));

        record.setValue(0, worksheet);
        record.setValue(1, specimen);
        record.setValue(2, machine);
        record.setValue(3, initials);

        QDateTime currDateTime = QDateTime::currentDateTime();
        QString currDateTimeStr = currDateTime.toString("MM-dd-yyyy hh:mm:ss AP");
        record.setValue(4, currDateTimeStr);

        sqlTable->insertRecord(-1, record); // insert new record

        // reset line edit boxes
        ui->lineEdit_initials->clear();
        ui->lineEdit_machine->clear();
        ui->lineEdit_specimen->clear();
        ui->lineEdit_worksheet->clear();

    }

}

void MainWindow::deleteItem()
{
    QModelIndexList indexList = ui->tableView->selectionModel()->selectedRows();
    if ( indexList.empty() )
    {
        QMessageBox msgBox;
        msgBox.setText( QString("Select a specimen from the database") );
        msgBox.setWindowTitle( QString("No Specimen Selected") );
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        int reply = msgBox.question(this, QString("Are you sure?"), QString("Are you sure you want to delete this specimen?"), QMessageBox::No, QMessageBox::Yes);
        if (reply == QMessageBox::Yes)
        {
            for( int i = 0; i < indexList.count(); i++ )
            {
                QModelIndex index = indexList.at(i);
                sqlTable->removeRow(index.row()); // delete row
                sqlTable->select(); // update db
            }
        }
        else
        {
            ui->tableView->clearSelection();
        }
    }
}

