
// OCCAPP.h : main header file for the OCCAPP application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Standard_Macro.hxx"
#include "Graphic3d_GraphicDriver.hxx"

// COCCAPPApp:
// See OCCAPP.cpp for the implementation of this class
//

class COCCAPPApp : public CWinAppEx
{
public:


	void SetSampleName (const wchar_t* theName);
  //virtual const wchar_t* GetSampleName() const;
	const wchar_t* GetInitDataDir() const;
  void SetSamplePath (const wchar_t* thePath);
	
	COCCAPPApp();
	  Handle(Graphic3d_GraphicDriver) myGraphicDriver;

	  Handle(Graphic3d_GraphicDriver) GetGraphicDriver() const
  {
    return myGraphicDriver;
  }
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CString SampleName;
	CString SamplePath;

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
//	afx_msg void OnBox();
};

extern COCCAPPApp theApp;
