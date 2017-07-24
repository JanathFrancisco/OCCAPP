
// OCCAPPView.cpp : implementation of the COCCAPPView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OCCAPP.h"
#endif

#include "OCCAPPDoc.h"
#include "OCCAPPView.h"




#include <Graphic3d_ExportFormat.hxx>
#include <Graphic3d_Camera.hxx>

#include <OpenGl_GraphicDriver.hxx>
#include <WNT_Window.hxx>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define ValZWMin 1

// COCCAPPView

IMPLEMENT_DYNCREATE(COCCAPPView, CView)

BEGIN_MESSAGE_MAP(COCCAPPView, CView)
	// Standard printing commands

	ON_COMMAND(ID_BUTTON_Rotate, OnBUTTONRot)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTONZoomProg, OnBUTTONZoomProg)

	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COCCAPPView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



void COCCAPPView ::OnBUTTONZoomProg() 
{  myCurrentMode = CurAction3d_DynamicZooming; }


void COCCAPPView::OnBUTTONRot() 
{
  myCurrentMode = CurAction3d_DynamicRotation; 
}


void COCCAPPView::OnLButtonDown(UINT nFlags, CPoint point) 
{
    //  save the current mouse coordinate in min 
  myXmin=point.x;  myYmin=point.y;
  myXmax=point.x;  myYmax=point.y;

  if ( nFlags & MK_CONTROL ) 
  {
    // Button MB1 down Control :start zomming 
    // SetCursor(AfxGetApp()->LoadStandardCursor());
  }
  else // if ( Ctrl )
  {
    switch (myCurrentMode)
    {
    case CurAction3d_Nothing : // start a drag
      if (nFlags & MK_SHIFT)
        GetDocument()->ShiftDragEvent(myXmax,myYmax,-1,myView);
      else
        GetDocument()->DragEvent(myXmax,myYmax,-1,myView);
      break;
      break;
    case CurAction3d_DynamicZooming : // noting
      break;
    case CurAction3d_WindowZooming : // noting
      break;
    case CurAction3d_DynamicPanning :// noting
      break;
    case CurAction3d_GlobalPanning :// noting
      break;
    case  CurAction3d_DynamicRotation :
      if (myHlrModeIsOn)
      {
        myView->SetComputedMode (Standard_False);
      }

      myView->StartRotation(point.x,point.y);  
      break;
    default :
      Standard_Failure::Raise(" incompatible Current Mode ");
      break;
    }
  }
}

void COCCAPPView::OnLButtonUp(UINT nFlags, CPoint point) 
{
   if ( nFlags & MK_CONTROL ) 
  {
    return;
  }
  else // if ( Ctrl )
  {
    const Handle(AIS_InteractiveContext)& aContext = GetDocument()->GetAISContext();
    switch (myCurrentMode)
    {
    case CurAction3d_Nothing :
      if (point.x == myXmin && point.y == myYmin)
      { // no offset between down and up --> selectEvent
        myXmax=point.x;  
        myYmax=point.y;
        if (nFlags & MK_SHIFT )
          GetDocument()->ShiftInputEvent(point.x,point.y,myView);
        else
          GetDocument()->InputEvent     (point.x,point.y,myView);
      } else
      {
        myXmax=point.x;    myYmax=point.y;
       // drawRectangle (myXmin, myYmin, myXmax, myYmax, aContext, Standard_False);
        if (nFlags & MK_SHIFT)
          GetDocument()->ShiftDragEvent(point.x,point.y,1,myView);
        else
          GetDocument()->DragEvent(point.x,point.y,1,myView);
      }
      break;
    case CurAction3d_DynamicZooming :
      myCurrentMode = CurAction3d_Nothing;
      break;
    case CurAction3d_WindowZooming :
      myXmax=point.x;        myYmax=point.y;
     // drawRectangle (myXmin, myYmin, myXmax, myYmax, aContext, Standard_False);
      if ((abs(myXmin-myXmax)>ValZWMin) || (abs(myYmin-myYmax)>ValZWMin))
        // Test if the zoom window is greater than a minimale window.
      {
        // Do the zoom window between Pmin and Pmax
        myView->WindowFitAll(myXmin,myYmin,myXmax,myYmax);  
      }  
      myCurrentMode = CurAction3d_Nothing;
      break;
    case CurAction3d_DynamicPanning :
      myCurrentMode = CurAction3d_Nothing;
      break;
    case CurAction3d_GlobalPanning :
      myView->Place(point.x,point.y,myCurZoom); 
      myCurrentMode = CurAction3d_Nothing;
      break;
    case  CurAction3d_DynamicRotation :
      myCurrentMode = CurAction3d_Nothing;
      if (myHlrModeIsOn)
      {
        CWaitCursor aWaitCursor;
        myView->SetComputedMode (myHlrModeIsOn);
      }
      else
      {
        myView->SetComputedMode (myHlrModeIsOn);
      }
      break;
    default :
      Standard_Failure::Raise(" incompatible Current Mode ");
      break;
    } //switch (myCurrentMode)
  } //	else // if ( Ctrl )
}

void COCCAPPView::OnMButtonDown(UINT nFlags, CPoint /*point*/) 
{
   if ( nFlags & MK_CONTROL ) 
  {
    // Button MB2 down Control : panning init  
    // SetCursor(AfxGetApp()->LoadStandardCursor());   
  }
}

void COCCAPPView::OnMButtonUp(UINT nFlags, CPoint /*point*/) 
{
	  if ( nFlags & MK_CONTROL ) 
  {
    // Button MB2 down Control : panning init  
    // SetCursor(AfxGetApp()->LoadStandardCursor());   
  }
}

void COCCAPPView::OnMouseMove(UINT nFlags, CPoint point) 
{
   //   ============================  LEFT BUTTON =======================
  if ( nFlags & MK_LBUTTON)
  {
    if ( nFlags & MK_CONTROL ) 
    {
      // move with MB1 and Control : on the dynamic zooming  
      // Do the zoom in function of mouse's coordinates  
      myView->Zoom (myXmax,myYmax,point.x,point.y); 
      // save the current mouse coordinate in min 
      myXmax = point.x; 
      myYmax = point.y;	
    }
    else // if ( Ctrl )
    {
      const Handle(AIS_InteractiveContext)& aContext = GetDocument()->GetAISContext();
      switch (myCurrentMode)
      {
      case CurAction3d_Nothing :
        myXmax = point.x;
        myYmax = point.y;

        if (nFlags & MK_SHIFT)
          GetDocument()->ShiftDragEvent(myXmax,myYmax,0,myView);
        else
          GetDocument()->DragEvent(myXmax,myYmax,0,myView);

       // drawRectangle (myXmin, myYmin, myXmax, myYmax, aContext);

        break;
      case CurAction3d_DynamicZooming :
        myView->Zoom(myXmax,myYmax,point.x,point.y); 
        // save the current mouse coordinate in min \n";
        myXmax = point.x;
        myYmax = point.y;
        break;
      case CurAction3d_WindowZooming :
        myXmax = point.x;
        myYmax = point.y;
       // drawRectangle (myXmin, myYmin, myXmax, myYmax, aContext);
        break;
      case CurAction3d_DynamicPanning :
        myView->Pan(point.x-myXmax,myYmax-point.y); // Realize the panning
        myXmax = point.x; myYmax = point.y;	
        break;
      case CurAction3d_GlobalPanning : // nothing           
        break;
      case  CurAction3d_DynamicRotation :
        myView->Rotation(point.x,point.y);
        myView->Redraw();
        break;
      default :
        Standard_Failure::Raise(" incompatible Current Mode ");
        break;
      }//  switch (myCurrentMode)
    }// if ( nFlags & MK_CONTROL )  else 
  } 
  else if ( nFlags & MK_MBUTTON)
  {
    if ( nFlags & MK_CONTROL ) 
    {
      myView->Pan(point.x-myXmax,myYmax-point.y); // Realize the panning
      myXmax = point.x;
      myYmax = point.y;
    }
  } 
  else if ( nFlags & MK_RBUTTON)
  {
    if ( nFlags & MK_CONTROL ) 
    {
      myView->Rotation(point.x,point.y);
    }
  }
  else 
  {  // No buttons
    myXmax = point.x;
    myYmax = point.y;
    if (nFlags & MK_SHIFT)
      GetDocument()->ShiftMoveEvent(point.x,point.y,myView);
    else
      GetDocument()->MoveEvent(point.x,point.y,myView);
  }
}


void COCCAPPView::OnRButtonDown(UINT nFlags, CPoint point) 
{
   if ( nFlags & MK_CONTROL )
  {
    if (myHlrModeIsOn)
    {
      myView->SetComputedMode (Standard_False);
    }
    myView->StartRotation(point.x,point.y);  
  }
  else // if ( Ctrl )
  {
    GetDocument()->Popup(point.x,point.y,myView);
  }	
}

void COCCAPPView::OnRButtonUp(UINT /*nFlags*/, CPoint /*point*/) 
{
   SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
    myView->SetComputedMode (myHlrModeIsOn);
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}



// COCCAPPView construction/destruction

COCCAPPView::COCCAPPView()



{

	myCurrentMode = CurAction3d_Nothing;
	// TODO: add construction code here

}

COCCAPPView::~COCCAPPView()
{
}

BOOL COCCAPPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void COCCAPPView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
      myView = GetDocument()->GetViewer()->CreateView();

  // store for restore state after rotation (which is in Degenerated mode)
  myHlrModeIsOn = Standard_False;
  myView->SetComputedMode (myHlrModeIsOn);


  Handle(OpenGl_GraphicDriver) aDriver = 
    Handle(OpenGl_GraphicDriver)::DownCast (((COCCAPPApp*)AfxGetApp())->GetGraphicDriver());

  Handle(WNT_Window) aWNTWindow = new WNT_Window(GetSafeHwnd());

  myView->SetWindow(aWNTWindow);
  myView->Camera()->SetProjectionType (aDriver->Options().contextStereo
    ? Graphic3d_Camera::Projection_Stereo
    : Graphic3d_Camera::Projection_Orthographic);

  if (!aWNTWindow->IsMapped())
  {
    aWNTWindow->Map();
  }

  // store the mode ( nothing , dynamic zooming, dynamic ... )
 /* myCurrentMode = CurAction3d_Nothing;

  m_pStereoDlg = new OCC_StereoConfigDlg (this);
  m_pStereoDlg->SetView (myView);
  m_pStereoDlg->Create (IDD_DIALOG_STEREO, this);*/
}







// COCCAPPView drawing

void COCCAPPView::OnDraw(CDC* /*pDC*/)
{
	 CRect aRect;
  GetWindowRect(aRect);
  if(myWidth != aRect.Width() || myHeight != aRect.Height()) {
    myWidth = aRect.Width();
    myHeight = aRect.Height();
    ::PostMessage ( GetSafeHwnd() , WM_SIZE , SW_SHOW , myWidth + myHeight*65536 );
  }
  myView->Redraw();

	// TODO: add draw code for native data here
}


// COCCAPPView printing


void COCCAPPView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COCCAPPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COCCAPPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COCCAPPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}
//
//void COCCAPPView::OnRButtonUp(UINT /* nFlags */, CPoint point)
//{
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
//}

void COCCAPPView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COCCAPPView diagnostics

#ifdef _DEBUG
void COCCAPPView::AssertValid() const
{
	CView::AssertValid();
}

void COCCAPPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COCCAPPDoc* COCCAPPView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COCCAPPDoc)));
	return (COCCAPPDoc*)m_pDocument;
}
#endif //_DEBUG


// COCCAPPView message handlers
