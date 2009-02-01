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



#ifndef FIND_DIALOG_H
#define FIND_DIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QAction>
#include "ui_find_dialog.h"
#include <QModelIndex>

class FindDialog : public QDialog, private Ui::DialogFind {
  Q_OBJECT
public:
  FindDialog (QSqlDatabase db, QWidget *parent=0);
							 
private slots:
  void Find();
  void showContextMenu(const QPoint &point);
  void DialogDelete();
  void DialogEdit();
  void DialogInfo();
  
private:
  QSqlDatabase db_psql;
  bool Empty_Test();
  QTableWidgetItem *__item0;
  QTableWidgetItem *__item1;
  QTableWidgetItem *__item2;
  void TestQuery();
  
};


#endif
