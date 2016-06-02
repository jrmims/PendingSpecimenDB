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
#include "inc/main.h"
#include <QApplication>
#include <QtCore>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // grab current directory
    QString dirPath = a.applicationDirPath();
    QString dbLocation = dirPath + QDir::separator() + dbName;

    MainWindow w;
    w.setDBLocation(dbLocation);
    w.setVersion(version);
    w.openDB();
    w.show();

    return a.exec();
}
