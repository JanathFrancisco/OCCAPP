
// OCCAPPView.h : interface of the COCCAPPView class
//

#pragma once
#include "V3d_View.hxx"
#include "AIS_RubberBand.hxx"


enum CurAction3d { 
  CurAction3d_Nothing,
  CurAction3d_DynamicZooming,
  CurAction3d_WindowZooming,
  CurAction3d_DynamicPanning,
  CurAction3d_GlobalPanning,
  CurAction3d_DynamicRotation
};

class COCCAPPView : public CView
{
protected: // create from serialization only
	COCCAPPView();
	DECLARE_DYNCREATE(COCCAPPView)

// Attributes
public:
	COCCAPPDoc* GetDocument() const;

// Operations
public:
	Handle(V3d_View) myView;
 // CurAction3d      myCurrentMode;
  Standard_Integer myWidth;
  Standard_Integer myHeight;
  Standard_Boolean myHlrModeIsOn;
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	void FitAll() {   if ( !myView.IsNull() ) myView->FitAll();  myView->ZFitAll(); };
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~COCCAPPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	  CurAction3d      myCurrentMode;

 protected:

  Quantity_Factor  myCurZoom;
  Standard_Integer myXmin;
  Standard_Integer myYmin;
  Standard_Integer myXmax;
  Standard_Integer myYmax;

  Handle(AIS_RubberBand) myRect; //!< Rubber rectangle for selection



// Generated message map functions
protected:
	afx_msg void OnBUTTONRot();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); //
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point); //
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);	 //
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);	   //
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	//
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);	  //
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);  //
		afx_msg void OnBUTTONZoomProg();

	afx_msg void OnFilePrintPreview();
	//afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};



#ifndef _DEBUG  // debug version in OCCAPPView.cpp
inline COCCAPPDoc* COCCAPPView::GetDocument() const
   { return reinterpret_cast<COCCAPPDoc*>(m_pDocument); }
#endif

