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



#ifndef USERS_FORWARD_H
#define USERS_FORWARD_H

#include <QtGui>
#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QTableWidget>
#include <QLineEdit>
#include <QModelIndex>
#include <QMessageBox>
#include <QTextCodec>
#include <QMenu>
#include <QAction>
#include <QVariant>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDialogButtonBox>
#include <QSqlError>
#include <QSqlQuery>
#include <ui_users_forward.h>

class UsersForward : public QWidget
{
  Q_OBJECT
  
public:
  
  UsersForward(QSqlDatabase db, QWidget *parent);
  QSqlDatabase db_psql;
					  
private slots:
  void GetUsersForward();
  void Dialog_Add_UsersForward();
  void Dialog_Delete_UsersForward();
  void Dialog_Edit_UsersForward();
  void Dialog_Find_UsersForward();
  void showContextMenu(const QPoint &point);
  
private:
  Ui::UsersForward ui;
  QTableWidgetItem *__item0;
  QTableWidgetItem *__item1;
  QTableWidgetItem *__item2;
  void TestQuery();
  
signals:
  void DisconnectDB();
  
};


#endif
