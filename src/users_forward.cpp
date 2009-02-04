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


#include <users_forward.h>
#include <users_forward_add_dialog.h>
#include <users_forward_delete_dialog.h>
#include <users_forward_edit_dialog.h>
#include <users_forward_find_dialog.h>

UsersForward::UsersForward(QSqlDatabase db, QWidget *parent)
  : QWidget(parent){
  
  db_psql = db;
  
  ui.setupUi( this );
  
  connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(GetUsersForward()));
  connect(ui.tableWidget_UsersForward, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
  
  for(int i = 0; i < 3; i++){
	
	ui.tableWidget_UsersForward->setColumnWidth(i, 100);
  
  }

  QStringList wordList;
  wordList << "obsd.org" ;
  
  completerModel = new QStringListModel;
  completerModel->setStringList(wordList);
  
  completer = new QCompleter(this);
  completer->setModel(completerModel);

  ui.lineEdit_Domain->setCompleter(completer);
  
}

void UsersForward::GetUsersForward(){

  /*freezing input Users Forward Widget*/
  ui.pushButton->setEnabled(false);
  ui.lineEdit_Domain->setEnabled(false);
  ui.tableWidget_UsersForward->setEnabled(false);
  
  TestQuery();
  
  if( db_psql.isOpen() ){ 

	if(ui.tableWidget_UsersForward->isSortingEnabled()){
	  
	  ui.tableWidget_UsersForward->setSortingEnabled(false);

	}
	
	ui.tableWidget_UsersForward->clearContents();
	ui.tableWidget_UsersForward->setRowCount(0);
	
	QSqlQuery query( db_psql );
	
	query.prepare("SELECT local_part,domain,recipients  FROM userforward_view WHERE domain=:id_domain");
	query.bindValue(":id_domain", ui.lineEdit_Domain->text());
	
	if( query.exec() ){

	  ui.tableWidget_UsersForward->setRowCount(query.size());
	  QProgressDialog progress(tr("Getting a list of users forward."), tr("Cancel"),0,query.size(),this);
	  
	  for(int i = 0; i < query.size(); i++){
		
		query.next();

		progress.setValue(i);
		qApp->processEvents();
		if (progress.wasCanceled()){
		  ui.tableWidget_UsersForward->setRowCount(i);
		  break;
		}

		__item0 = new QTableWidgetItem();
		__item0->setText(query.value(0).toString());
		ui.tableWidget_UsersForward->setItem(i, 0, __item0);
		
		__item1 = new QTableWidgetItem();
		__item1->setText(query.value(1).toString());
		ui.tableWidget_UsersForward->setItem(i, 1, __item1);
		
		__item2 = new QTableWidgetItem();
		__item2->setText(query.value(2).toString());
		ui.tableWidget_UsersForward->setItem(i, 2, __item2);
		

	  }
	  
	  ui.tableWidget_UsersForward->resizeColumnsToContents();
	  
	  for(int i = 0; i < 3; i++){
		
		if(ui.tableWidget_UsersForward->columnWidth(i) < 100){
		
		  ui.tableWidget_UsersForward->setColumnWidth(i, 100);
		
		}
	  
	  }

	  ui.tableWidget_UsersForward->setSortingEnabled(true);
	  
	  query.clear();
	  
	}else{
	  
	  QMessageBox::warning(this, tr("Query Error"),
						   query.lastError().text(),
						   QMessageBox::Ok);
	  query.clear();
	
	}
	
  }else{

	emit DisconnectDB();
	
  }
  
  /*defrosting input Users ForwardWidget*/
  ui.pushButton->setEnabled(true);
  ui.tableWidget_UsersForward->setEnabled(true);
  ui.lineEdit_Domain->setEnabled(true); 
}


void UsersForward::showContextMenu(const QPoint &point){

  ui.tableWidget_UsersForward->setCurrentItem(ui.tableWidget_UsersForward->itemAt(point));
  
  QMenu Pop_up;
  
  connect(Pop_up.addAction(tr("New Users Forward")), SIGNAL(triggered()), this, SLOT(Dialog_Add_UsersForward()));
  connect(Pop_up.addAction(tr("Find Users Forward")), SIGNAL(triggered()), this, SLOT(Dialog_Find_UsersForward()));
  
  if(ui.tableWidget_UsersForward->indexAt(point).row() != -1){
	
	connect(Pop_up.addAction(tr("Delete Users Forward")), SIGNAL(triggered()), this, SLOT(Dialog_Delete_UsersForward()));
	connect(Pop_up.addAction(tr("Edit Users Forward")), SIGNAL(triggered()), this, SLOT(Dialog_Edit_UsersForward()));

  }
  
  Pop_up.exec(QCursor::pos());

}

void UsersForward::Dialog_Add_UsersForward(){
  
  UsersForwardAddDialog *DialogAdd;
  DialogAdd = new UsersForwardAddDialog( db_psql );
  DialogAdd->exec();
  delete DialogAdd;

  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();
	
  }
  
}

void UsersForward::Dialog_Delete_UsersForward(){

  QString Local_Part = ui.tableWidget_UsersForward->item(ui.tableWidget_UsersForward->currentItem()->row(), 0)->text();
  QString Domain = ui.tableWidget_UsersForward->item(ui.tableWidget_UsersForward->currentItem()->row(), 1)->text();
  
  UsersForwardDeleteDialog *DialogDelete;
  DialogDelete = new UsersForwardDeleteDialog(db_psql, Local_Part, Domain);
  DialogDelete->exec();
  delete DialogDelete;

  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();
	
  }
  
}

void UsersForward::Dialog_Edit_UsersForward(){

  QString Local_Part = ui.tableWidget_UsersForward->item(ui.tableWidget_UsersForward->currentItem()->row(), 0)->text();
  QString Domain = ui.tableWidget_UsersForward->item(ui.tableWidget_UsersForward->currentItem()->row(), 1)->text();
  
  UsersForwardEditDialog *DialogEdit;
  DialogEdit = new UsersForwardEditDialog(db_psql, Local_Part, Domain);
  DialogEdit->exec();
  delete DialogEdit;
  
  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();
	
  }
  
}
void UsersForward::Dialog_Find_UsersForward(){

  UsersForwardFindDialog *DialogFind;
  DialogFind = new UsersForwardFindDialog( db_psql );
  DialogFind->exec();
  delete DialogFind;

  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();
	
  }
  
}

void UsersForward::TestQuery(){

  QSqlQuery query( db_psql );
  
  query.exec("SELECT 1");
  query.clear();
  
}

void UsersForward::SetCompleterList(QStringList list){

  completerModel->setStringList(list);
  
}
