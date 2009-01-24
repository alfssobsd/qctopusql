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


#include <profiles.h>


Profiles::Profiles (QLineEdit *host, QLineEdit *port, QLineEdit *db, QLineEdit *login, QLineEdit *Psw,QWidget *parent)
  :QDialog(parent){

  QLineHost = host;
  QLinePort = port;
  QLineLogin = login;
  QLinePsw = Psw;
  QLineDB = db;
  
#ifdef Q_OS_WIN32
  ConfigFile.append("qctoupusql.xml");
#else
  QDir ConfigDir;
  ConfigDir.setPath(ConfigDir.homePath());
  ConfigDir.mkdir(".qctoupusql");
  ConfigFile.append(ConfigDir.homePath());
  ConfigFile.append("/.qctoupusql/qctoupusql.xml");
#endif
  
  setupUi( this );
  
  file.setFileName(ConfigFile);
  config = new XmlProfiles(&file);
  
  
  for(int i = 0; i < config->XmlCount(); i++){
	
	listWidget->addItem(config->XmlGetElement(i,"name"));
	
  }
  if(listWidget->count() > 0){
	
	listWidget->setCurrentRow(0);
	LoadProfile(listWidget->currentItem());
	
  }
  
  connect(listWidget, SIGNAL(itemClicked(QListWidgetItem * )), this, SLOT(LoadProfile(QListWidgetItem * )));
  connect(pushButton_Save, SIGNAL(clicked()), this, SLOT(SaveProfile()));
  connect(pushButton_New, SIGNAL(clicked()), this, SLOT(NewProfile()));
  connect(pushButton_Delete, SIGNAL(clicked()), this, SLOT(DeleteProfile()));
  connect(pushButton_Ok, SIGNAL(clicked()), this, SLOT(AcceptProfile()));
}

void Profiles::LoadProfile(QListWidgetItem * item){

  int currentRow;

  currentRow = listWidget->row(item);
  
  lineEdit_Name->setText(config->XmlGetElement(currentRow, "name"));
  lineEdit_User->setText(config->XmlGetElement(currentRow, "user"));
  lineEdit_Password->setText(config->XmlGetElement(currentRow, "password"));
  lineEdit_Port->setText(config->XmlGetElement(currentRow, "port"));
  lineEdit_Host->setText(config->XmlGetElement(currentRow, "host"));
  lineEdit_Database->setText(config->XmlGetElement(currentRow, "db"));
  textEdit->setText(config->XmlGetElement(currentRow, "context"));
  
}

void Profiles::SaveProfile(){

  if(listWidget->count() < 1){

	config->XmlCreateNullElement();
	listWidget->addItem(lineEdit_Name->text());
	listWidget->setCurrentRow(0);
	LoadProfile(listWidget->currentItem());
	
	config->XmlSetTextElement(0,"name",lineEdit_Name->text());
	config->XmlSetTextElement(0,"user",lineEdit_User->text());
	config->XmlSetTextElement(0,"password",lineEdit_Password->text());
	config->XmlSetTextElement(0,"port",lineEdit_Port->text());
	config->XmlSetTextElement(0,"host",lineEdit_Host->text());
	config->XmlSetTextElement(0,"db",lineEdit_Database->text());
	config->XmlSetTextElement(0,"context",textEdit->toPlainText());

  }else{
	int currentRow;

	currentRow = listWidget->currentRow();
	listWidget->currentItem()->setText(lineEdit_Name->text());
	
	config->XmlSetTextElement(currentRow,"name",lineEdit_Name->text());
	config->XmlSetTextElement(currentRow,"user",lineEdit_User->text());
	config->XmlSetTextElement(currentRow,"password",lineEdit_Password->text());
	config->XmlSetTextElement(currentRow,"port",lineEdit_Port->text());
	config->XmlSetTextElement(currentRow,"host",lineEdit_Host->text());
	config->XmlSetTextElement(currentRow,"db",lineEdit_Database->text());
	config->XmlSetTextElement(currentRow,"context",textEdit->toPlainText());

  }

  file.setFileName(ConfigFile);
  config->XmlSaveFile(&file);

}

void Profiles::NewProfile(){

  int currentRow;
 
  config->XmlCreateNullElement();
  listWidget->addItem("New Profile");
  currentRow = (listWidget->count() - 1);
  listWidget->setCurrentRow(currentRow);

  lineEdit_Name->setText(config->XmlGetElement(currentRow, "name"));
  lineEdit_User->setText(config->XmlGetElement(currentRow, "user"));
  lineEdit_Password->setText(config->XmlGetElement(currentRow, "password"));
  lineEdit_Port->setText(config->XmlGetElement(currentRow, "port"));
  lineEdit_Host->setText(config->XmlGetElement(currentRow, "host"));
  lineEdit_Database->setText(config->XmlGetElement(currentRow, "db"));
  textEdit->setText(config->XmlGetElement(currentRow, "context"));
  
  LoadProfile(listWidget->currentItem());
  
}

void Profiles::DeleteProfile(){
  
  if(listWidget->count() > 0){
	
	config->XmlRemoveElement( listWidget->currentRow() );
	listWidget->clear();

	for(int i = 0; i < config->XmlCount(); i++){
	  
	  listWidget->addItem(config->XmlGetElement(i,"name"));
	  
	}
	
	if(listWidget->count() >= 1){

	  listWidget->setCurrentRow(0);
	  LoadProfile(listWidget->item(0));
	  
	}
	
  }
  
}

void Profiles::AcceptProfile(){
  
  QLineHost->setText(lineEdit_Host->text());
  QLinePort->setText(lineEdit_Port->text());
  QLineLogin->setText(lineEdit_User->text());
  QLinePsw->setText(lineEdit_Password->text());
  QLineDB->setText(lineEdit_Database->text());
  this->accept();
  
}
