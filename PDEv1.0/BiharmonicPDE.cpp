// BiharmonicPDE.cpp : implementation file
//

#include "stdafx.h"
#include "PDE.h"
#include "BiharmonicPDE.h"
#include "afxdialogex.h"

#include "EnableGroupboxControls.h"
#include "PDE_Algorithm.h"
#include "StringReverse.h"

// CBiharmonicPDE dialog

IMPLEMENT_DYNAMIC(CBiharmonicPDE, CDialogEx)

CBiharmonicPDE::CBiharmonicPDE(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBiharmonicPDE::IDD, pParent)
{
	m_a = m_b = 0;
	m_xLeft = m_yLeft = 0;
	m_xRight = m_yRight = 1;
	m_xh = m_yh = 1;
	m_fexp = m_g1exp = m_g2exp = "0";
	m_var = "xy";
}

CBiharmonicPDE::~CBiharmonicPDE()
{
}

void CBiharmonicPDE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_BPDE_A,m_a);
	DDX_Text(pDX,IDC_BPDE_B,m_b);
	DDX_Text(pDX,IDC_BPDE_F,m_fexp);
	DDX_Text(pDX,IDC_BPDE_G1,m_g1exp);
	DDX_Text(pDX,IDC_BPDE_G2,m_g2exp);
	DDX_Text(pDX,IDC_BPDE_SYMBOL,m_var);
	DDX_Text(pDX,IDC_BPDE_XSTEP,m_xh);
	DDX_Text(pDX,IDC_BPDE_YSTEP,m_yh);
	DDX_Text(pDX,IDC_BPDE_XL,m_xLeft);
	DDX_Text(pDX,IDC_BPDE_XR,m_xRight);
	DDX_Text(pDX,IDC_BPDE_YL,m_yLeft);
	DDX_Text(pDX,IDC_BPDE_YR,m_yRight);
}


BEGIN_MESSAGE_MAP(CBiharmonicPDE, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BPDE_RUN, &CBiharmonicPDE::OnBnClickedBpdeRun)
	ON_BN_CLICKED(IDC_BPDE_CALCEXACT,&CBiharmonicPDE::OnClickedBpdeCalcCheck)
	ON_BN_CLICKED(IDC_BPDE_CALCNUM,&CBiharmonicPDE::OnClickedBpdeCalcCheck)	
END_MESSAGE_MAP()

//EasySize
BEGIN_EASYSIZE_MAP(CBiharmonicPDE)
	EASYSIZE(IDC_BPDE_PIC,ES_BORDER,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,0)
	EASYSIZE(IDC_BPDE_RICHEDIT,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0);
END_EASYSIZE_MAP

// CBiharmonicPDE message handlers


BOOL CBiharmonicPDE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CheckDlgButton(IDC_BPDE_CALCNUM,1);
	SetDlgItemText(IDC_BPDE_ESYMBOL,m_var);
	INIT_EASYSIZE;	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBiharmonicPDE::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	UPDATE_EASYSIZE;
}


void CBiharmonicPDE::OnBnClickedBpdeRun()
{
	//开始计算
	// TODO: Add your control notification handler code here		
	if(!StartCalc())	//开始计算
		MessageBox(_T("计算发生未知错误!"),_T("错误"),MB_ICONERROR);		
}

void CBiharmonicPDE::OnClickedBpdeCalcCheck()
{
	// TODO: Add your control notification handler code here
	CButton *btn_numcheck = (CButton*)GetDlgItem(IDC_BPDE_CALCNUM);
	CButton *btn_exactcheck = (CButton*)GetDlgItem(IDC_BPDE_CALCEXACT);
	if(btn_numcheck->GetCheck() == BST_CHECKED && btn_exactcheck->GetCheck() == BST_CHECKED)
	{
		GetDlgItem(IDC_BPDE_NUMCHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BPDE_EXACTCHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BPDE_MAXERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BPDE_EXACTSOLUTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_BPDE_ESYMBOL)->EnableWindow(TRUE);		
		
		EnableGroupboxControls(::GetDlgItem(m_hWnd,IDC_BPDE_GROUPBOX),TRUE);
	}
	else if(btn_numcheck->GetCheck() == BST_CHECKED && btn_exactcheck->GetCheck() == BST_UNCHECKED)
	{
		GetDlgItem(IDC_BPDE_NUMCHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BPDE_EXACTCHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BPDE_MAXERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BPDE_EXACTSOLUTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_BPDE_ESYMBOL)->EnableWindow(FALSE);
		
		EnableGroupboxControls(::GetDlgItem(m_hWnd,IDC_BPDE_GROUPBOX),TRUE);
	}
	else if(btn_numcheck->GetCheck() == BST_UNCHECKED && btn_exactcheck->GetCheck() == BST_CHECKED)
	{
		GetDlgItem(IDC_BPDE_NUMCHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BPDE_EXACTCHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BPDE_MAXERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BPDE_EXACTSOLUTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_BPDE_ESYMBOL)->EnableWindow(TRUE);

		EnableGroupboxControls(::GetDlgItem(m_hWnd,IDC_BPDE_GROUPBOX),TRUE);
	}
	else
	{
		GetDlgItem(IDC_BPDE_NUMCHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BPDE_EXACTCHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BPDE_MAXERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BPDE_EXACTSOLUTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_BPDE_ESYMBOL)->EnableWindow(FALSE);

		EnableGroupboxControls(::GetDlgItem(m_hWnd,IDC_BPDE_GROUPBOX),FALSE);
	}
}

BOOL CBiharmonicPDE::StartCalc()
{
	vector<double> result_num;		//保存数值解
	vector<double> result_exact;	//保存精确解
	CButton *btn_numcheck = (CButton*)GetDlgItem(IDC_BPDE_CALCNUM);		//计算数值解
	CButton *btn_exactcheck = (CButton*)GetDlgItem(IDC_BPDE_CALCEXACT);	//计算精确解
	
	//判断是否要计算数值解
	if(btn_numcheck->GetCheck() == BST_CHECKED)
	{		
		Biharmonic_PDE_Data pde;

		UpdateData(TRUE);	//获取参数
		
		//构造参数
		pde.a = m_a;
		pde.b = m_b;
		pde.xLeft = m_xLeft;
		pde.xRight = m_xRight;
		pde.yLeft = m_yLeft;
		pde.yRight = m_yRight;
		pde.xh = m_xh;
		pde.yh = m_yh;
		CStringToString(m_fexp,pde.fExp);
		CStringToString(m_g1exp,pde.gExp);
		CStringToString(m_g2exp,pde.hExp);
		CStringToString(m_var,pde.var_symbol);

		if(!Biharmonic_PDE(pde,result_num))
			return FALSE;
	}

	//判断是否要计算精确解
	if(btn_exactcheck->GetCheck() == BST_CHECKED)
	{
		double xLeft,xRight,yLeft,yRight;
		string solution,postexp;
		CString cstr;
		
		GetDlgItemText(IDC_BPDE_EXACTSOLUTION,cstr);		//获取精确解表达式
		CStringToString(cstr,solution);

		//获取区间参数
		GetDlgItemText(IDC_BPDE_XL,cstr);
		xLeft = _ttof(cstr);
		GetDlgItemText(IDC_BPDE_XR,cstr);
		xRight = _ttof(cstr);
		GetDlgItemText(IDC_BPDE_YL,cstr);
		yLeft = _ttof(cstr);
		GetDlgItemText(IDC_BPDE_YR,cstr);
		yRight = _ttof(cstr);

		//
	}
	return TRUE;
}