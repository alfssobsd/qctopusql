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


#ifndef DOMAINS_H
#define DOMAINS_H

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
#include <ui_domains.h>
#include <QStringListModel>
#include <QStringList>

class Domains : public QWidget
{
  Q_OBJECT
  
public:
  
  Domains(QSqlDatabase db,QWidget *parent);
  ~Domains();
  QSqlDatabase db_psql;
  QStringListModel *model;
  void setTypeModel(QStringList list);
						 
private slots:
  void GetDomains();
  void Dialog_Delete_Domains();
  void Dialog_Add_Domains();
  void showContextMenu(const QPoint &point);  

private:
  Ui::WidgetDomains ui;
  QTableWidgetItem *__item0;
  QTableWidgetItem *__item1;
  void TestQuery();
  
signals:
  void DisconnectDB();
  
};


#endif
