#include "XMLParse.h"
#include <direct.h>		// Needed for getcwd()
#include <fstream>
#include <regex>

int nm;

XMLParse::XMLParse(void)
{
}

XMLParse::~XMLParse(void)
{
}
extern CListCtrl m_listCtrl;
/*********************************************************************************************
* Def : Opens and parse file which in the given path and loads contents to list control item. 
* Args : a_strFile => XML file's path
* Ret : if xml file is found and loaded properly returns true else returns false
**********************************************************************************************/
bool XMLParse::LoadFromXML(const char* a_strFile)
{
	tinyxml2::XMLElement* pXML = nullptr;	//variable for xml file operations
	char szBuf[_MAX_PATH + 1];
	CString strTemp = a_strFile;
	_getcwd(szBuf, sizeof(szBuf));
	strcat(szBuf, "\\");
	strcat(szBuf, strTemp.GetBuffer(1));	//All file path assigned to szBuf
	std::ifstream ifile;
	ifile.open(szBuf);
	if(!ifile) return false;
	if (!xmlDoc.LoadFile(szBuf))	//Load xml file contents
	{
		// XML root
		pXML = xmlDoc.RootElement();	//load xml file's root
		if (nullptr == pXML) return false;
		
		Load(pXML);				//Load list contents
		return true;
	}
	return false;
}

/*************************************************************************************************
* Def : Takes root element and goes to first customer node and loads all customers to list control 
*		by iterating customer elements respectively.
* Args : a_pNode => XML file's root
*************************************************************************************************/
void XMLParse::Load(tinyxml2::XMLElement* a_pNode)
{
	ASSERT(nullptr != a_pNode);
	// Get node "data" data is parent node of customer nodes
	tinyxml2::XMLElement* pCustomers = a_pNode->FirstChildElement("data");
	tinyxml2::XMLElement* pCustomer = nullptr;
	if (nullptr == pCustomers) return;
	// Get first customer
	pCustomer = pCustomers->FirstChildElement("customer");
	LoadItem(pCustomer);	
}

/*************************************************************************************************
* Def : Takes first customer element and load contents to list control by iterating first customer
* Args : a_pNode => first customer element
*************************************************************************************************/
void XMLParse::LoadItem(tinyxml2::XMLElement* a_pNode)
{
	CRect rect;
	m_listCtrl.GetClientRect(&rect);
	int nColInterval = rect.Width() / 10;

	m_listCtrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, nColInterval);
	m_listCtrl.InsertColumn(1, _T("Name"), LVCFMT_LEFT, nColInterval * 2);
	m_listCtrl.InsertColumn(2, _T("Subname"), LVCFMT_LEFT, 2 * nColInterval);
	m_listCtrl.InsertColumn(3, _T("Tel"), LVCFMT_LEFT, 2 * nColInterval);
	m_listCtrl.InsertColumn(4, _T("Address"), LVCFMT_LEFT, 3 * nColInterval);
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listCtrl.SetExtendedStyle(LVS_EX_GRIDLINES);
	//m_listCtrl.SetExtendedStyle( LVS_SHOWSELALWAYS);
	while (a_pNode)		//iterate a_pNode till the last element
	{
		//parse and take all childs of each customer element
		tinyxml2::XMLElement* pName = a_pNode->FirstChildElement("name");	//name child of customer element
		tinyxml2::XMLElement* pSubname = pName->NextSiblingElement();	//subName child of customer element
		tinyxml2::XMLElement* phone = pSubname->NextSiblingElement();	//tel child of customer element
		tinyxml2::XMLElement* address = phone->NextSiblingElement();	//address child of customer element

		//take each child element text value
		CString id = a_pNode->Attribute("id");
		CString name = pName->GetText();
		CString subname = pSubname->GetText();
		CString tel = phone->GetText();
		CString adres = address->GetText();

		int nIndex = m_listCtrl.InsertItem(nm++, id);
		m_listCtrl.SetItemText(nIndex, 1, name);
		m_listCtrl.SetItemText(nIndex, 2, subname);
		m_listCtrl.SetItemText(nIndex, 3, tel);
		m_listCtrl.SetItemText(nIndex, 4, adres);
		
		a_pNode = a_pNode->NextSiblingElement("customer");	// pass the next customer
	}
}

/*************************************************************************************************
* Def : Takes customer element node and delete it from CustomerDB.xml file and also 
* remove this element from list by going its place inside list control each list element has 6 item
* to go its place multiple with 6 and remove 6 next element of list control
* Args : node => customer element, num => customer's index 
*************************************************************************************************/
void XMLParse::deleteNode(tinyxml2::XMLElement* node, int num)
{
	if (node)
	{
		//delete node and save xml file as this node is deleted
		xmlDoc.DeleteNode(node);	
		xmlDoc.SaveFile("CustomerDB.xml");	
		m_listCtrl.DeleteItem(num);
	}
}

/*
The given argument to pattern() is regular expression. With the help of
regular expression we can validate mobile number. 
1) Begins with 0 or 91  2) Then contains 7 or 8 or 9.  3) Then contains 9 digits
*/ 
bool XMLParse::isValid(CString s)
{
	const std::regex pattern("(0|91)?[7-9][0-9]{9}");
	std::string str((LPCTSTR)s);
	// regex_match() is used to
	// to find match between given number
	// and regular expression
	if (std::regex_match(str, pattern))
	{
		return true;
	}
	else
	{
		return false;
	}
}