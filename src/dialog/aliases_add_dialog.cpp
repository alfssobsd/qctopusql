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


#include <aliases_add_dialog.h>
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

AliasesAddDialog::AliasesAddDialog(QSqlDatabase db, QWidget *parent)
        : QDialog(parent) {

  db_psql = db;

  setupUi( this );
  
  alpha = new QRegExp("-?[a-z.\\-_0-9]+");
  ValLocal_Part = new QRegExpValidator(*alpha, this);
  
  connect(pushButton_Add, SIGNAL(clicked()), this, SLOT(NewRow()));
  connect(pushButton_Deleted, SIGNAL(clicked()), this, SLOT(DeleteRow()));
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(Add()));

  lineEdit_Local_Part->setValidator(ValLocal_Part);
  tableWidget_recipients->setColumnWidth(0, 230);
  tableWidget_recipients->setColumnWidth(1, 230);

  completer = new QCompleter(this);
  lineEdit_Domain->setCompleter(completer);
}

AliasesAddDialog::~AliasesAddDialog(){

  delete completer;
  delete alpha;
  delete ValLocal_Part;
}

void AliasesAddDialog::NewRow(){
  
  tableWidget_recipients->setRowCount(tableWidget_recipients->rowCount() + 1);

  QTableWidgetItem *__item0 = new QTableWidgetItem();
  __item0->setText(tr(""));
  tableWidget_recipients->setItem(tableWidget_recipients->rowCount() - 1, 0, __item0);

  QTableWidgetItem *__item1 = new QTableWidgetItem();
  __item1->setText(tr(""));
  tableWidget_recipients->setItem(tableWidget_recipients->rowCount() - 1, 0, __item1);

}

void AliasesAddDialog::DeleteRow(){
  
  tableWidget_recipients->removeRow(tableWidget_recipients->rowCount() - 1);

}


void AliasesAddDialog::Add(){
  
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
	  
	  query.prepare("INSERT INTO aliases (local_part,id_domain,recipients)"
					"VALUES (:local_part,get_domain_id(:domain),:recipients)");
	  
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



bool AliasesAddDialog::Empty_Test(){
  
 QPalette pal;
 
 lineEdit_Local_Part->setPalette(QApplication::palette());
 lineEdit_Domain->setPalette(QApplication::palette());

 pal.setColor(QPalette::Base, Qt::red);

 if(lineEdit_Domain->text().isEmpty()){
   
   lineEdit_Domain->setPalette(pal);
   
 }else if(lineEdit_Local_Part->text().isEmpty()){
   
   lineEdit_Local_Part->setPalette(pal);
 
 }else{
   
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
 
  return false;
  
}


void AliasesAddDialog::TestQuery(){

  QSqlQuery query( db_psql );
  
  query.exec("SELECT 1");
  query.clear();
  
}

void AliasesAddDialog::setCompleterModel(QAbstractItemModel *model){

  completer->setModel(model);
  
}
