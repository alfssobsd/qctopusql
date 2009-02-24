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


#include <main_window.h>
#include <profiles.h>
#include <about.h>
#include <QWidget>


MainWindow::MainWindow(QWidget *parent)
  : QWidget(parent)
{
  db = QSqlDatabase::addDatabase("QPSQL", "Exim");


  Tab = new QTabWidget;
  layout = new QHBoxLayout;
  LoginWidget = new QWidget;
  
  ui.setupUi( LoginWidget );
  
  connect(ui.pushButton_Profile, SIGNAL(clicked()), this, SLOT(DialogProfiles()));
  connect(ui.pushButton_Connect, SIGNAL(clicked()), this, SLOT(ConnectDB()));
  connect(ui.pushButton_About, SIGNAL(clicked()), this, SLOT(About()));
  
  UserWidget = new User(db, this);
  AliasesWidget = new Aliases(db, this);
  UsersForwardWidget = new UsersForward(db, this);
  DomainsWidget = new Domains(db, this);
  
  connect(UserWidget, SIGNAL(DisconnectDB()), this, SLOT(DisconnectDB()));
  connect(AliasesWidget, SIGNAL(DisconnectDB()), this, SLOT(DisconnectDB()));
  connect(UsersForwardWidget, SIGNAL(DisconnectDB()), this, SLOT(DisconnectDB()));
  connect(DomainsWidget, SIGNAL(DisconnectDB()), this, SLOT(DisconnectDB()));
  
  Tab->addTab(UserWidget, tr("Users"));
  Tab->addTab(AliasesWidget, tr("Aliases"));
  Tab->addTab(UsersForwardWidget, tr("Users Forward"));
  Tab->addTab(DomainsWidget, tr("Domains"));
  Tab->setEnabled(false);

  layout->addWidget(LoginWidget);
  layout->addWidget(Tab);
  
  setLayout(layout);

}

MainWindow::~MainWindow(){
  
  delete LoginWidget;
  delete UserWidget;
  delete AliasesWidget;
  delete UsersForwardWidget;
  delete DomainsWidget;
  delete Tab;
  delete layout;
}

void MainWindow::About(){

  AboutDialog *Dialog;
  Dialog = new AboutDialog();
  Dialog->exec();
  delete Dialog;
  
}
void MainWindow::DialogProfiles(){

  Profiles profil(ui.lineEdit_Host, ui.lineEdit_Port, ui.lineEdit_DB, ui.lineEdit_User, ui.lineEdit_Password);
  profil.exec();

}

void MainWindow::ConnectDB(){
  bool ok;
  
  qDebug("Connect");

  db.setHostName(ui.lineEdit_Host->text()); //host name
  db.setDatabaseName(ui.lineEdit_DB->text());//db name
  db.setUserName(ui.lineEdit_User->text());//user name
  db.setPassword(ui.lineEdit_Password->text()); //password
  db.setPort(ui.lineEdit_Port->text().toInt(&ok, 10));

  if( !db.open() ){

	QMessageBox::warning(this, tr("Connections Error"),
						 db.lastError().text(),
						 QMessageBox::Ok);
  }else{
	ui.pushButton_Profile->setEnabled(false);
	ui.lineEdit_Port->setEnabled(false);
	ui.lineEdit_Host->setEnabled(false);
	ui.lineEdit_DB->setEnabled(false);
	ui.lineEdit_Password->setEnabled(false);
	ui.lineEdit_User->setEnabled(false);
	Tab->setEnabled(true);
	
	disconnect(ui.pushButton_Connect, SIGNAL(clicked()), this, SLOT(ConnectDB()));
	connect(ui.pushButton_Connect, SIGNAL(clicked()), this, SLOT(DisconnectDB()));
	ui.pushButton_Connect->setText(tr("Disconnect"));

	GetAllDomains();
	GetTypeDomains();
  }
  
}

void MainWindow::DisconnectDB(){

  qDebug("Disconnect");

  if( !db.isOpen() ){
	QMessageBox::warning(this, tr("Disconnect"),
						 tr("Disconnect Database %1").arg(ui.lineEdit_DB->text()),
						 QMessageBox::Ok);
  }

  disconnect(ui.pushButton_Connect, SIGNAL(clicked()), this, SLOT(DisconnectDB()));
  connect(ui.pushButton_Connect, SIGNAL(clicked()), this, SLOT(ConnectDB()));
  ui.pushButton_Connect->setText(tr("Connect"));
  
  ui.pushButton_Profile->setEnabled(true);
  ui.lineEdit_Port->setEnabled(true);
  ui.lineEdit_Host->setEnabled(true);
  ui.lineEdit_DB->setEnabled(true);
  ui.lineEdit_Password->setEnabled(true);
  ui.lineEdit_User->setEnabled(true);
  Tab->setEnabled(false);
	  
  
  db.close();

}

int MainWindow::GetAllDomains(){

  QStringList tmp_list;
	
  QSqlQuery query( db );
  
  if( db.isOpen() ){
	
	query.prepare("SELECT domain FROM domains");
	
	if( query.exec() ){
	  
	  for(int i = 0; i < query.size(); i++){
		
		query.next();
		tmp_list << query.value(0).toString();
		
	  }
	  
	  query.clear();

	  UserWidget->SetCompleterList(tmp_list);
	  UsersForwardWidget->SetCompleterList(tmp_list);
	  AliasesWidget->SetCompleterList(tmp_list);
	  
	}else{
	  
	  QMessageBox::warning(this, tr("Query Error"),
						   query.lastError().text(),
						   QMessageBox::Ok);
	  return 1;
	  
	}
	
  }else{

	DisconnectDB();
	return 1;

  }

  return 0;

}

int MainWindow::GetTypeDomains(){

  QStringList tmp_list;
  QSqlQuery query( db );

  if( db.isOpen() ){
	
	query.prepare("SELECT type FROM domain_types ORDER BY id");

	if( query.exec() ){
	  for(int i = 0; i < query.size(); i++){

		query.next();
		tmp_list << query.value(0).toString();

	  }
	  
	  query.clear();
	  DomainsWidget->setTypeModel(tmp_list);

	}else{

	  QMessageBox::warning(this, tr("Query Error"),
						   query.lastError().text(),
						   QMessageBox::Ok);
	  return 1;
	  
	}	
  }else{

	DisconnectDB();
	return 1;
	
  }
  
  return 0;
}
