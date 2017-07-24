// CreateBox.cpp : implementation file
//

#include "stdafx.h"
#include "OCCAPP.h"
#include "CreateBox.h"
#include "afxdialogex.h"
#include "AIS_TexturedShape.hxx"
#include "AIS_InteractiveObject.hxx"
#include <BRepPrimAPI_MakeBox.hxx>
#include"BRepFilletAPI_MakeFillet.hxx"
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"
#include "OCCAPPDoc.h"


// CCreateBox dialog

IMPLEMENT_DYNAMIC(CCreateBox, CDialog)

CCreateBox::CCreateBox(Handle_AIS_InteractiveContext myContext, CWnd* pParent /*=NULL*/)
	: CDialog(CCreateBox::IDD, pParent)
{
	myAISContext = myContext;
	clicked = false;


}

CCreateBox::~CCreateBox()
{
	//CCreateBox nn();
}

void CCreateBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_Hight, m_edtHight);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_edtWidth);
	DDX_Control(pDX, IDC_EDIT_Length, m_edtlength);
	DDX_Control(pDX,IDC_EDIT4,m_editRadius);
}


BEGIN_MESSAGE_MAP(CCreateBox, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &CCreateBox::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &CCreateBox::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT2, &CCreateBox::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT4, &CCreateBox::OnEnChangeEdit4)
	//ON_EN_CHANGE(IDC_EDIT_Radius, &CCreateBox::OnEnChangeEditRadius)
END_MESSAGE_MAP()


// CCreateBox message handlers


void CCreateBox::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.



	// TODO:  Add your control notification handler code here
}


void CCreateBox::OnBnClickedOk()
{

	 CString str;
	 clicked = true;

	m_edtlength.GetWindowText(str);
	double length = _wtof(str);
    setLength(length);

	m_edtHight.GetWindowText(str);
    double height = _wtof(str);
	setHeight(height);

     m_edtWidth.GetWindowText(str);
	 double width =_wtof(str);
	 setWidth(width);

     m_editRadius.GetWindowText(str);
	 double radius =  _wtof(str);

	 if((length<=0) || (height<=0) || (width<=0))
	 {
	 	 	AfxMessageBox(_T("The Values you entered are not Valid,\nplease enter some valid Inputs"));
		    m_edtlength.SetWindowText(NULL);
		    m_edtHight.SetWindowText(NULL);
		    m_edtWidth.SetWindowText(NULL);
		    m_editRadius.SetWindowText(NULL);

	 } else	if ((radius>=length)||(radius>=height)||(radius>=width))
	 {
	 	  	 	AfxMessageBox(_T("Value of the Radius should be below than lenght,height and width"));
				m_editRadius.SetWindowText(NULL);
	 }else
	 
	 {
	 
     BRepPrimAPI_MakeBox B(height,length,width);
	 boxxx = B.Shape();

	 if(radius>0)
	 {

		 BRepFilletAPI_MakeFillet mkFillet(B.Shape());

		 TopoDS_Edge anEdge;
		 TopExp_Explorer anEdgeExplorer(B, TopAbs_EDGE);
		while(anEdgeExplorer.More())
		{
			TopoDS_Edge anEdge = TopoDS::Edge(anEdgeExplorer.Current());
			//Add edge to fillet algorithm
			mkFillet.Add(radius, anEdge);
			anEdgeExplorer.Next();	 
		}
	
		boxxx = mkFillet.Shape();
	 }

	 Handle(AIS_InteractiveObject) obj = (new AIS_Shape(boxxx));
	 myAISContext->Display(obj);
     COCCAPPDoc:: Fit();

	// TODO: Add your control notification handler code here
	
	CDialog::OnOK();
	 }
}

/* ====== Getters and Setters ====== */

// SET LENGTH
void CCreateBox::setLength( double len ) {
   length = len;
}
// GET LENGTH 
double CCreateBox::getLength( void ) {
   return length;
}
//SET HEIGHT
void CCreateBox::setHeight( double hei ) {
	height = hei;
}
// GET HEIGHT 
double CCreateBox::getHeight( void ) {
   return height;
}

//SET WIDTH
void CCreateBox::setWidth( double wid ) {
	width = wid;
}
// GET WIDTH
double CCreateBox::getWidth( void ) {
   return width;
}




void CCreateBox::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}


void CCreateBox::OnEnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}


void CCreateBox::OnEnChangeEditRadius()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}
