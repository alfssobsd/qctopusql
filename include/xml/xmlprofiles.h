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



#ifndef XML_PROFILES_H
#define XML_PROFILES_H

#include <QFile>
#include <QDomDocument>
#include <QList>
#include <QTextStream>
#include <QString>

class XmlProfiles{

public:

  XmlProfiles(QFile *xmlfile);
  bool XmlCreateDocNull(void);
  int XmlCount(void);
  QString XmlGetElement(int i,QString element);
  void XmlRemoveElement(int i);
  void XmlSetTextElement(int i,QString element,QString value);
  void XmlCreateNullElement(void);
  void XmlSaveFile(QFile *xmlfile);
  
private:
  QList<QDomNode> ListNode;
  QDomDocument domDoc;
  QDomElement domElementRoot;
  QDomElement domElementTmp;
  QDomElement domElementNew;
  QDomElement domElementOld;
  QDomNode domNodeTmp;
  QDomText domTextTmp;

};
#endif
