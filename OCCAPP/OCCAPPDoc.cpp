
// OCCAPPDoc.cpp : implementation of the COCCAPPDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OCCAPP.h"
#endif
#include "V3d_Viewer.hxx"
#include "OCCAPPDoc.h"
#include "CreateBox.h"
#include "DrilledBox.h"
#include <propkey.h>
#include "AIS_TexturedShape.hxx"
#include "AIS_InteractiveObject.hxx"
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include "GC_MakeSegment.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_Transform.hxx"
#include "BRepBuilderAPI_MakeWire.hxx"
#include "TopoDS.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepPrimAPI_MakePrism.hxx"
#include "BRepFilletAPI_MakeFillet.hxx"
#include "TopExp_Explorer.hxx"
#include "BRepAlgoAPI_Cut.hxx"
#include "BrepTools.hxx"
#include "OCCAPPView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COCCAPPDoc
//CCreateBox* Boxdlg; 
IMPLEMENT_DYNCREATE(COCCAPPDoc, CDocument)

BEGIN_MESSAGE_MAP(COCCAPPDoc, CDocument)
ON_COMMAND(ID_BUTTONBox,OnBox)
ON_COMMAND(ID_BUTTONBox2,OnBox2)
ON_COMMAND(ID_BUTTON_Drilled,OnDrilledBox)
ON_COMMAND(ID_FILE_NEW,OnNew)
ON_COMMAND(ID_BUTTON_Volute,OnVolute)
END_MESSAGE_MAP()


// COCCAPPDoc construction/destruction

COCCAPPDoc::COCCAPPDoc()
{
	Boxdlg = NULL;

	Handle(Graphic3d_GraphicDriver) aGraphicDriver = ((COCCAPPApp*)AfxGetApp())->GetGraphicDriver();

  myViewer = new V3d_Viewer (aGraphicDriver, Standard_ExtString("Visu3D") );
  myViewer->SetDefaultLights();
  myViewer->SetLightOn();
  myAISContext = new AIS_InteractiveContext (myViewer);
  myAISContext->SetDisplayMode(AIS_Shaded);
  AfxInitRichEdit();
	// TODO: add one-time construction code here
  

}

COCCAPPDoc::~COCCAPPDoc()
{
	if(Boxdlg !=NULL)
		delete(Boxdlg);
}

BOOL COCCAPPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

  void COCCAPPDoc::OnVolute()
	{  
		 
	// document has been modified
	SetModifiedFlag(TRUE);

      Volutedlg = new CVolute(myAISContext);
	  std::string s ="Test";
      Volutedlg->Create( IDD_DIALOG_Volute);
      Volutedlg->ShowWindow(SW_SHOW);
  
	}

	void COCCAPPDoc::OnNew()
	{  
		myAISContext->RemoveAll();
	}


void COCCAPPDoc::Fit()
{
	CMDIFrameWnd *pFrame =  (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =  (CMDIChildWnd *) pFrame->GetActiveFrame();
	COCCAPPView *pView = (COCCAPPView *) pChild->GetActiveView();
	pView->FitAll();
}

/*================SHIFT DRAG EVENT ========== */

void COCCAPPDoc::ShiftDragEvent (const Standard_Integer theMouseX,
                                    const Standard_Integer theMouseY,
                                    const Standard_Integer theState,
                                    const Handle(V3d_View)& theView)
{
  // TheState == -1  button down
  // TheState ==  0  move
  // TheState ==  1  button up

  static Standard_Integer aStartDragX = 0;
  static Standard_Integer aStartDragY = 0;

  if (theState == -1)
  {
    // button down
    aStartDragX = theMouseX;
    aStartDragY = theMouseY;
  }

  if (theState == 0)
  {
    // button up
    myAISContext->ShiftSelect (aStartDragX, aStartDragY,
                               theMouseX, theMouseY,
                               theView);
  }
}

/*==============Drag Event =======================*/

void COCCAPPDoc::DragEvent (const Standard_Integer theMouseX,
                               const Standard_Integer theMouseY,
                               const Standard_Integer theState,
                               const Handle(V3d_View)& theView)
{
  // TheState == -1  button down
  // TheState ==  0  move
  // TheState ==  1  button up

  static Standard_Integer aStartDragX = 0;
  static Standard_Integer aStartDragY = 0;

  switch (theState)
  {
  case -1:
    {
      aStartDragX = theMouseX;
      aStartDragY = theMouseY;
      break;
    }
  case 0:
    {
      myAISContext->UpdateCurrentViewer();
      break;
    }
  case 1:
    {
      myAISContext->Select (aStartDragX, aStartDragY,
                            theMouseX, theMouseY,
                            theView);
      break;
    }
  };
}
//-----------------------------------------------------------------------------------------
void COCCAPPDoc::ShiftInputEvent (const Standard_Integer /*theMouseX*/,
                                     const Standard_Integer /*theMouseY*/,
                                     const Handle(V3d_View)& /*theView*/)
{
  myAISContext->ShiftSelect();
}
  //-----------------------------------------------------------------------------------------
void COCCAPPDoc::InputEvent (const Standard_Integer theMouseX,
                                const Standard_Integer theMouseY,
                                const Handle(V3d_View)& theView)
{
  myAISContext->MoveTo (theMouseX, theMouseY, theView);
  myAISContext->Select();
}
  //-----------------------------------------------------------------------------------------
void COCCAPPDoc::ShiftMoveEvent (const Standard_Integer theMouseX,
                                    const Standard_Integer theMouseY,
                                    const Handle(V3d_View)& theView)
{
  myAISContext->MoveTo (theMouseX, theMouseY, theView);
}
  //-----------------------------------------------------------------------------------------
void COCCAPPDoc::MoveEvent (const Standard_Integer theMouseX,
                               const Standard_Integer theMouseY,
                               const Handle(V3d_View)& theView)
{
  myAISContext->MoveTo (theMouseX, theMouseY, theView);
}
//-----------------------------------------------------------------------------------------
void  COCCAPPDoc::Popup (const Standard_Integer theMouseX,
                            const Standard_Integer theMouseY,
                            const Handle(V3d_View)& theView)
{
  // Base check which context menu to call
  if (!myPopupMenuNumber)
  {
    myAISContext->InitCurrent();
    if (myAISContext->MoreCurrent())
      myPopupMenuNumber=1;
  }

 // CMenu menu;
 // //VERIFY(menu.LoadMenu(IDR_Popup3D));
 // CMenu* pPopup = menu.GetSubMenu(myPopupMenuNumber);

 // ASSERT(pPopup != NULL);
 //  if (myPopupMenuNumber == 1) // more than 1 object.
 // {
 //   bool OneOrMoreInShading = false;
	//for (myAISContext->InitCurrent();myAISContext->MoreCurrent ();myAISContext->NextCurrent ())
 //   if (myAISContext->IsDisplayed(myAISContext->Current(),1)) OneOrMoreInShading=true;
	//if(!OneOrMoreInShading)
 //  	pPopup->EnableMenuItem(5, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
 //  }

 // POINT winCoord = { theMouseX , theMouseY };
 //// Handle(WNT_Window) aWNTWindow=
 //// Handle(WNT_Window)::DownCast(theView->Window());
 // //ClientToScreen ( (HWND)(aWNTWindow->HWindow()),&winCoord);
 //// pPopup->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON,
 //                         winCoord.x,
 //                         winCoord.y,
 //                         AfxGetMainWnd());
}



void COCCAPPDoc::OnBox()
{

	// document has been modified
	SetModifiedFlag(TRUE);

      Boxdlg = new CCreateBox(myAISContext);
	  std::string s ="Test";
      Boxdlg->Create( IDD_DIALOG1);
      Boxdlg->ShowWindow(SW_SHOW);
  
   //   AfxMessageBox(_T("Error Creating Dialog Object"));
   
}

void   COCCAPPDoc::OnDrilledBox()
{	
	/*BRepPrimAPI_MakeBox B1(200.,150.,100.);

	BRepPrimAPI_MakeCylinder C1(40.,50.);

	TopoDS_Shape s1,s2;
	s1 = B1.Shape();
	s2 = C1.Shape();

	BRepAlgoAPI_Cut Drilling( B1.Shape(),C1.Shape());

	Handle(AIS_InteractiveObject) obj1 = (new AIS_Shape(Drilling));
	myAISContext->Display(obj1);*/
	length = Boxdlg->getLength();
	width  = Boxdlg->getWidth();
	height = Boxdlg->getHeight();

	
		SetModifiedFlag(TRUE);

	Boxdlg2 = new DrilledBox( myAISContext);
	  std::string s ="Test";
      Boxdlg2->Create( IDD_DIALOG2);
      Boxdlg2->ShowWindow(SW_SHOW);
	  Boxdlg2->boxxx	  =  Boxdlg->boxxx;
	  Boxdlg2->setLength(length);
	  Boxdlg2->setWidth(width);
      Boxdlg2->setHeight(height);

}



void COCCAPPDoc::OnBox2()
{
 double myWidth,myThickness,myHeight; 
	myWidth = 500;
	myThickness=400;
	myHeight= 400;
 



  // BRepPrimAPI_MakeBox B(myWidth,myThickness,myHeight);
	BRepPrimAPI_MakeBox B(200.,150.,100.);
	TopoDS_Shape sh = B.Shape();

	
	//BRepPrimAPI_MakeBox B(200.,150.,100.);
   	   BRepFilletAPI_MakeFillet mkFillet(B.Shape());

	   TopoDS_Edge anEdge;
	   TopExp_Explorer anEdgeExplorer(B, TopAbs_EDGE);
    while(anEdgeExplorer.More()){
        TopoDS_Edge anEdge = TopoDS::Edge(anEdgeExplorer.Current());
        //Add edge to fillet algorithm
        mkFillet.Add(20 , anEdge);
        anEdgeExplorer.Next();
	}
	
	
	Handle(AIS_InteractiveObject) obj = (new AIS_Shape(mkFillet.Shape()));
	myAISContext->Display(obj);


}



// COCCAPPDoc serialization

void COCCAPPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void COCCAPPDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void COCCAPPDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void COCCAPPDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// COCCAPPDoc diagnostics

#ifdef _DEBUG
void COCCAPPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COCCAPPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COCCAPPDoc commands
