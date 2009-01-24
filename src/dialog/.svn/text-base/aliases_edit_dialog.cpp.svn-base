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


#include <aliases_edit_dialog.h>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegExp>
#include <QValidator>
#include <QPalette>
#include <QComboBox>
#include <QSpinBox>
#include <QString>
#include <QTableWidgetItem>
#include <QStringList>
#include <QString>

AliasesEditDialog::AliasesEditDialog(QSqlDatabase db,QString local_part, QString domain, QWidget *parent)
        : QDialog(parent){
  
  QStringList list_recipients, list2_recipients;
  
  db_psql=db;
  
  setupUi( this );
  
  index = NULL;
  lineEdit_Local_Part->setText(local_part);
  lineEdit_Domain->setText(domain);
  
  /*

	Get recipients from aliases

   */
  TestQuery();
  
  if( db_psql.isOpen() ){
	
	QSqlQuery query( db_psql );
	
	query.prepare("SELECT recipients FROM aliases_view WHERE local_part=:local_part and domain=:domain LIMIT 1");
	query.bindValue(":local_part", lineEdit_Local_Part->text());
	query.bindValue(":domain", lineEdit_Domain->text());
	
	if( query.exec() ){
	  
	  for(int i = 0; i < query.size(); i++){
		
		query.next();
		
		list_recipients = query.value(0).toString().split(",");
		
		tableWidget_recipients->setRowCount(list_recipients.size());
		
		for(int j = 0; j < list_recipients.size(); j++){
		  
		  list2_recipients=list_recipients.at(j).split("@");
		  
		  __item0 = new QTableWidgetItem();
		  __item0->setText(list2_recipients.at(0));
		  tableWidget_recipients->setItem(j, 0, __item0);

		  if(list2_recipients.count() > 1){
			
			__item1 = new QTableWidgetItem();
			__item1->setText(list2_recipients.at(1));
			tableWidget_recipients->setItem(j, 1, __item1);

		  }else{
			
			__item1 = new QTableWidgetItem();
			__item1->setText(lineEdit_Domain->text());
			tableWidget_recipients->setItem(j, 1, __item1);

		  }
		  
		}
		
	  }

	  query.clear();
	  
	}else{
	  
	  QMessageBox::warning(this, tr("Query Error"),
						   query.lastError().text(),
						   QMessageBox::Ok);

	  query.clear();
	  
	}
	
  }else{

	this->reject();
	
  }
  
  
  lineEdit_Local_Part->setReadOnly(true);
  lineEdit_Domain->setReadOnly(true);
  
  connect(pushButton_Add, SIGNAL(clicked()), this, SLOT(NewRow()));
  connect(pushButton_Deleted, SIGNAL(clicked()), this, SLOT(DeleteRow()));
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(Update()));
  connect(tableWidget_recipients ,SIGNAL(itemClicked(QTableWidgetItem * )), this, SLOT(SelectRow(QTableWidgetItem * )));//updte

  
  tableWidget_recipients->setColumnWidth(0, 230);
  tableWidget_recipients->setColumnWidth(1, 230);
 
}


void AliasesEditDialog::NewRow(){
  
  tableWidget_recipients->setRowCount(tableWidget_recipients->rowCount() + 1);

  QTableWidgetItem *__item0 = new QTableWidgetItem();
  __item0->setText(tr(""));
  tableWidget_recipients->setItem(tableWidget_recipients->rowCount() - 1, 0, __item0);

  QTableWidgetItem *__item1 = new QTableWidgetItem();
  __item1->setText(tr(""));
  tableWidget_recipients->setItem(tableWidget_recipients->rowCount() - 1, 0, __item1);

}

void AliasesEditDialog::DeleteRow(){
  

  if(index != NULL){
	
	tableWidget_recipients->removeRow(index->row());
	index = NULL;
  
  }else{
	
	tableWidget_recipients->removeRow( tableWidget_recipients->rowCount()-1 );
  
  }

}



void AliasesEditDialog::SelectRow(QTableWidgetItem *itemTable ){
  
  index = itemTable;

}


void AliasesEditDialog::Update(){
  
  QString recipients;
  
  if( Empty_Test() ){

	/*
	  Create string recipients
	 */
	
	for(int i = 0; i < tableWidget_recipients->rowCount(); i++){
	  	
	  recipients.append(tableWidget_recipients->item(i, 0)->text());
	  recipients.append("@");
	  recipients.append(tableWidget_recipients->item(i, 1)->text());
	  
	  if(tableWidget_recipients->rowCount() != (i + 1)){
		
		recipients.append(",");
	  
	  }
	  
	}

	TestQuery();
	
	if( db_psql.isOpen() ){ 
	  
	  QSqlQuery query( db_psql );

	  query.prepare("UPDATE aliases SET recipients=:recipients WHERE local_part=:local_part and id_domain=get_domain_id(:domain)");
	  
	  query.bindValue(":local_part", lineEdit_Local_Part->text());
	  query.bindValue(":domain", lineEdit_Domain->text());
	  query.bindValue(":recipients", recipients);
	  
	  if( !query.exec() ){
		
		QMessageBox::warning(this, tr("Query Error"),
							 query.lastError().text(),
							 QMessageBox::Ok);
		query.clear();
		
	  }else{
		
		query.clear();
		this->accept();
	  
	  }
	  
	}else{

	  this->reject();
	
	}
	
  } 
  
} 

bool AliasesEditDialog::Empty_Test(){
  
  if(tableWidget_recipients->rowCount() == 0){
	
	return false;
  
  }
  
  for(int i = 0; i < tableWidget_recipients->rowCount(); i++){
	
	if(tableWidget_recipients->item(i, 0)->text().isEmpty()){
	  
	  return false;
	
	}
	
	if(tableWidget_recipients->item(i, 1)->text().isEmpty()){
	  
	  return false;
	
	}
	
  }
  
  return true;

}

void AliasesEditDialog::TestQuery(){

  QSqlQuery query( db_psql );
  
  query.exec("SELECT 1");
  query.clear();
  
}
