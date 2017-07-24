#pragma once


// CCreateBox dialog
#include "TopoDs_Shape.hxx"


#include <AIS_InteractiveContext.hxx>
class CCreateBox : public CDialog




{
	DECLARE_DYNAMIC(CCreateBox)

public:
	bool clicked;
	CCreateBox(Handle_AIS_InteractiveContext,CWnd* pParent = NULL);   // standard constructor
	virtual ~CCreateBox();
	CEdit m_edtHight;
	CEdit m_edtWidth;
	CEdit m_edtlength;
	CEdit m_editRadius;
	TopoDS_Shape boxxx; 

    double height, length, width;

    void setLength( double len );
    double getLength( void );

	void setHeight( double hei );
    double getHeight( void );

	void setWidth( double wid );
    double getWidth( void );
  
  

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

	 



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	 Handle(V3d_Viewer) myViewer;
  Handle(AIS_InteractiveContext) myAISContext;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEditRadius();
};
