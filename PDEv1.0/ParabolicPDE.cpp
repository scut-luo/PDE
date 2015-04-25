// ParabolicPDE.cpp : implementation file
//

#include "stdafx.h"
#include "PDE.h"
#include "ParabolicPDE.h"
#include "afxdialogex.h"


// CParabolicPDE dialog

IMPLEMENT_DYNAMIC(CParabolicPDE, CDialogEx)

CParabolicPDE::CParabolicPDE(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParabolicPDE::IDD, pParent)
{

}

CParabolicPDE::~CParabolicPDE()
{
}

void CParabolicPDE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CParabolicPDE, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


//EasySize
BEGIN_EASYSIZE_MAP(CParabolicPDE)
	EASYSIZE(IDC_PPDE_RICHEDIT,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0);
END_EASYSIZE_MAP

// CParabolicPDE message handlers


BOOL CParabolicPDE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	INIT_EASYSIZE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CParabolicPDE::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	UPDATE_EASYSIZE;
}
