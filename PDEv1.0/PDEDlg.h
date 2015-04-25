
// PDEDlg.h : header file
//

#pragma once
#include "afxcmn.h"

#include "BiharmonicPDE.h"
#include "ParabolicPDE.h"
#include "EasySize.h"	//EasySize

// CPDEDlg dialog
class CPDEDlg : public CDialogEx
{
	DECLARE_EASYSIZE
// Construction
public:
	CPDEDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PDE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	afx_msg void OnHelpAbout();

private:
	CParabolicPDE m_ParabolicPDE;
	CBiharmonicPDE m_BiharmonicPDE;
public:
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnToolsCalc();
};
