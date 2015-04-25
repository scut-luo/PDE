#pragma once


// CParabolicPDE dialog
#include "EasySize.h"
#include "afxwin.h"

class CParabolicPDE : public CDialogEx
{
	DECLARE_EASYSIZE	//EasySize
	DECLARE_DYNAMIC(CParabolicPDE)

public:
	CParabolicPDE(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParabolicPDE();

// Dialog Data
	enum { IDD = IDD_PARABOLIC_PDE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	CStatic m_BPDE_pic;
	CStatic m_BPDE_PIC;
};
