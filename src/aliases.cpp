/*
Copyright (C) 2008  by Kravchuk Sergei V. (alfss@obsd.ru)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#include <aliases.h>
#include <aliases_add_dialog.h>
#include <aliases_delete_dialog.h>
#include <aliases_edit_dialog.h>
#include <aliases_find_dialog.h>

Aliases::Aliases(QSqlDatabase db, QWidget *parent)
  : QWidget(parent){
  
  db_psql = db;
  
  ui.setupUi( this );
  
  connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(GetAliases()));
  connect(ui.tableWidget_Aliases, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
  
  for(int i = 0; i < 3; i++){
	
	ui.tableWidget_Aliases->setColumnWidth(i, 100);
  
  }
  
}


void Aliases::GetAliases(){
  
  TestQuery();
  
  if( db_psql.isOpen() ){ 

	if(ui.tableWidget_Aliases->isSortingEnabled()){
	  
	  ui.tableWidget_Aliases->setSortingEnabled(false);

	}

	ui.tableWidget_Aliases->clearContents();
	ui.tableWidget_Aliases->setRowCount(0);
	
	QSqlQuery query( db_psql );
	
	query.prepare("SELECT local_part,domain,recipients  FROM aliases_view WHERE domain=:id_domain");
	query.bindValue(":id_domain", ui.lineEdit_Domain->text());
	
	if( query.exec() ){
	  
	  ui.tableWidget_Aliases->setRowCount(query.size());

	  for(int i = 0; i < query.size(); i++){
		
		query.next();
		
		__item0 = new QTableWidgetItem();
		__item0->setText(query.value(0).toString());
		ui.tableWidget_Aliases->setItem(i, 0, __item0);
		
		__item1 = new QTableWidgetItem();
		__item1->setText(query.value(1).toString());
		ui.tableWidget_Aliases->setItem(i, 1, __item1);
		
		__item2 = new QTableWidgetItem();
		__item2->setText(query.value(2).toString());
		ui.tableWidget_Aliases->setItem(i, 2, __item2);
		

	  }
	  
	  ui.tableWidget_Aliases->resizeColumnsToContents();
	  
	  for(int i = 0; i < 3; i++){
		
		if(ui.tableWidget_Aliases->columnWidth(i) < 100){
		
		  ui.tableWidget_Aliases->setColumnWidth(i, 100);
		
		}
	  
	  }
	  
	  ui.tableWidget_Aliases->setSortingEnabled(true);

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
  
}


void Aliases::showContextMenu(const QPoint &point){

  ui.tableWidget_Aliases->setCurrentItem(ui.tableWidget_Aliases->itemAt(point));
  
  QMenu Pop_up;
  
  connect(Pop_up.addAction(tr("New Aliases")), SIGNAL(triggered()), this, SLOT(Dialog_Add_Aliases()));
  connect(Pop_up.addAction(tr("Find Aliases")), SIGNAL(triggered()), this, SLOT(Dialog_Find_Aliases()));
  
  if(ui.tableWidget_Aliases->indexAt(point).row() != -1){
	
	connect(Pop_up.addAction(tr("Delete Aliases")), SIGNAL(triggered()), this, SLOT(Dialog_Delete_Aliases()));
	connect(Pop_up.addAction(tr("Edit Aliases")), SIGNAL(triggered()), this, SLOT(Dialog_Edit_Aliases()));

  }
  
  Pop_up.exec(QCursor::pos());

}

void Aliases::Dialog_Add_Aliases(){
  
  AliasesAddDialog *DialogAdd;
  DialogAdd = new AliasesAddDialog( db_psql );
  DialogAdd->exec();
  delete DialogAdd;

  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();
	
  }
  
}

void Aliases::Dialog_Delete_Aliases(){
  
  QString Local_Part = ui.tableWidget_Aliases->item(ui.tableWidget_Aliases->currentItem()->row(), 0)->text();
  QString Domain = ui.tableWidget_Aliases->item(ui.tableWidget_Aliases->currentItem()->row(), 1)->text();
  
  AliasesDeleteDialog *DialogDelete;
  DialogDelete = new AliasesDeleteDialog(db_psql, Local_Part, Domain);
  DialogDelete->exec();
  delete DialogDelete;

  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();
	
  }
  
}

void Aliases::Dialog_Edit_Aliases(){

  QString Local_Part = ui.tableWidget_Aliases->item(ui.tableWidget_Aliases->currentItem()->row(), 0)->text();
  QString Domain = ui.tableWidget_Aliases->item(ui.tableWidget_Aliases->currentItem()->row(), 1)->text();
  
  AliasesEditDialog *DialogEdit;
  DialogEdit = new AliasesEditDialog(db_psql, Local_Part, Domain);
  DialogEdit->exec();
  delete DialogEdit;

  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();
	
  }
  
}
void Aliases::Dialog_Find_Aliases(){

  AliasesFindDialog *DialogFind;
  DialogFind = new AliasesFindDialog( db_psql );
  DialogFind->exec();
  delete DialogFind;
  
  TestQuery();
  
  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();
	
  }
  
}


void Aliases::TestQuery(){

  QSqlQuery query( db_psql );
  
  query.exec("SELECT 1");
  query.clear();
  
}

