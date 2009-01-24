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


#include <users_add_dialog.h>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegExp>
#include <QValidator>
#include <QPalette>
#include <QComboBox>
#include <QSpinBox>

UsersAddDialog::UsersAddDialog(QSqlDatabase db,QWidget *parent)
        : QDialog(parent) {

  db_psql = db;

  setupUi( this );
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(Add()));
  
  QRegExp alpha("-?[a-z.\\-_0-9]+");
  QValidator *ValLogin = new QRegExpValidator(alpha, this);
  
  lineEdit_Login->setValidator(ValLogin);
  lineEdit_Domain->setValidator(ValLogin);
  
}


bool  UsersAddDialog::Empty_Test(){
  
  QPalette pal;
  
  lineEdit_Login->setPalette(QApplication::palette());
  lineEdit_Name->setPalette(QApplication::palette());
  lineEdit_Password->setPalette(QApplication::palette());
  lineEdit_Domain->setPalette(QApplication::palette());
  
  
  pal.setColor(QPalette::Base, Qt::red);
  
  if(lineEdit_Login->text().isEmpty()){
	
	lineEdit_Login->setPalette(pal);
	
  }else if(lineEdit_Name->text().isEmpty()){
	
	lineEdit_Name->setPalette(pal);
 
  }else if(lineEdit_Password->text().isEmpty()){
	
	lineEdit_Password->setPalette(pal);
   
  }else if(lineEdit_Domain->text().isEmpty()){
	
	lineEdit_Domain->setPalette(pal);
	
  }else{
	
	return true;
	
  }
  
  return false;
  
}



void UsersAddDialog::Add(){
 
  if( Empty_Test() ){
	
	QSqlQuery query( db_psql );
	
	query.exec("SELECT 1");
	query.clear();
	
	if( db_psql.isOpen() ){
	  
	  query.prepare("INSERT INTO users (login, name, password, decrypt, id_domain, quota, status, id_spam, spam_score)"
					"VALUES ( :login, :name, crypt(:password, gen_salt('des')), :decrypt, get_domain_id(:domain), :quota, :status, :spam, :spam_score)");
	  query.bindValue(":login", lineEdit_Login->text());
	  query.bindValue(":name", lineEdit_Name->text());
	  query.bindValue(":password",lineEdit_Password->text());
	  query.bindValue(":decrypt", lineEdit_Password->text());
	  query.bindValue(":domain", lineEdit_Domain->text());
	  query.bindValue(":quota", spinBox_Quote->value());
	  
	  if(checkBox->checkState() == Qt::Checked){

		query.bindValue(":status", 1);

	  }else{

		query.bindValue(":status", 0);

	  }
	  
	  query.bindValue(":spam",comboBox_Spam->currentIndex());
	  query.bindValue(":spam_score",spinBox_Spam_Score->value() );

	  if(!query.exec()){

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
