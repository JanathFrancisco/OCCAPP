#pragma once

#include <AIS_InteractiveContext.hxx>
#include "TopoDS_Shape.hxx"

// DrilledBox dialog

class DrilledBox : public CDialogEx
{
	DECLARE_DYNAMIC(DrilledBox)

public:
	DrilledBox(Handle_AIS_InteractiveContext,CWnd* pParent = NULL);   // standard constructor
	//DrilledBox(COCCAPPDoc *myDoc,  Handle_AIS_InteractiveContext ,CWnd* pParent = NULL);
	
	virtual ~DrilledBox();
		CEdit m_edtDiameter;
		CEdit  m_edtDepth  ;
		CButton m_radioXAxis;
		CButton m_radioYaxis ;
		CButton m_radioZaxis;
		

		/*CEdit m_Xaxis;
		CEdit m_Yaxis;
		CEdit m_Zaxis;*/
	int m_Xaxis;
	int m_Yaxis;
	int m_Zaxis;

	TopoDS_Shape boxxx;

private:
	double length, width, height;
public:
	void setLength( double len );
  //  double getLength( void );

	void setHeight( double hei );
  //  double getHeight( void );

	void setWidth( double wid );
   // double getWidth( void );
  


// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	 Handle(V3d_Viewer) myViewer;
     Handle(AIS_InteractiveContext) myAISContext;
	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList3();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();

	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEditDiameter();
	afx_msg void OnCbnSelchangeComboAxis();
	afx_msg void OnBnClickedRadio1();

	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	
	bool isXAxis;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEditDepth();
};

	  
   