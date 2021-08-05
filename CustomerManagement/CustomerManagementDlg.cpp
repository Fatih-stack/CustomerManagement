
// CustomerManagementDlg.cpp : implementation file
//

#include "CustomerManagement.h"
#include "CustomerManagementDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CListCtrl m_listCtrl;
// CCustomerManagementDlg dialog
CCustomerManagementDlg::CCustomerManagementDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CUSTOMERMANAGEMENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCustomerManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CCustomerManagementDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHANGE, &CCustomerManagementDlg::OnBnClickedChange)
	ON_BN_CLICKED(IDC_DELETE, &CCustomerManagementDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_SAVE, &CCustomerManagementDlg::OnBnClickedSave)
END_MESSAGE_MAP()


// CCustomerManagementDlg message handlers

BOOL CCustomerManagementDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	bool file_opened = xmlParse.LoadFromXML("CustomerDB.xml");
	//if file is not found or didn't parse properly create CustomerDB.xml file in given format
	if (!file_opened)
	{
		newfile = 1;
		tinyxml2::XMLDocument xmldocument;
		xmldocument.LinkEndChild(xmldocument.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\""));
		auto htmlElement = xmldocument.NewElement("Root");
		htmlElement->SetAttribute("xmlns", "http://www.adventure-works.com");
		xmlParse.dataElement = xmldocument.NewElement("data");
		htmlElement->LinkEndChild(xmlParse.dataElement);
		xmldocument.LinkEndChild(htmlElement);
		tinyxml2::XMLPrinter printer;
		xmldocument.Print(&printer);
		xmldocument.SaveFile("CustomerDB.xml");
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCustomerManagementDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCustomerManagementDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*********************************************************************************************
 * Take id information from id list edit and find its place from xml file update values which*
 * entered to edit boxes and also update it in the list control								 *
 *********************************************************************************************/
void CCustomerManagementDlg::OnBnClickedChange()
{
	char id[5], name[20], subname[20], tel[15], address[50];
	GetDlgItemText(IDC_ID, id, 16);
	CString ID = id;
	GetDlgItemText(IDC_NAME, name, 16);
	CString NAME = name;
	GetDlgItemText(IDC_SUBNAME, subname, 16);
	CString SubName = subname;
	GetDlgItemText(IDC_TEL, tel, 16);
	CString TEL = tel;
	GetDlgItemText(IDC_ADDRESS, address, 16);
	CString Adres = address;
	//go to first customer element and take its id value
	tinyxml2::XMLElement* newElement = xmlParse.xmlDoc.RootElement();
	tinyxml2::XMLElement* pItems = newElement->FirstChildElement("data");
	tinyxml2::XMLElement* pItem = pItems->FirstChildElement("customer");
	CString idd = pItem->ToElement()->Attribute("id");
	int num = 0;
	//find entered item from the xml elements according to given id value in the edit control
	while (idd != ID && pItem->NextSiblingElement()) {		//travel elements till the find given element
		pItem = pItem->NextSiblingElement();	//pass the next element
		idd = pItem->ToElement()->Attribute("id");	//take item's id
		num++;		//increase item's index
	}
	if (idd != ID) {
		MessageBox(NULL, "Eleman bulunamadi!!!", MB_ICONWARNING | MB_DEFBUTTON2);
		return;
	}
	//Update list control according to entered values num specifies item's place in list control
	//+2, +3... for its childs places in the list controls 
	//Update xml file according to entered values take first child and insert values by their places
	//by iterating first child element
	tinyxml2::XMLElement* element = pItem->FirstChildElement("name");
	if (NAME != "") {
		m_listCtrl.SetItemText(num * 6 + 2, 0, "Name : " + NAME);
		element->SetText(NAME);
	}
	element = element->NextSiblingElement();
	if (SubName != "") {
		m_listCtrl.SetItemText(num * 6 + 3, 0, "SubName : " + SubName);
		element->SetText(SubName);
	}
	element = element->NextSiblingElement();
	if (TEL != "") {
		m_listCtrl.SetItemText(num * 6 + 4, 0, "Tel : " + TEL);
		element->SetText(TEL);
	}
	element = element->NextSiblingElement();
	if (Adres != "") {
		m_listCtrl.SetItemText(num * 6 + 5, 0, "Address : " + Adres);
		element->SetText(Adres);
	}

	xmlParse.xmlDoc.SaveFile("CustomerDB.xml");
	//delete all edit control contents
	SetDlgItemText(IDC_ID, "");
	SetDlgItemText(IDC_NAME, "");
	SetDlgItemText(IDC_SUBNAME, "");
	SetDlgItemText(IDC_TEL, "");
	SetDlgItemText(IDC_ADDRESS, "");
}

/*********************************************************************************************
 * Take id information from id list edit and finds its place inside xml file and list control*
 * delete it from xml file and list control													 *
 *********************************************************************************************/
void CCustomerManagementDlg::OnBnClickedDelete()
{
	char id[5];
	GetDlgItemText(IDC_ID, id, 16);
	CString ID = id;
	tinyxml2::XMLElement* temp = xmlParse.xmlDoc.FirstChildElement("Root");	//take root element
	tinyxml2::XMLElement* pItems = temp->FirstChildElement("data");		//take data element
	tinyxml2::XMLElement* pItem = pItems->FirstChildElement("customer");	//came to first customer
	CString idd = pItem->ToElement()->Attribute("id");		//take its id
	int num = 0;
	//iterate all child elements till the find given element according to its id
	while (idd != ID && pItem->NextSiblingElement()) {		
		pItem = pItem->NextSiblingElement();	//pass next element
		idd = pItem->ToElement()->Attribute("id");	//take its id to compare given id
		num++;						//increase item's index to trace its place
	}
	if (idd != ID) {
		MessageBox(NULL, "Eleman bulunamadi!!!", MB_ICONWARNING | MB_DEFBUTTON2);
		return;
	}
	xmlParse.deleteNode(pItem, num);
}

/*********************************************************************************************
 * Take all informations from edit controls and create customer node and save end of the
 * xml file and also add new item to end of the list control								 
 *********************************************************************************************/
void CCustomerManagementDlg::OnBnClickedSave()
{
	if (newfile == 1) {
		xmlParse.LoadFromXML("CustomerDB.xml");
		newfile = 0;
	}

	tinyxml2::XMLElement* newElement = xmlParse.xmlDoc.RootElement();	//take root element
	tinyxml2::XMLElement* pItems = newElement->FirstChildElement("data"); //take data element
	char id[5], name[20], subname[20], tel[15], address[100];
	//take values from edit controls and assign them to CString type variables
	GetDlgItemText(IDC_ID, id, 16);
	CString ID = id;
	GetDlgItemText(IDC_NAME, name, 16);
	CString NAME = name;
	GetDlgItemText(IDC_SUBNAME, subname, 16);
	CString SubName = subname;
	GetDlgItemText(IDC_TEL, tel, 16);
	CString TEL = tel;
	GetDlgItemText(IDC_ADDRESS, address, 16);
	CString Adres = address;
	if (ID == "" || NAME == "" || SubName == "" || TEL == "" || Adres == "") {
		MessageBox( NULL, "Lütfen eksik yerleri doldurunuz!!!", 
			MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
		return;
	}
	//Create parent element and set attribute id
	auto customerElement = xmlParse.xmlDoc.NewElement("customer");
	customerElement->SetAttribute("id", ID);
	//Create new child elements according to given values inside of edit controls
	auto nameElement = xmlParse.xmlDoc.NewElement("name");
	nameElement->SetText(NAME);
	auto subElement = xmlParse.xmlDoc.NewElement("subname");
	subElement->SetText(SubName);
	auto telElement = xmlParse.xmlDoc.NewElement("tel");
	telElement->SetText(TEL);
	auto adresElement = xmlParse.xmlDoc.NewElement("address");
	adresElement->SetText(Adres);
	//add child elements to parent customer element
	customerElement->LinkEndChild(nameElement);
	customerElement->LinkEndChild(subElement);
	customerElement->LinkEndChild(telElement);
	customerElement->LinkEndChild(adresElement);
	//add customer element to its parent
	pItems->LinkEndChild(customerElement);
	newElement->LinkEndChild(pItems);
	xmlParse.xmlDoc.LinkEndChild(newElement);
	xmlParse.xmlDoc.SaveFile("CustomerDB.xml");
	//add new element to list control
	m_listCtrl.InsertItem(0, "----------data----------");
	m_listCtrl.InsertItem(1, "id : " + ID);
	m_listCtrl.InsertItem(2, "Name : " + NAME);
	m_listCtrl.InsertItem(3, "SubName : " + SubName);
	m_listCtrl.InsertItem(4, "Tel : " + TEL);
	m_listCtrl.InsertItem(5, "Address : " + Adres);
	
	//clear all edit lists
	SetDlgItemText(IDC_ID, "");
	SetDlgItemText(IDC_NAME, "");
	SetDlgItemText(IDC_SUBNAME, "");
	SetDlgItemText(IDC_TEL, "");
	SetDlgItemText(IDC_ADDRESS, "");
}
