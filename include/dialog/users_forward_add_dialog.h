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


#ifndef USERS_FORWARD_ADD_DIALOG_H
#define USERS_FORWARD_ADD_DIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QModelIndex>
#include "ui_users_forward_add_dialog.h"
#include <QCompleter>
#include <QAbstractItemModel>

class UsersForwardAddDialog : public QDialog, private Ui::DialogAddUsersForward {
    Q_OBJECT
 public:
  UsersForwardAddDialog(QSqlDatabase db,QWidget *parent=0);
  ~UsersForwardAddDialog();
  void setCompleterModel(QAbstractItemModel *model);													  
														  
 private slots:
  void Add();
  void NewRow();
  void DeleteRow();

 
 private:
  QModelIndex index;
  QSqlDatabase db_psql;
  bool Empty_Test();
  void TestQuery();
	  
};


#endif
