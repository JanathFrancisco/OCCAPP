#pragma once
#include <AIS_InteractiveContext.hxx>
#include "TopoDs_Shape.hxx"
#include "TopoDS_Wire.hxx"
// CVolute dialog

class CVolute : public CDialog
{
	DECLARE_DYNAMIC(CVolute)

public:
	CVolute(Handle_AIS_InteractiveContext,CWnd* pParent = NULL);   // standard constructor
	virtual ~CVolute();
					 
	CEdit m_edtheightVolute;
	CEdit m_edtwidthVolute;
   	CEdit m_edttheta01Volute;
	CEdit m_edttheta02Volute;
	CEdit m_edtFraction;
	CEdit m_edtexitPipeLength;
	CEdit m_edtexitPipeFraction;

	TopoDS_Shape Volute; 
// Dialog Data
	enum { IDD = IDD_DIALOG_Volute };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	  Handle(AIS_InteractiveContext) myAISContext;	  

public: 
	TopoDS_Wire makeCross_section(double heightVolute, double widthVolute, double theta01Volute, double theta02Volute,
		                          double expected_Area,double angle,double throat_Area,double start_Area,double trapeziumArea_startFraction,double trapeziumArea_Throat);   

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditheightvolute();
	afx_msg void OnEnChangeEditwidthvolute();
	afx_msg void OnEnChangeEditTheta01();
	afx_msg void OnEnChangeEditTheta04();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit4();
	

	afx_msg void OnEnChangeEditFraction();
	afx_msg void OnEnChangeEditexitpipelength();
	afx_msg void OnEnChangeEditexitpipefraction();
};
