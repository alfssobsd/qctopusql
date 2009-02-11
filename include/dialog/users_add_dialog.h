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



#ifndef USERS_ADD_DIALOG_H
#define USERS_ADD_DIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QCompleter>
#include <QAbstractItemModel>
#include "ui_users_add_dialog.h"

class UsersAddDialog : public QDialog, private Ui::DialogAdd {
  Q_OBJECT

public:
  UsersAddDialog(QSqlDatabase db,QWidget *parent=0);
  ~UsersAddDialog();
  void setCompleterModel(QAbstractItemModel *model);											   

private slots:
  void Add();
 
 private:
  QSqlDatabase db_psql;
  bool Empty_Test();
  QCompleter *completer;
  
//signals:
//  void DisconnectDB();

};

#endif
