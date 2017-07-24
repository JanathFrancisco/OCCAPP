// DrilledBox.cpp : implementation file
//

#include "stdafx.h"
#include "OCCAPP.h"
#include "DrilledBox.h"
#include "afxdialogex.h"
#include "AIS_InteractiveObject.hxx"
#include "AIS_InteractiveContext.hxx"
#include "AIS_TexturedShape.hxx"
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include "BRepAlgoAPI_Cut.hxx"
#include "BRepFeat_MakeCylindricalHole.hxx"
#include "BRepTools.hxx"
#include "CreateBox.h"


// DrilledBox dialog

IMPLEMENT_DYNAMIC(DrilledBox, CDialogEx)

DrilledBox::DrilledBox(Handle_AIS_InteractiveContext myContext2,CWnd* pParent /*=NULL*/)
	: CDialogEx(DrilledBox::IDD, pParent)
	//, XAxisRadio(0)
	, isXAxis(false)
{
	myAISContext = myContext2;
	//clicked = false;

}

//DrilledBox::DrilledBox(COCCAPPDoc* myDoc,  Handle_AIS_InteractiveContext myContext2,CWnd* pParent)
//		: CDialogEx(DrilledBox::IDD, pParent)
//{
//	myAISContext = myContext2;
//	//clicked = false;
//	//myDocument = myDoc;
//
//}
DrilledBox::~DrilledBox()
{
}

void DrilledBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_Diameter, m_edtDiameter);
	DDX_Control(pDX, IDC_RADIO2, m_radioXAxis);
	DDX_Control(pDX, IDC_RADIO3, m_radioYaxis);
	DDX_Control(pDX, IDC_RADIO4, m_radioZaxis);

	DDX_Control(pDX, IDC_EDIT_Depth, m_edtDepth);


	/*DDX_Control(pDX, IDC_RADIO2, m_Xaxis);
	DDX_Control(pDX, IDC_RADIO3, m_Yaxis);
	DDX_Control(pDX, IDC_RADIO4, m_Zaxis);*/


}


BEGIN_MESSAGE_MAP(DrilledBox, CDialogEx)
	ON_BN_CLICKED(IDOK, &DrilledBox::OnBnClickedOk)
	//ON_CBN_SELCHANGE(IDC_COMBO2, &DrilledBox::OnCbnSelchangeCombo2)
	ON_EN_CHANGE(IDC_EDIT1, &DrilledBox::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT_Diameter, &DrilledBox::OnEnChangeEditDiameter)
	//ON_CBN_SELCHANGE(IDC_COMBO_Axis, &DrilledBox::OnCbnSelchangeComboAxis)
	
	ON_BN_CLICKED(IDC_RADIO2, &DrilledBox::OnBnClickedRadio2)  // X axis
	ON_BN_CLICKED(IDC_RADIO3, &DrilledBox::OnBnClickedRadio3)  // Y axis
	ON_BN_CLICKED(IDC_RADIO4, &DrilledBox::OnBnClickedRadio4)  // Z axis
	ON_EN_CHANGE(IDC_EDIT2, &DrilledBox::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT_Depth, &DrilledBox::OnEnChangeEditDepth)
END_MESSAGE_MAP()


// DrilledBox message handlers


void DrilledBox::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
   //BRepPrimAPI_MakeBox B1(400.,100.,100
	//double length,width,height;

	CString str;

	//Getting Diameter
	m_edtDiameter.GetWindowText(str);
	double diameter = _wtof(str);

	//Getting Depth
	m_edtDepth.GetWindowText(str);
	double depth = 	_wtof(str);
	 
	TopoDS_Shape DrilledShape;
	

	/* ======= Creating a Cylinder Hole ========= */

	if((diameter<=0)||(depth<=0))
	{
			AfxMessageBox(_T("The Values you entered are not Valid,\nplease enter some valid Inputs"));

			m_edtDiameter.SetWindowText(NULL);
			m_edtDepth.SetWindowText(NULL);

	} else if((m_radioXAxis.GetCheck() == false)&&(m_radioYaxis.GetCheck() == false)&&(m_radioZaxis.GetCheck() == false))
	{
			AfxMessageBox(_T("Please select the axis, need to be drilled !!!"));

	} else if((depth>length)||(depth>width)||(depth>height))
	{
		  AfxMessageBox(_T("Depth value should be below to the lenght,width and height "));
          m_edtDepth.SetWindowText(NULL);

	}else
	{
	     if (m_radioXAxis.GetCheck() == true)
	   {

    Standard_Real featureDiameter =diameter;
	Standard_Real featureDepth = depth;
	gp_Ax1 feature_origin =gp_Ax1(gp_Pnt(0,length/2.0,width/2.0),gp_Dir(1,0,0));
	BRepFeat_MakeCylindricalHole feature_Maker;
	//BRepTools::Write(boxxx,"boxx.brep");
	feature_Maker.Init(boxxx,feature_origin);
	feature_Maker.PerformBlind(featureDiameter,featureDepth);

	DrilledShape = feature_Maker.Shape();

	BRepTools::Write(DrilledShape,"C:/Users/Dell/Desktop/Shapes/xAxis.brep");
	Handle(AIS_InteractiveObject) obj1 = (new AIS_Shape(DrilledShape));
	myAISContext->RemoveAll();
	myAISContext->Display(obj1);

	  

    CDialogEx::OnOK();

	} else if(m_radioYaxis.GetCheck() == true)
	{

	Standard_Real featureDiameter =diameter;
	Standard_Real featureDepth = depth;
	gp_Ax1 feature_origin =gp_Ax1(gp_Pnt(height/2,0,width/2),gp_Dir(0,1,0));
	BRepFeat_MakeCylindricalHole feature_Maker;
	//BRepTools::Write(boxxx,"boxx.brep");
	feature_Maker.Init(boxxx,feature_origin);
	feature_Maker.PerformBlind(featureDiameter,featureDepth);
	

	DrilledShape = feature_Maker.Shape();

	BRepTools::Write(DrilledShape,"C:/Users/Dell/Desktop/Shapes/yAxis.brep");
	Handle(AIS_InteractiveObject) obj1 = (new AIS_Shape(DrilledShape));
	myAISContext->RemoveAll();
	myAISContext->Display(obj1);

    CDialogEx::OnOK(); 

	} else
	{

	Standard_Real featureDiameter =diameter;
	Standard_Real featureDepth = depth;
	gp_Ax1 feature_origin =gp_Ax1(gp_Pnt(height/2,length/2,0),gp_Dir(0,0,1));
	BRepFeat_MakeCylindricalHole feature_Maker;
	//BRepTools::Write(boxxx,"boxx.brep");
	feature_Maker.Init(boxxx,feature_origin);
	feature_Maker.PerformBlind(featureDiameter,featureDepth);

	DrilledShape = feature_Maker.Shape();

	BRepTools::Write(DrilledShape,"C:/Users/Dell/Desktop/Shapes/zAxis.brep");
	Handle(AIS_InteractiveObject) obj1 = (new AIS_Shape(DrilledShape));
	myAISContext->RemoveAll();
	myAISContext->Display(obj1);

	CDialogEx::OnOK();
	}


	//Standard_Real featureDiameter =diameter;
	//gp_Ax1 feature_origin =gp_Ax1(gp_Pnt(height/2,width/2,0),gp_Dir(0,0,1));
	//BRepFeat_MakeCylindricalHole feature_Maker;
	////BRepTools::Write(boxxx,"boxx.brep");
	//feature_Maker.Init(boxxx,feature_origin);
	//feature_Maker.Perform(featureDiameter);

	//DrilledShape = feature_Maker.Shape();

	//Handle(AIS_InteractiveObject) obj1 = (new AIS_Shape(DrilledShape));
	//myAISContext->RemoveAll();
	//myAISContext->Display(obj1);

	//CDialogEx::OnOK();
	}
}

void DrilledBox::setLength( double len ) {
   length = len;
}
	 
void DrilledBox::setHeight( double hei ) {
	height = hei;
}
void DrilledBox::setWidth( double wid ) {
	width = wid;
}
//{
	// TODO: Add your control notification handler code here
//}







void DrilledBox::OnCbnSelchangeCombo2()
{

	// TODO: Add your control notification handler code here
}
	


void DrilledBox::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}


void DrilledBox::OnEnChangeEditDiameter()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}




void DrilledBox::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
}


void DrilledBox::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
}


void DrilledBox::OnBnClickedRadio4()
{
	// TODO: Add your control notification handler code here
}


void DrilledBox::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}


void DrilledBox::OnEnChangeEditDepth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}
