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



#include <xmlprofiles.h>

XmlProfiles::XmlProfiles(QFile *xmlfile){
  
  if(domDoc.setContent(xmlfile)){
	qDebug("Open Xml File");
	
	xmlfile->close();
	
	domElementRoot = domDoc.documentElement();
	
	if(domElementRoot.tagName() != "profiles") {
	  
	  qDebug("Error Tag XML");
	  
	  if( !XmlCreateDocNull() ){
		
		xmlfile->close();
		domElementRoot = domDoc.documentElement();
		
	  }
	  
	}else if(domElementRoot.hasAttribute("version") && domElementRoot.attribute("version") != "1.0") {
	  
	  qDebug("Error Version Config");
	  
	  if( !XmlCreateDocNull() ){
		
		xmlfile->close();
		domElementRoot = domDoc.documentElement();
		 
	  }
	  
	}
	
  }else{
	
	qDebug("Can`t open File");
	qDebug(xmlfile->fileName().toAscii());
	  
	if( !XmlCreateDocNull() ){
	  
	  domElementRoot = domDoc.documentElement();
	  
	}
	
  }
  
  domNodeTmp = domElementRoot.firstChild();

  while( !domNodeTmp.isNull() ) {
	
	ListNode.append(domNodeTmp);
	domNodeTmp = domNodeTmp.nextSibling();

  }
  
}


bool XmlProfiles::XmlCreateDocNull(void){
  bool ok;
  
  QString errorStr,xmlStr;
  int errorLine;
  int errorColumn;
  
  xmlStr = "<?xml version='1.0' encoding='UTF-8' ?><profiles version='1.0'><profile><name>default</name><user>postgres</user><password></password><host>localhost</host><port>5432</port><db>exim</db><context>test context</context></profile><profiles></xml>";

  ok = domDoc.setContent(xmlStr, true,
						 &errorStr, &errorLine, &errorColumn);

  return ok;

}

int XmlProfiles::XmlCount(void){
  
  return  ListNode.size();
}

QString XmlProfiles::XmlGetElement(int i, QString element){

  domElementTmp = ListNode.at(i).toElement();
  
  return domElementTmp.firstChildElement(element).text();

}

void XmlProfiles::XmlRemoveElement(int i){
  
  domElementRoot.removeChild(ListNode.at(i));
  ListNode.removeAt(i);

}

void XmlProfiles::XmlSaveFile(QFile *xmlfile){

  const int IndentSize = 4;
  
  xmlfile->open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream out(xmlfile);
  domDoc.save(out, IndentSize);
  xmlfile->close();
  
}

void XmlProfiles::XmlCreateNullElement(void){
  
  domElementNew = domDoc.createElement("profile"); // create element <profile>

  domTextTmp = domDoc.createTextNode("New Profile"); // create sub element name
  domElementTmp = domDoc.createElement("name");		 // <profile>
  domElementTmp.appendChild(domTextTmp);			 //     |
  domElementNew.appendChild(domElementTmp);			 //     -><name>

  domTextTmp = domDoc.createTextNode("User");	// create sub element user
  domElementTmp = domDoc.createElement("user"); // <profile>
  domElementTmp.appendChild(domTextTmp);		//     |
  domElementNew.appendChild(domElementTmp);		//     -><user>
  
  domTextTmp = domDoc.createTextNode("Password");
  domElementTmp = domDoc.createElement("password");
  domElementTmp.appendChild(domTextTmp);
  domElementNew.appendChild(domElementTmp);

  domTextTmp = domDoc.createTextNode("Host name");
  domElementTmp = domDoc.createElement("host");
  domElementTmp.appendChild(domTextTmp);
  domElementNew.appendChild(domElementTmp);

  domTextTmp = domDoc.createTextNode("5432");
  domElementTmp = domDoc.createElement("port");
  domElementTmp.appendChild(domTextTmp);
  domElementNew.appendChild(domElementTmp);

  domTextTmp = domDoc.createTextNode("Database name");
  domElementTmp = domDoc.createElement("db");
  domElementTmp.appendChild(domTextTmp);
  domElementNew.appendChild(domElementTmp);

  domTextTmp = domDoc.createTextNode("Text");
  domElementTmp = domDoc.createElement("context");
  domElementTmp.appendChild(domTextTmp);
  domElementNew.appendChild(domElementTmp);

  ListNode.append(domElementRoot.appendChild(domElementNew)); // add element <profile>
                                                              // <profiles>
                                                              //    |
                                                              //    -><profile>
}

void XmlProfiles::XmlSetTextElement(int i, QString element, QString value){


  domTextTmp = domDoc.createTextNode(value);

  domElementNew = domDoc.createElement(element);
  domElementNew.appendChild(domTextTmp);
  
  domElementTmp = ListNode.at(i).toElement();//.appendChild(domTextTmp);
  domElementOld = domElementTmp.firstChildElement(element);

  domElementTmp.replaceChild(domElementNew,domElementOld);
 
}
