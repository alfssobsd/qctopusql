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


#include <domains.h>
#include <domains_add_dialog.h>
#include <QProgressDialog>


Domains::Domains (QSqlDatabase db,QWidget *parent)
  : QWidget(parent){
  

  db_psql = db;
  
  ui.setupUi( this );
  connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(GetDomains()));
  connect(ui.tableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
  ui.tableWidget->setColumnWidth(0, 200);

}

void Domains::GetDomains(){


  /*freezing input Domains Widget*/
  ui.pushButton->setEnabled(false);
  ui.comboBox->setEnabled(false);
  ui.tableWidget->setEnabled(false);
  
  TestQuery();
  
  if( db_psql.isOpen() ){ 

	if(ui.tableWidget->isSortingEnabled()){
	  
	  ui.tableWidget->setSortingEnabled(false);

	}
	
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(0);
	
	QSqlQuery query( db_psql );
	
	query.prepare("SELECT domain,type  FROM domains_view WHERE type=:id_type");
	
	switch( ui.comboBox->currentIndex() ){
	  
	case 0 :
	  query.bindValue(":id_type", tr("LOCAL"));
	  break;
	  
	case 1 :
	  query.bindValue(":id_type", tr("RELAY"));
	  break;

	default :
	  query.bindValue(":id_type", tr("VIRTUAL"));
	  
	}

	if( query.exec() ){
	  
	  ui.tableWidget->setRowCount(query.size());
	  QProgressDialog progress(tr("Getting a list of domains."), tr("Cancel"),0,query.size(),this);
	  
	  for(int i = 0; i < query.size(); i++){
		
		query.next();

		progress.setValue(i);
		qApp->processEvents();
		if (progress.wasCanceled()){
		  ui.tableWidget->setRowCount(i);
		  break;
		}
		
		__item0 = new QTableWidgetItem();
		__item0->setText(query.value(0).toString());
		ui.tableWidget->setItem(i, 0, __item0);
		
		__item1 = new QTableWidgetItem();
		__item1->setText(query.value(1).toString());
		ui.tableWidget->setItem(i, 1, __item1);
		
	  }
	  
	  ui.tableWidget->resizeColumnsToContents();
	  
	  if(ui.tableWidget->columnWidth(0) < 200){
		
		ui.tableWidget->setColumnWidth(0, 200);
		
	  }
	  
	  ui.tableWidget->setSortingEnabled(true);
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
  
  /*defrosting input Domains Widget*/
  ui.pushButton->setEnabled(true);
  ui.tableWidget->setEnabled(true);
  ui.comboBox->setEnabled(true);
}

void Domains::showContextMenu(const QPoint &point){

  ui.tableWidget->setCurrentItem(ui.tableWidget->itemAt(point));
  
  QMenu Pop_up;
  
  connect(Pop_up.addAction(tr("New Domain")), SIGNAL(triggered()), this, SLOT(Dialog_Add_Domains()));
  
  if(ui.tableWidget->indexAt(point).row() != -1){

	connect(Pop_up.addAction(tr("Delete Domain")), SIGNAL(triggered()), this, SLOT(Dialog_Delete_Domains()));
	
  }
  
  Pop_up.exec(QCursor::pos());
  
}

void Domains::Dialog_Add_Domains(){
  
  DomainsAddDialog *AddDialog;
  AddDialog = new DomainsAddDialog(db_psql, NULL,NULL);
  AddDialog->exec();
  delete AddDialog;
  
  TestQuery();

  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();
	
  }
  
}

void Domains::Dialog_Delete_Domains(){
  
  QString Domain = ui.tableWidget->item(ui.tableWidget->currentItem()->row(), 0)->text();
  QString Type = ui.tableWidget->item(ui.tableWidget->currentItem()->row(), 1)->text();
  
  DomainsAddDialog *DeleteDialog;
  DeleteDialog = new DomainsAddDialog(db_psql, Domain, Type);
  DeleteDialog->exec();
  delete DeleteDialog;

  TestQuery();

  if( !db_psql.isOpen() ){
	
	emit DisconnectDB();
	
  }
  
}


void Domains::TestQuery(){

  QSqlQuery query( db_psql );
  
  query.exec("SELECT 1");
  query.clear();
  
}
