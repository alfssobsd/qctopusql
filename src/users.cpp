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


#include <users.h>

#include <QApplication>
#include <QFile>
#include <QWidget>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDialog>
#include <QSqlError>
#include <users_edit_dialog.h>
#include <users_delete_dialog.h>
#include <QMessageBox>
#include <QColor>
#include <users_add_dialog.h>
#include <find_dialog.h>
#include <QCompleter>
#include <QStringList>
#include <QStringListModel>
#include <QProgressDialog>

User::User(QSqlDatabase db,QWidget *parent)
  : QWidget(parent)
{

  db_psql = db;
  
  ui.setupUi( this );

  UserTable = ui.UserTable;
  Button_Get = ui.pushButton_Get;
  QLineDomain = ui.lineEdit_Domain;

  //resize colum
  for(int i = 0; i < 5; i++){
	
	UserTable->setColumnWidth(i, 100);
  
  }
  
  QStringList wordList;
  wordList << "obsd.org" ;

  completerModel = new QStringListModel;
  completerModel->setStringList(wordList);

  completer = new QCompleter(this);
  completer->setModel(completerModel);

  QLineDomain->setCompleter(completer);

  connect(UserTable, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
  connect(Button_Get, SIGNAL(clicked()), this, SLOT(GetUser()));

  
}

void User::GetUser(){
  
  bool ok;
  QColor ColorItem(0, 0, 0);
  QSqlQuery query( db_psql);

  query.exec("SELECT 1");
  query.clear();

  /*freezing input Users Widget*/
  ui.pushButton_Get->setEnabled(false);
  ui.lineEdit_Domain->setEnabled(false);
  ui.UserTable->setEnabled(false);
  
  if( db_psql.isOpen() ){


	if(UserTable->isSortingEnabled()){
	  
	  UserTable->setSortingEnabled(false);

	}

	UserTable->clearContents();
	UserTable->setRowCount(0);
	
	query.prepare("SELECT login,domain,name,status,quota FROM users_view WHERE domain=:id_domain");
	query.bindValue(":id_domain", QLineDomain->text());
	
	if( query.exec() ){

	  UserTable->setRowCount(query.size());

	  QProgressDialog progress(tr("Getting a list of users."), tr("Cancel"),0,query.size(),this);
	  
	  for(int i = 0; i < query.size(); i++){
		
		query.next();

		progress.setValue(i);
		qApp->processEvents();
		if (progress.wasCanceled()){
		  UserTable->setRowCount(i);
		  break;
		}
		
		if(query.value(3).toInt(&ok) == 1){

		  ColorItem.setRgb(198, 252, 155);

		}else{

		  ColorItem.setRgb(255, 202, 82);

		}
		
		__item0 = new QTableWidgetItem();
		__item0->setText(query.value(0).toString());
		__item0->setBackgroundColor(ColorItem);
		UserTable->setItem(i, 0, __item0);
		
		__item1 = new QTableWidgetItem();
		__item1->setText(query.value(1).toString());
		__item1->setBackgroundColor(ColorItem);
		UserTable->setItem(i, 1, __item1);
		
		__item2 = new QTableWidgetItem();
		__item2->setText(query.value(2).toString());
		__item2->setBackgroundColor(ColorItem);
		UserTable->setItem(i, 2, __item2);
		
		__item3 = new QTableWidgetItem();
		__item3->setText(query.value(3).toString());
		__item3->setBackgroundColor(ColorItem);
		UserTable->setItem(i, 3, __item3);
		
		
		__item4 = new QTableWidgetItem();
		__item4->setText(query.value(4).toString());
		__item4->setBackgroundColor(ColorItem);
		UserTable->setItem(i, 4, __item4);
		
	  }
	  
	  UserTable->resizeColumnsToContents();
	  
	  for(int i = 0; i < 5; i++){
		
		if(UserTable->columnWidth(i) < 100){

		  UserTable->setColumnWidth(i, 100);

		}
		
	  }

	  UserTable->setSortingEnabled(true);
	  query.clear();

	}else{
	  
	  QMessageBox::warning(this, tr("Query Error"),
						   query.lastError().text(),
						   QMessageBox::Ok);
	
	}
  }else{

	emit DisconnectDB();
	
  }

  /*defrosting input Users Widget*/
  ui.pushButton_Get->setEnabled(true);
  ui.lineEdit_Domain->setEnabled(true);
  ui.UserTable->setEnabled(true);
  
}

void User::showContextMenu(const QPoint &point){


  UserTable->setCurrentItem(UserTable->itemAt(point));
  
  QMenu Pop_up;
  
  connect(Pop_up.addAction(tr("New User")), SIGNAL(triggered()), this, SLOT(Dialog_Add_User()));
  connect(Pop_up.addAction(tr("Find User")), SIGNAL(triggered()), this, SLOT(DialogFind()));
  
  if( UserTable->indexAt(point).row() != -1){

	connect(Pop_up.addAction(tr("Info User")), SIGNAL(triggered()), this, SLOT(Dialog_Info_Users()));
	connect(Pop_up.addAction(tr("Edit User")), SIGNAL(triggered()), this, SLOT(Dialog_Edit_Users()));
	connect(Pop_up.addAction(tr("Delete User")), SIGNAL(triggered()), this, SLOT(Dialog_Delete_Users()));

  }
  
  Pop_up.exec(QCursor::pos());
  
}


void User::Dialog_Add_User(){
  
  UsersAddDialog *AddDialog;
  AddDialog = new UsersAddDialog( db_psql );
  AddDialog->setCompleterModel( completerModel );
  AddDialog->exec();

  delete AddDialog;

  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();

  }

}

void User::DialogFind(){
 
  FindDialog *DialogFindUsers;
  DialogFindUsers = new FindDialog( db_psql );
  DialogFindUsers->setCompleterModel( completerModel );
  DialogFindUsers->exec();

  delete DialogFindUsers;

  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();

  }
  
}

void User::Dialog_Edit_Users(){

  QString Login = UserTable->item(UserTable->currentItem()->row(), 0)->text();
  QString Domain = UserTable->item(UserTable->currentItem()->row(), 1)->text();
  
  UsersEditDialog *DialogEdit;
  DialogEdit = new UsersEditDialog(db_psql, Login, Domain, false);
  DialogEdit->exec();

  delete DialogEdit;
  
  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();
	
  }
  
}

void User::Dialog_Delete_Users(){
 
  QString Login = UserTable->item(UserTable->currentItem()->row(), 0)->text();
  QString Domain = UserTable->item(UserTable->currentItem()->row(), 1)->text();
  
  UsersDeleteDialog *DialogDelete;
  DialogDelete = new UsersDeleteDialog(db_psql, Login, Domain);
  DialogDelete->exec();
  delete DialogDelete;
  
  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();
	
  }
  
}

void User::Dialog_Info_Users(){
 
  QString Login = UserTable->item(UserTable->currentItem()->row(), 0)->text();
  QString Domain = UserTable->item(UserTable->currentItem()->row(), 1)->text();

  UsersEditDialog *DialogEdit;
  DialogEdit = new UsersEditDialog(db_psql, Login, Domain, true);
  DialogEdit->exec();

  delete DialogEdit;
  
  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();
	
  }

}

void User::TestQuery(){

  QSqlQuery query( db_psql );
  
  query.exec("SELECT 1");
  query.clear();
  
}

void User::SetCompleterList(QStringList list){

  completerModel->setStringList(list);
  
}
