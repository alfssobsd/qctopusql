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


#include <domains_add_dialog.h>



DomainsAddDialog::DomainsAddDialog (QSqlDatabase db, QString domain, QString type){

  db_psql = db;
  
  setupUi( this );
  if( domain != NULL){
	
	label_Domain->setText(tr("Deleted Domain"));
	lineEdit_Domain->setText(domain);
	lineEdit_Domain->setReadOnly(true);
	
	this->setWindowTitle(tr("Deleted Domain"));

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(Deleted()));
	
	if(0 == QString::compare("LOCAL", type, Qt::CaseInsensitive)){
	  
	  comboBox->setCurrentIndex(0);
	  comboBox->setEnabled(false);
	  
	}
	
	if(0 == QString::compare("RELAY", type, Qt::CaseInsensitive)){
	  
	  comboBox->setCurrentIndex(1);
	  comboBox->setEnabled(false);
	
	}

	if(0 == QString::compare("VIRTUAL", type, Qt::CaseInsensitive)){
	  
	  comboBox->setCurrentIndex(2);
	  comboBox->setEnabled(false);
	
	}
	 
	
  }else{
	
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(New()));
	
  }
  
  

}

void DomainsAddDialog::New(){

  TestQuery();
  
  if( !lineEdit_Domain->text().isEmpty() ){

	if( db_psql.isOpen() ){ 
	  
	  QSqlQuery query( db_psql );
	  
	  query.prepare("INSERT INTO domains (domain,id_type) VALUES (:domain, :id_type)");
	  
	  query.bindValue(":id_type", comboBox->currentIndex());
	  query.bindValue(":domain", lineEdit_Domain->text());
	  
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

void DomainsAddDialog::Deleted(){
  
  TestQuery();
  
  if( db_psql.isOpen() ){ 
	
	QSqlQuery query( db_psql );
	
	query.prepare("DELETE FROM domains WHERE domain=:domain and id_type=:id_type");
	
	query.bindValue(":id_type", comboBox->currentIndex());
	query.bindValue(":domain", lineEdit_Domain->text());
	
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

void DomainsAddDialog::TestQuery(){

  QSqlQuery query( db_psql );
  
  query.exec("SELECT 1");
  query.clear();
  
}
