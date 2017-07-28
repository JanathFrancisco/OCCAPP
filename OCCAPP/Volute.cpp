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
#include <BRepTools.hxx>
#include "Math.h"
#include "gp_Ax1.hxx"
#include "gp_Trsf.hxx"
#include "BRepBuilderAPI_Transform.hxx"
#include "TopoDS.hxx"

 

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
	DDX_Control(pDX,IDC_EDIT_exitPipeLength, m_edtexitPipeLength);
	DDX_Control(pDX,IDC_EDIT_exitPipeFraction, m_edtexitPipeFraction);

}


BEGIN_MESSAGE_MAP(CVolute, CDialog)
	ON_EN_CHANGE(IDC_EDIT_heightVolute, &CVolute::OnEnChangeEditheightvolute)
	ON_EN_CHANGE(IDC_EDIT_widthVolute, &CVolute::OnEnChangeEditwidthvolute)
	ON_EN_CHANGE(IDC_EDIT_Theta01, &CVolute::OnEnChangeEditTheta01)
	ON_EN_CHANGE(IDC_EDIT_Theta04, &CVolute::OnEnChangeEditTheta04)
	ON_BN_CLICKED(IDOK, &CVolute::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT4, &CVolute::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT_Fraction, &CVolute::OnEnChangeEditFraction)
	ON_EN_CHANGE(IDC_EDIT_exitPipeLength, &CVolute::OnEnChangeEditexitpipelength)
	ON_EN_CHANGE(IDC_EDIT_exitPipeFraction, &CVolute::OnEnChangeEditexitpipefraction)
END_MESSAGE_MAP()




void CVolute::OnBnClickedOk()
{
	ofstream out;
	out.open("C:/Users/Dell/Desktop/Shapes/areaInit.txt");	

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

	 m_edtexitPipeLength.GetWindowText(str);
	 double exitPipeLength = _wtof(str);

	 m_edtexitPipeFraction.GetWindowText(str);
	 double exitPipe_Fraction = _wtof(str);

	 

	 double tan_theta01Volute= tan(theta01Volute*M_PI/180.0);
	 double tan_theta02Volute = tan(theta02Volute*M_PI/180.0);

	 
     Standard_Real ratio_height = heightVolute*0.1;
     Standard_Real top_ratio_height = heightVolute*0.3;


	 gp_Pnt aPnt1( 0, 0, 0); 
	 gp_Pnt aPnt2(-(heightVolute/tan_theta01Volute), heightVolute ,0);
     gp_Pnt aPnt3 (widthVolute,0,0);
     gp_Pnt aPnt4 ((widthVolute+(heightVolute)/tan_theta02Volute),heightVolute,0);
	 gp_Pnt aPnt5 (widthVolute/2,heightVolute+top_ratio_height,0);
	

	 BRepBuilderAPI_MakeEdge LINE01(aPnt1, aPnt2);
	 BRepBuilderAPI_MakeEdge LINE02(aPnt1, aPnt3);
	 BRepBuilderAPI_MakeEdge LINE03(aPnt3, aPnt4);
	       
	 BRepBuilderAPI_MakeWire wire_Lines(LINE01, LINE02, LINE03);
	  

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
 	wire_Lines.Add(Curve);
	TopoDS_Face Face = BRepBuilderAPI_MakeFace(wire_Lines);

	BRepTools::Write(Face,"C:/Users/Dell/Desktop/Shapes/ThroatSect_Face_crv.brep");

	GProp_GProps system;
	BRepGProp::SurfaceProperties(Face, system);
	Standard_Real throat_Area = system.Mass();	
    gp_Pnt point =system.CentreOfMass();

	out<<"throat area  : "<<throat_Area<<endl;

	out<<"start area fraction  : "<<start_areaFraction<<endl;

	Standard_Real start_Area = (throat_Area)*(start_areaFraction/100.00);

	out<<"start area  : "<<start_Area<<endl;

	Standard_Real trapeziumArea_Throat= ((widthVolute+(widthVolute + heightVolute/tan_theta01Volute + heightVolute/tan_theta02Volute))*heightVolute)/2 ;

	out<<"trapeziumArea_Throat  : "<<trapeziumArea_Throat<<endl;

	Standard_Real trapeziumArea_startFraction =(trapeziumArea_Throat)*(start_areaFraction/100.00);

	out<<"trapeziumArea_start  : "<<trapeziumArea_startFraction<<endl;
	out.close();

	int number_of_crossections = 10;

	std::vector <TopoDS_Wire> cross;
	BRepOffsetAPI_ThruSections generator;
	BRepOffsetAPI_ThruSections generatorCircle;


	for(int i=0 ; i< number_of_crossections-1; i++)
	 {
		 Standard_Real expected_Area = start_Area + i*(throat_Area - start_Area)/(number_of_crossections-1.00);
		 Standard_Real expected_trapArea = trapeziumArea_startFraction + i*(trapeziumArea_Throat - trapeziumArea_startFraction)/(number_of_crossections-1.00);
			
		 Standard_Real angle = 360.00/(number_of_crossections-1.00)*i;

		 TopoDS_Wire sub_Wire = makeCross_section(heightVolute,widthVolute,theta01Volute,theta02Volute,expected_Area,angle,throat_Area,
				                                      start_Area,expected_trapArea,trapeziumArea_Throat);

		  BRepTools::Write(sub_Wire,"C:/Users/Dell/Desktop/Shapes/sub_Wire.brep");
	    
		 gp_Trsf rotation;
		 gp_Ax1 rotationAxis(gp_Pnt(0.0,-2,0.0),gp_Dir(1.0,0.0,0.0));
		 rotation.SetRotation(rotationAxis, angle*M_PI/180.0); 
		 BRepBuilderAPI_Transform tform(sub_Wire, rotation);

		 TopoDS_Shape myShape = tform.Shape();
	     TopoDS_Wire  myWire = TopoDS::Wire(myShape);
		   BRepTools::Write(myWire,"C:/Users/Dell/Desktop/Shapes/myWire.brep");

		
       	 cross.push_back(myWire);
		 generator.AddWire(myWire);	 
	} 

	cross.push_back(wire_Lines);  
	generator.AddWire(wire_Lines);

	generator.Build();	   
	TopoDS_Shape volute   = generator.Shape();

    BRepTools::Write(volute,"C:/Users/Dell/Desktop/Shapes/voluteshape.brep");

	/*EXIT PIPE*/
		
	Standard_Real radius = Sqrt((throat_Area*(exitPipe_Fraction)/100)/M_PI);
	gp_Dir dir(0,0,1);
	gp_Vec UnitVector(dir);
		
    gp_Pnt centerOfCircle = point;
    centerOfCircle.Translate(UnitVector*exitPipeLength);
    gp_Circ circle(gp_Ax2( centerOfCircle, dir),radius);
    BRepBuilderAPI_MakeEdge makeCircle(circle);
    BRepBuilderAPI_MakeWire exitPipeCircleWire(makeCircle);

    BRepTools::Write(exitPipeCircleWire,"C:/Users/Dell/Desktop/Shapes/exitPipeCircleWire.brep");

    generatorCircle.AddWire(exitPipeCircleWire);
	generatorCircle.AddWire(wire_Lines);
	generatorCircle.Build();
    TopoDS_Shape exitPipe = generatorCircle.Shape();

	BRepTools::Write(exitPipe,"C:/Users/Dell/Desktop/Shapes/exitPipe.brep");
			
    TopoDS_Compound completeShape;
    BRep_Builder aBuilder;
	aBuilder.MakeCompound (completeShape);
	aBuilder.Add(completeShape,volute);
	aBuilder.Add(completeShape,exitPipe);

	BRepTools::Write(completeShape,"C:/Users/Dell/Desktop/Shapes/completeShape.brep");

    Handle(AIS_InteractiveObject) obj3 = (new AIS_Shape(completeShape));
	myAISContext->Display(obj3);
	COCCAPPDoc:: Fit();

    CDialog::OnOK();
}


TopoDS_Wire CVolute::makeCross_section(double heightVolute,double widthVolute,double theta01Volute,double theta02Volute,
									   double expected_Area,double angle,double throat_Area,double start_Area,double trapeziumArea_startFraction,
									   double trapeziumArea_Throat)
{
  ofstream out;
  out.open("C:/Users/Dell/Desktop/Shapes/area.txt");	
	  
  double tan_theta01Volute = tan(theta01Volute*M_PI/180.00);
  double tan_theta02Volute = tan(theta02Volute*M_PI/180.00);
		  

   	
  Standard_Real new_heightVolute = (-widthVolute +(fabs(sqrt(widthVolute*widthVolute+2*trapeziumArea_startFraction*(1/tan_theta01Volute + 1/tan_theta02Volute)))))/(1/tan_theta01Volute + 1/tan_theta02Volute);
  out<<" height       : "<<heightVolute<<endl;
  out<<" width        : "<<widthVolute<<endl;
  out<<"new height    : "<<new_heightVolute<<endl;
  out<<"expected_Area  : "<<expected_Area<<endl;
  out<<"trapeziumArea_start  : "<<trapeziumArea_startFraction<<endl;

  Standard_Real newratio_height = new_heightVolute*0.1;


  double Tolerance =  0.01*expected_Area; 
		 		
  double h_up  =heightVolute+ heightVolute*0.5;	
  double h_low = new_heightVolute;
  double h_mid = (h_up + h_low)/2.0;


  gp_Pnt aPnt1( 0, 0, 0); 
  gp_Pnt new_aPnt2(-(new_heightVolute/tan_theta01Volute), new_heightVolute ,0);
  gp_Pnt aPnt3 (widthVolute,0,0);
  gp_Pnt new_aPnt4 ((widthVolute+(new_heightVolute)/tan_theta02Volute),new_heightVolute,0);
		 
  BRepBuilderAPI_MakeEdge new_LINE01(aPnt1, new_aPnt2);
  BRepBuilderAPI_MakeEdge LINE02(aPnt1, aPnt3);
  BRepBuilderAPI_MakeEdge new_LINE03(aPnt3, new_aPnt4);

  BRepBuilderAPI_MakeWire new_wire_Lines(new_LINE01, LINE02, new_LINE03);
  TopoDS_Wire basewire = new_wire_Lines.Wire();

  BRepTools::Write(basewire,"C:/Users/Dell/Desktop/Shapes/newBaseWire1.brep");

	 /****************************************************/


	 /****************************************************/


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

  BRepBuilderAPI_MakeWire init_wire_Lines(basewire);
  init_wire_Lines.Add(new_Curve);
  TopoDS_Face new_Face = BRepBuilderAPI_MakeFace(init_wire_Lines.Wire());

  BRepTools::Write(new_wire_Lines,"C:/Users/Dell/Desktop/Shapes/newFullWire.brep");

  GProp_GProps new_system;
  BRepGProp::SurfaceProperties(new_Face,new_system);
  Standard_Real  newArea = new_system.Mass();
	 

	 if(fabs(expected_Area-newArea)<Tolerance)
	 {
		
		 return init_wire_Lines;

	 }else
	 {
		 TopoDS_Wire newWire;
	
		 out<<"Throat Area    : "<<throat_Area<<endl;
		 out<<"Start Area     : "<<start_Area<<endl;
		 out<<"Expected Area  : "<<expected_Area<<endl;

		 while(fabs(expected_Area-newArea)>Tolerance)
		 {
			 out<<"Expected Area : "<<expected_Area<<" \t New Area : "<<newArea<<"\t trapezium area throat :"<<trapeziumArea_Throat
				                    <<"\t trapezium area fraction :"<<trapeziumArea_startFraction<<endl;
			  out<<"h low : "<<h_low<<" \t h mid : "<<h_mid<<" \t h up : "<<h_up<<endl;

			 if(expected_Area>newArea)	  
			 {
				
					h_up  = h_up ;
					h_low = h_mid ;

					h_mid = (h_mid + h_up)/2;
			 }else
			 {
					h_low = h_low ; 
				    h_up  = h_mid ;	  

					h_mid = ( h_mid + h_low )/2;

			 } /*ending bracket of else part in 2nd if */

	 out<<"h low : "<<h_low<<" \t h mid : "<<h_mid<<" \t h up : "<<h_up<<endl;	

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

	 BRepTools::Write(basewire,"C:/Users/Dell/Desktop/Shapes/newBaseWire2.brep");

	 BRepBuilderAPI_MakeWire mkFullWire(basewire);
	 mkFullWire.Add(new_Curve);
	 TopoDS_Face Face = BRepBuilderAPI_MakeFace(mkFullWire);
     BRepTools::Write(Face,"C:/Users/Dell/Desktop/Shapes/new_Face_final.brep");
	 
	 GProp_GProps new_system;
	 BRepGProp::SurfaceProperties(Face,new_system);
	 newArea = new_system.Mass();
	 newWire = mkFullWire.Wire();
	 
	 BRepTools::Write(newWire,"C:/Users/Dell/Desktop/Shapes/newFullWire2.brep");

	 } /*ending bracket of while loop*/

		 out.close();
         return newWire;
  } /*ending bracket of else part in 1st if condition*/
  
	   return basewire;
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


void CVolute::OnEnChangeEditexitpipelength()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}


void CVolute::OnEnChangeEditexitpipefraction()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
}
