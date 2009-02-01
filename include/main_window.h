/*
 * Copyright (C) 2008-2009  by Kravchuk Sergei V. (alfss@obsd.ru)
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



#ifndef MAIN_WONDOWS_H
#define MAIN_WINDOWS_H

#include <QtGui>
#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QTableWidget>
#include <QLineEdit>
#include <QTabWidget>
#include <QSpacerItem>
#include <ui_login.h>
#include <QSqlDatabase>
#include <users.h>
#include <aliases.h>
#include <users_forward.h>
#include <domains.h>

class MainWindow : public QWidget
{
  Q_OBJECT
  
public:
  
  MainWindow (QWidget *parent = 0);
  QSqlDatabase db;
 
private:
  Ui::Login ui;
  QTabWidget *Tab;
  User *UserWidget;
  Aliases *AliasesWidget;
  UsersForward *UsersForwardWidget;
  Domains *DomainsWidget;			  
  int GetAllDomains();
						
private slots:
  void About();
  void DialogProfiles();
  void ConnectDB();
  void DisconnectDB();
};

#endif
