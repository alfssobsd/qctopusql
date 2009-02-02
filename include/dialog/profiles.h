/*
 * Copyright (C) 2008  by Kravchuk Sergei V. (alfss@obsd.ru)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */



#ifndef PROFILES_H
#define PROFILES_H

#include <QDialog>
#include <QDomDocument>
#include <QFile>
#include <QList>
#include <QTextStream>
#include <xmlprofiles.h>
#include <QDir>

#include <ui_profiles.h>


class Profiles : public QDialog, private Ui::ProfilesDialog {

  Q_OBJECT

public:

  Profiles(QLineEdit *host, QLineEdit *port, QLineEdit *db, QLineEdit *login, QLineEdit *Psw,QWidget *parent=0);
  ~Profiles();
  QLineEdit *QLineHost;
  QLineEdit *QLinePort;
  QLineEdit *QLineLogin;
  QLineEdit *QLinePsw;
  QLineEdit *QLineDB;
  QLineEdit *QLineDomain;
  
//private slots:					


//public slots:
private:
 
  XmlProfiles *config;
  QFile file;
  QString ConfigFile;
  
private slots:
  void LoadProfile(QListWidgetItem * item);										  
  void SaveProfile();
  void NewProfile();
  void DeleteProfile();
  void AcceptProfile();

  //QDomDocument doc;
  //QDomElement root;
  //QDomElement child;
  
};


#endif
