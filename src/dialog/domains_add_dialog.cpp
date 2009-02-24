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


#include <domains_add_dialog.h>



DomainsAddDialog::DomainsAddDialog (QSqlDatabase db, QStringListModel *model, QString domain,QString type){

  db_psql = db;
  
  setupUi( this );
  comboBox->setModel(model);
  
  if( type != NULL || domain != NULL){
	
	label_Domain->setText(tr("Deleted Domain"));
	lineEdit_Domain->setText(domain);
	lineEdit_Domain->setReadOnly(true);
	
	this->setWindowTitle(tr("Deleted Domain"));

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(Deleted()));

	comboBox->setCurrentIndex(comboBox->findText(type));
	comboBox->setEnabled(false);
	
  }else{
	
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(New()));
	
  }
}

void DomainsAddDialog::New(){

  TestQuery();
  
  if( !lineEdit_Domain->text().isEmpty() ){

	if( db_psql.isOpen() ){ 
	  
	  QSqlQuery query( db_psql );
	  
	  query.prepare("INSERT INTO domains (domain,id_type) VALUES (:domain, get_domain_type_id(:id_type))");
	  
	  query.bindValue(":id_type", comboBox->currentText());
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
	
	query.prepare("DELETE FROM domains WHERE domain=:domain and id_type=get_domain_type_id(:id_type)");
	
	query.bindValue(":id_type", comboBox->currentText());
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
