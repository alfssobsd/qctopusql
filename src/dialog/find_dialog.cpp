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


#include <find_dialog.h>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegExp>
#include <QValidator>
#include <QPalette>
#include <QComboBox>
#include <QSpinBox>
#include <QString>
#include <QMenu>
#include <QColor>
#include <users_delete_dialog.h>
#include <users_edit_dialog.h>


FindDialog::FindDialog(QSqlDatabase db, QWidget *parent)
        : QDialog(parent) {
  
  db_psql=db;

  setupUi( this );


  connect(pushButton_Find, SIGNAL(clicked()), this, SLOT(Find()));
  connect(checkBox, SIGNAL(clicked(bool)), lineEdit_Domain, SLOT(setEnabled(bool)));
  connect(tableWidget_Find, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

  completer = new QCompleter(this);
  lineEdit_Domain->setCompleter(completer);
}

FindDialog::~FindDialog(){

  delete completer;
}

bool FindDialog::Empty_Test(){

  return true;

}

void FindDialog::Find(){

  QString StringFind;
  QString StringDomain;
  bool ok;
  QColor ColorItem(0, 0, 0);
  
  if( lineEdit_Domain->isEnabled() ){
	
	StringDomain.append("%");
	StringDomain.append(lineEdit_Domain->text());
	StringDomain.append("%");
  
  }else{
	
	StringDomain="%%";
  
  }

  QSqlQuery query( db_psql );
  
  query.exec("SELECT 1");
  query.clear();
  
  StringFind.append("%");
  StringFind.append(lineEdit_Find->text());
  StringFind.append("%");
  
  if( db_psql.isOpen() ){ 

	if(tableWidget_Find->isSortingEnabled()){
	  
	  tableWidget_Find->setSortingEnabled(false);

	}
	
	tableWidget_Find->clearContents();
	tableWidget_Find->setRowCount(0);
	
	if(comboBox->currentIndex() == 0){
	  
	  query.prepare("SELECT login,name,domain,status FROM users_view WHERE login ILIKE :find and domain ILIKE :domain");
	
	}else{
	  
	  query.prepare("SELECT login,name,domain,status FROM users_view WHERE domain ILIKE :domain and name ILIKE :find ");
	
	}
	
	query.bindValue(":find", StringFind);
	query.bindValue(":domain", StringDomain);
	
	if( !query.exec() ){
	  
	  QMessageBox::warning(this, tr("Query Error"),
						   query.lastError().text(),
						   QMessageBox::Ok);
	  query.clear();
	  
	}else{
	  
	  tableWidget_Find->setRowCount(query.size());
	  
	  for(int i = 0; i < query.size(); i++){
		
		query.next();
		
		if(query.value(3).toInt(&ok) == 1){

		  ColorItem.setRgb(198, 252, 155);

		}else{

		  ColorItem.setRgb(255, 202, 82);

		}
		
		__item0 = new QTableWidgetItem();
		__item0->setText(query.value(0).toString());
		__item0->setBackgroundColor(ColorItem);
		tableWidget_Find->setItem(i, 0, __item0);
		
		__item1 = new QTableWidgetItem();
		__item1->setText(query.value(1).toString());
		__item1->setBackgroundColor(ColorItem);
		tableWidget_Find->setItem(i, 1, __item1);
		
		__item2 = new QTableWidgetItem();
		__item2->setText(query.value(2).toString());
		__item2->setBackgroundColor(ColorItem);
		tableWidget_Find->setItem(i, 2, __item2);
		
	  }
	  
	}

	tableWidget_Find->setSortingEnabled(true);
	query.clear();
	
  }else{

	this->reject();
	
  }
  
}

void FindDialog::showContextMenu(const QPoint &point){
  
  tableWidget_Find->setCurrentItem(tableWidget_Find->itemAt(point));
  
  if(tableWidget_Find->indexAt(point).row() != -1){
	
	QMenu Pop_up;

  	connect(Pop_up.addAction(tr("Info User")), SIGNAL(triggered()), this, SLOT(DialogInfo()));
  	connect(Pop_up.addAction(tr("Delete User")), SIGNAL(triggered()), this, SLOT(DialogDelete()));
  	connect(Pop_up.addAction(tr("Edit User")), SIGNAL(triggered()), this, SLOT(DialogEdit()));
	
	Pop_up.exec(QCursor::pos());
  
  }
  
}

void FindDialog::DialogDelete(){

  QString Login = tableWidget_Find->item(tableWidget_Find->currentItem()->row(), 0)->text();
  QString Domain = tableWidget_Find->item(tableWidget_Find->currentItem()->row(), 2)->text();

  UsersDeleteDialog *DialogDelete;
  DialogDelete = new UsersDeleteDialog(db_psql, Login, Domain);
  DialogDelete->exec();

  delete DialogDelete;
  
  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	this->reject();
	
  }
  
}

void FindDialog::DialogEdit(){

  
  QString Login = tableWidget_Find->item(tableWidget_Find->currentItem()->row(), 0)->text();
  QString Domain = tableWidget_Find->item(tableWidget_Find->currentItem()->row(), 2)->text();

  UsersEditDialog *DialogEdit;
  DialogEdit = new UsersEditDialog(db_psql, Login, Domain, false);
  DialogEdit->exec();
  
  delete DialogEdit;
  
  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	this->reject();
	
  }


  
}
void FindDialog::DialogInfo(){

  QString Login = tableWidget_Find->item(tableWidget_Find->currentItem()->row(), 0)->text();
  QString Domain = tableWidget_Find->item(tableWidget_Find->currentItem()->row(), 2)->text();

  UsersEditDialog *DialogEdit;
  DialogEdit = new UsersEditDialog(db_psql, Login, Domain, true);
  DialogEdit->exec();
  
  delete DialogEdit;
  
  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	this->reject();
	
  }
  
}

void FindDialog::TestQuery(){

  QSqlQuery query( db_psql );
  
  query.exec("SELECT 1");
  query.clear();
   
}

void FindDialog::setCompleterModel(QAbstractItemModel *model){

  completer->setModel(model);
  
}
