// Volute.cpp : implementation file
//

#include "stdafx.h"
#include "OCCAPP.h"
#include "Volute.h"
#include "afxdialogex.h"
#include "AIS_InteractiveObject.hxx"
#include "AIS_InteractiveContext.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "TopoDS_Wire.hxx"
#include "BRepBuilderAPI_MakeWire.hxx"
#include "OCCAPPDoc.h"
#include "AIS_TexturedShape.hxx"
#include "Geom_BezierCurve.hxx"
#include "gp_Circ.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepOffsetAPI_MakePipe.hxx"
#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <GProp_GProps.hxx>
#include "BRepGProp.hxx"
#include <vector>  
#include "BRepOffsetAPI_ThruSections.hxx"


//Inheritance diagram for GProp_GProps:

// CVolute dialog

IMPLEMENT_DYNAMIC(CVolute, CDialog)

CVolute::CVolute(Handle_AIS_InteractiveContext myContext3,CWnd* pParent /*=NULL*/)
	: CDialog(CVolute::IDD, pParent)
{
	myAISContext = myContext3;
//	clicked = false;

}

CVolute::~CVolute()
{
}

void CVolute::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_heightVolute, m_edtheightVolute);
	DDX_Control(pDX, IDC_EDIT_widthVolute, m_edtwidthVolute);
	DDX_Control(pDX, IDC_EDIT_Theta01, m_edttheta01Volute);
	DDX_Control(pDX, IDC_EDIT_Theta04, m_edttheta02Volute);	 
	DDX_Control(pDX,IDC_EDIT_Fraction, m_edtFraction);

}


BEGIN_MESSAGE_MAP(CVolute, CDialog)
	ON_EN_CHANGE(IDC_EDIT_heightVolute, &CVolute::OnEnChangeEditheightvolute)
	ON_EN_CHANGE(IDC_EDIT_widthVolute, &CVolute::OnEnChangeEditwidthvolute)
	ON_EN_CHANGE(IDC_EDIT_Theta01, &CVolute::OnEnChangeEditTheta01)
	ON_EN_CHANGE(IDC_EDIT_Theta04, &CVolute::OnEnChangeEditTheta04)
	ON_BN_CLICKED(IDOK, &CVolute::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT4, &CVolute::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT_Fraction, &CVolute::OnEnChangeEditFraction)
END_MESSAGE_MAP()




void CVolute::OnBnClickedOk()
{
	 CString str;

	 m_edtheightVolute.GetWindowText(str);
     double heightVolute = _wtof(str);

 	 m_edtwidthVolute.GetWindowText(str);
	 double widthVolute = _wtof(str);

	 m_edttheta01Volute.GetWindowText(str);
	 double theta01Volute = _wtof(str);
	
	 m_edttheta02Volute.GetWindowText(str);
	 double theta02Volute = _wtof(str);

	 m_edtFraction.GetWindowText(str);
	 double start_areaFraction = _wtof(str);
/*
	 double heightVolute = 3;
	 double widthVolute  = 1;
	 double theta01Volute =40.00;
	 double theta02Volute = 40.00;
*/
	 double tan_theta01Volute= tan(theta01Volute*M_PI/180.0);
	 double tan_theta02Volute = tan(theta02Volute*M_PI/180.0);

	 
     Standard_Real ratio_height = heightVolute*0.01;
     Standard_Real top_ratio_height = heightVolute*0.03;


	 gp_Pnt aPnt1( 0, 0, 0); 
	 gp_Pnt aPnt2(-(heightVolute/tan_theta01Volute), heightVolute ,0);
     gp_Pnt aPnt3 (widthVolute,0,0);
     gp_Pnt aPnt4 ((widthVolute+(heightVolute)/tan_theta02Volute),heightVolute,0);
	 gp_Pnt aPnt5 (widthVolute/2,heightVolute+top_ratio_height,0);
	

	 BRepBuilderAPI_MakeEdge LINE01(aPnt1, aPnt2);
	 BRepBuilderAPI_MakeEdge LINE02(aPnt1, aPnt3);
	 BRepBuilderAPI_MakeEdge LINE03(aPnt3, aPnt4);
     
	 BRepBuilderAPI_MakeWire wire_Lines(LINE01, LINE02, LINE03);

	 //Handle(AIS_InteractiveObject) obj3 = (new AIS_Shape(wire));
	 //myAISContext->Display(obj3);
  //   COCCAPPDoc:: Fit();

   TColgp_Array1OfPnt CurvePoles(1,5); 
   gp_Pnt pt = gp_Pnt(-(heightVolute/tan_theta01Volute), heightVolute ,0);
   CurvePoles(1) = pt; 
   pt = gp_Pnt(-((heightVolute + ratio_height)/tan_theta01Volute), heightVolute+ratio_height ,0); 
   CurvePoles(2) = pt; 
   pt = gp_Pnt (widthVolute/2,heightVolute+top_ratio_height,0);
   CurvePoles(3) = pt; 
   pt = gp_Pnt	 ((widthVolute+(heightVolute + ratio_height)/tan_theta02Volute),heightVolute+ratio_height,0);
   CurvePoles(4) = pt; 
   pt = gp_Pnt	    ((widthVolute+(heightVolute)/tan_theta02Volute),heightVolute,0); 
   CurvePoles(5) = pt; 
   Handle(Geom_BezierCurve) E = new Geom_BezierCurve(CurvePoles); 
   TopoDS_Edge Curve = BRepBuilderAPI_MakeEdge(E); 	 

/*
	TopoDS_Compound aRes;
    BRep_Builder aBuilder;
    aBuilder.MakeCompound (aRes);
    aBuilder.Add (aRes, wire_Lines);
    aBuilder.Add (aRes, Curve);
    
	TopoDS_Compound Volute_CrossSection =  aRes;
*/

	wire_Lines.Add(Curve);

	TopoDS_Face Face = BRepBuilderAPI_MakeFace(wire_Lines);

	GProp_GProps system;
	BRepGProp::LinearProperties(Face, system);
	Standard_Real throat_Area = system.Mass();
	Standard_Real start_Area = (throat_Area)*(start_areaFraction/100.00);
	int number_of_crossections = 10.00;

	std::vector <TopoDS_Wire> cross;
	BRepOffsetAPI_ThruSections generator;

		for(int i=0 ; i< number_of_crossections-1; i++)
		 {
			 Standard_Real expected_Area = start_Area + i*(throat_Area - start_Area)/(number_of_crossections-1.00);
			 Standard_Real angle = 360.00/(number_of_crossections-1.00)*i;

			 TopoDS_Wire sub_Wire = makeCross_section(heightVolute,widthVolute,theta01Volute,theta02Volute,expected_Area,angle,throat_Area);
			 cross.push_back(sub_Wire);
			 generator.AddWire(sub_Wire);

		 }
	         cross.push_back(wire_Lines);
	    	 generator.AddWire(wire_Lines);
	    	 generator.Build();

	    	TopoDS_Shape volute = generator.Shape();
			 						  
    	Handle(AIS_InteractiveObject) obj = (new AIS_Shape(volute));
	    myAISContext->Display(obj);
        COCCAPPDoc:: Fit();
/*  
       Handle(AIS_InteractiveObject) obj3 = (new AIS_Shape(E));
	   myAISContext->Display(obj3);
       COCCAPPDoc:: Fit();
*/

	   CDialog::OnOK();
}



TopoDS_Wire CVolute::makeCross_section(double heightVolute,double widthVolute,double theta01Volute,double theta02Volute,
									   double expected_Area,double angle,double throat_Area)
{
     double Tolerance =  0.01*expected_Area; 

	 Standard_Real new_heightVolute = (expected_Area/throat_Area)*heightVolute;
	 
     Standard_Real newratio_height = new_heightVolute*0.01;
     Standard_Real top_newratio_height = new_heightVolute*0.03;


	// Standard_Real upnew_heightVolute =  (expected_Area/throat_Area)*new_heightVolute;

	 double tan_theta01Volute = tan(theta01Volute*M_PI/180.00);
	 double tan_theta02Volute = tan(theta02Volute*M_PI/180.00);

	 gp_Pnt aPnt1( 0, 0, 0); 
	 gp_Pnt new_aPnt2(-(new_heightVolute/tan_theta01Volute), new_heightVolute ,0);
     gp_Pnt aPnt3 (widthVolute,0,0);
     gp_Pnt new_aPnt4 ((widthVolute+(new_heightVolute)/tan_theta02Volute),new_heightVolute,0);
	 gp_Pnt new_aPnt5 (widthVolute/2, new_heightVolute + top_newratio_height,0);

	 
	 BRepBuilderAPI_MakeEdge new_LINE01(aPnt1, new_aPnt2);
	 BRepBuilderAPI_MakeEdge LINE02(aPnt1, aPnt3);
	 BRepBuilderAPI_MakeEdge new_LINE03(aPnt3, new_aPnt4);

     BRepBuilderAPI_MakeWire new_wire_Lines(new_LINE01, LINE02, new_LINE03);

	 TopoDS_Wire basewire = new_wire_Lines.Wire();

	 //***** Drawing new Beziere curve *****//


      double h_up  = heightVolute+ heightVolute*0.03;
	  double h_low = 0;
	  double h_mid = (h_up+h_low)/2.0;  ///(expected_Area*new_heightVolute) /newArea;

  //Standard_Real b1 = (new_heightVolute + (new_heightVolute + top_newratio_height))/2; //bisection calculated by height of new volute and top point of new volute  
 // Standard_Real b2 = (heightVolute + (new_heightVolute + top_newratio_height))/2; //bisection calculated by top point of throat volute and top point of new Volute 

   TColgp_Array1OfPnt new_CurvePoles(1,5); 
   gp_Pnt new_pt = gp_Pnt(-(new_heightVolute/tan_theta01Volute), new_heightVolute ,0);
   new_CurvePoles(1) = new_pt; 
   new_pt = gp_Pnt(-((new_heightVolute+newratio_height)/tan_theta01Volute), new_heightVolute+newratio_height,0); 
   new_CurvePoles(2) = new_pt; 
   new_pt = gp_Pnt (widthVolute/2, new_heightVolute + top_newratio_height,0); /*(widthVolute/2,h_mid,0); */
   new_CurvePoles(3) = new_pt; 
   new_pt = gp_Pnt	 ((widthVolute+(new_heightVolute+newratio_height)/tan_theta02Volute),new_heightVolute+newratio_height,0);
   new_CurvePoles(4) = new_pt; 
   new_pt = gp_Pnt	    ((widthVolute+(new_heightVolute)/tan_theta02Volute),new_heightVolute,0); 
   new_CurvePoles(5) = new_pt; 
   Handle(Geom_BezierCurve) E2 = new Geom_BezierCurve(new_CurvePoles); 
   TopoDS_Edge new_Curve = BRepBuilderAPI_MakeEdge(E2); 

   new_wire_Lines.Add(new_Curve);
   TopoDS_Face Face = BRepBuilderAPI_MakeFace(new_wire_Lines);

   GProp_GProps new_system;
   BRepGProp::LinearProperties(new_wire_Lines,new_system);
   Standard_Real  newArea = new_system.Mass();

 //  MessageBox(newArea);

   if(fabs(expected_Area-newArea)<Tolerance)
   {
		AfxMessageBox(_T("xx"));
		return new_wire_Lines;
   }  else
   {
	  TopoDS_Wire newWire;
      AfxMessageBox(_T("uu"));

	    
	  while( fabs(expected_Area-newArea)>Tolerance)
	{
	   if(expected_Area>newArea)
	   {		  
		
           h_up  = h_up ;
		   h_low = h_mid ;

		   h_mid = ( h_mid + h_up )/2;

	   }else
	   {    		    	
		  h_low = h_low ; 
		  h_up  = h_mid ;

	      h_mid = ( h_mid + h_low )/2;

       }
	  	
   TColgp_Array1OfPnt new_CurvePoles(1,5); 
   gp_Pnt new_pt = gp_Pnt(-(new_heightVolute/tan_theta01Volute), new_heightVolute ,0);
   new_CurvePoles(1) = new_pt; 
   new_pt = gp_Pnt(-((new_heightVolute+newratio_height)/tan_theta01Volute), new_heightVolute+newratio_height,0); 
   new_CurvePoles(2) = new_pt; 
   new_pt = gp_Pnt (widthVolute/2, h_mid,0);
   new_CurvePoles(3) = new_pt; 
   new_pt = gp_Pnt	 ((widthVolute+(new_heightVolute+newratio_height)/tan_theta02Volute),new_heightVolute+newratio_height,0);
   new_CurvePoles(4) = new_pt; 
   new_pt = gp_Pnt	    ((widthVolute+(new_heightVolute)/tan_theta02Volute),new_heightVolute,0); 
   new_CurvePoles(5) = new_pt; 
   Handle(Geom_BezierCurve) E2 = new Geom_BezierCurve(new_CurvePoles); 
   TopoDS_Edge new_Curve = BRepBuilderAPI_MakeEdge(E2); 

  // new_wire_Lines.Add(new_Curve);
   BRepBuilderAPI_MakeWire mkFullWire(basewire);
   mkFullWire.Add(new_Curve);
   TopoDS_Face Face = BRepBuilderAPI_MakeFace(mkFullWire);

   GProp_GProps new_system;
   BRepGProp::LinearProperties(mkFullWire,new_system);
   newArea = new_system.Mass();

   newWire = mkFullWire.Wire();

   }
   return newWire;
	
   }
       
  // return TopoDS_Wire(new_wire_Lines);
   
 }



// CVolute message handlers


void CVolute::OnEnChangeEditheightvolute()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}


void CVolute::OnEnChangeEditwidthvolute()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}


void CVolute::OnEnChangeEditTheta01()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}


void CVolute::OnEnChangeEditTheta04()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}


void CVolute::OnEnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}


void CVolute::OnEnChangeEditFraction()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}
