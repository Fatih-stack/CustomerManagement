#pragma once
// Don't forget the TinyXML include :-)
#include "tinyxml2.h"
#include "StdAfx.h"
#include <iostream>

extern int nm;

class XMLParse : public CListCtrl
{

public:
	XMLParse(void);
	virtual ~XMLParse(void);

public:
	
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLElement* dataElement;
	bool LoadFromXML(const char* a_strFile);
	void Load(tinyxml2::XMLElement* a_pNode);
	void LoadItemList(tinyxml2::XMLElement* a_pNode);
	void LoadItem(tinyxml2::XMLElement* a_pNode);
	void deleteNode(tinyxml2::XMLElement* node, int num);
	bool isValid(const CString& s);
	bool is_number(const CString& s);
};