#include "XMLParse.h"
#include <direct.h>		// Needed for getcwd()
#include <fstream>

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
	int num = 0;
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
		
		//insert all childs to list control
		//num variable is used for adding all child elements respectively to list control
		m_listCtrl.InsertItem(num++, "----------data----------");
		m_listCtrl.InsertItem(num++, "id : " + id);
		m_listCtrl.InsertItem(num++, "Name : " + name);
		m_listCtrl.InsertItem(num++, "SubName : " + subname);
		m_listCtrl.InsertItem(num++, "Tel : " + tel);
		m_listCtrl.InsertItem(num++, "Address : " + adres);
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
		int index = num * 6;
		int count = m_listCtrl.GetItemCount();
		// Delete all items of this node from the list view control.
		for (int nItem = 0; nItem < count; nItem++)
		{
			if(nItem >= index && nItem < (index+6))
				m_listCtrl.DeleteItem(index);
		}
	}
}