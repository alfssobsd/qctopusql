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



#ifndef ALIASES_FIND_DIALOG_H
#define ALIASES_FIND_DIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QAction>
#include "ui_aliases_find_dialog.h"
#include <QModelIndex>

class AliasesFindDialog : public QDialog, private Ui::AliasesDialogFind {
    Q_OBJECT
 public:
  AliasesFindDialog (QSqlDatabase db, QWidget *parent=0);
  QSqlDatabase db_psql;
  QString Local_Part;
  QString Domain;
				
 private slots:
  void Find();
  void showContextMenu(const QPoint &point);
  void DialogDelete();
  void DialogEdit();
 
 private:
  QTableWidgetItem *__item0;
  QTableWidgetItem *__item1;
  QTableWidgetItem *__item2;
  void TestQuery();
  
};



#endif
