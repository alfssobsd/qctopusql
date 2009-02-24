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



#ifndef DOMAINS_ADD_DIALOG_H
#define DOMAINS_ADD_DIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QStringListModel>
#include <QStringList>
#include "ui_domains_add_dialog.h"


class DomainsAddDialog : public QDialog, private Ui::DomainsAddDialog {
  Q_OBJECT
public:
  DomainsAddDialog (QSqlDatabase db, QStringListModel *model, QString domain, QString type);
																  
private slots:
  void New();
  void Deleted();
  
private:
  QSqlDatabase db_psql;
  void TestQuery();
  
};


#endif
