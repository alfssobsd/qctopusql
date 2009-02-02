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



#ifndef ALIASES_ADD_DIALOG_H
#define ALIASES_ADD_DIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QModelIndex>
#include "ui_aliases_add_dialog.h"


class AliasesAddDialog : public QDialog, private Ui::DialogAddAliases {
    Q_OBJECT
 public:
  AliasesAddDialog(QSqlDatabase db,QWidget *parent=0);
  ~AliasesAddDialog();
					 
 private slots:
  void Add();
  void NewRow();
  void DeleteRow();

 
 private:

  QRegExp alpha;
  QValidator *ValLocal_Part;
  QModelIndex index;
  QSqlDatabase db_psql;
  bool Empty_Test();
  void TestQuery();
  
};


#endif
