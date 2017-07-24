
// OCCAPPDoc.h : interface of the COCCAPPDoc class
//

#include <AIS_InteractiveContext.hxx>

#pragma once
#include "CreateBox.h"
#include "DrilledBox.h"
#include "Volute.h"

#include <BRepPrimAPI_MakeBox.hxx>
#include<BRepPrimAPI_MakeWedge.hxx>

class COCCAPPDoc : public CDocument
{
protected: // create from serialization only
	COCCAPPDoc();
	DECLARE_DYNCREATE(COCCAPPDoc)
	
private:
	double height;
	double length, width;

	public:
	  virtual void ShiftDragEvent (const Standard_Integer theMouseX,
                               const Standard_Integer theMouseY,
                               const Standard_Integer theState,
                               const Handle(V3d_View)& theView);

	  
  virtual void DragEvent (const Standard_Integer theMouseX,
                          const Standard_Integer theMouseY,
                          const Standard_Integer theState,
                          const Handle(V3d_View)& theView);

  virtual void ShiftInputEvent (const Standard_Integer theMouseX,
                                const Standard_Integer theMouseY,
                                const Handle(V3d_View)& theView);

   virtual void InputEvent (const Standard_Integer theMouseX,
                           const Standard_Integer theMouseY,
                           const Handle(V3d_View)& theView);

     virtual void ShiftMoveEvent (const Standard_Integer theMouseX,
                               const Standard_Integer theMouseY,
                               const Handle(V3d_View)& theView);

	 
  virtual void MoveEvent (const Standard_Integer theMouseX,
                          const Standard_Integer theMouseY,
                          const Handle(V3d_View)& theView);

   virtual void Popup (const Standard_Integer theMouseX,
                      const Standard_Integer theMouseY,
                      const Handle(V3d_View)& theView);

   static void Fit();

public:
	   Handle_AIS_InteractiveContext myAISContext;

protected:
 int myPopupMenuNumber;



// Attributes
public:
	afx_msg void OnBox();
	afx_msg void OnBox2();
	afx_msg void OnDrilledBox();
	afx_msg void OnSimplify();
	afx_msg void OnNew();
	afx_msg void OnVolute();

// Operations
public:
	CCreateBox* Boxdlg;
	DrilledBox* Boxdlg2;
	CVolute*    Volutedlg;

	Handle(AIS_InteractiveContext)& GetAISContext() { return myAISContext; }

  Handle(V3d_Viewer) GetViewer() { return myViewer; }

//DrilledBox* Boxdlg2;
//	Handle(AIS_InteractiveContext)& GetAISContext() { return myAISContext; }
//			   public:
//	 
//  Handle(V3d_Viewer) GetViewer() { return myViewer; }

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation


public:
	virtual ~COCCAPPDoc();
//	CCreateBox* Boxdlg; 



#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	  Handle(V3d_Viewer) myViewer;
 // Handle(AIS_InteractiveContext) myAISContext;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	
#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
