
// PDEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PDE.h"
#include "PDEDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_COMMAND(IDD_ABOUTBOX, &CAboutDlg::OnIddAboutbox)
//	ON_COMMAND(ID_HELP_ABOUT, &CAboutDlg::OnHelpAbout)
END_MESSAGE_MAP()


// CPDEDlg dialog




CPDEDlg::CPDEDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPDEDlg::IDD, pParent)
{
	//Load Icon
	//m_hIcon =AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon =AfxGetApp()->LoadIcon(IDI_ICON_MAIN);
}

void CPDEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CPDEDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_HELP_ABOUT, &CPDEDlg::OnHelpAbout)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CPDEDlg::OnTcnSelchangeTab)
	ON_WM_SIZE()
	ON_COMMAND(ID_TOOLS_CALC, &CPDEDlg::OnToolsCalc)
END_MESSAGE_MAP()

//EasySize
BEGIN_EASYSIZE_MAP(CPDEDlg)
	EASYSIZE(IDC_TAB,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
END_EASYSIZE_MAP

// CPDEDlg message handlers

BOOL CPDEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//插入标签
	CRect tabRect;	//标签控件客户区的位置和大小
	m_tab.InsertItem(0,_T("重调和方程"));
	m_tab.InsertItem(1,_T("抛物型方程"));
	m_BiharmonicPDE.Create(IDD_BIHARMONIC_PDE,&m_tab);
	m_ParabolicPDE.Create(IDD_PARABOLIC_PDE,&m_tab);
	
	m_tab.GetClientRect(&tabRect);		//获取标签控件客户区Rect
	//调整tabRect,使其覆盖范围适合放置标签页
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 22;
	tabRect.bottom -= 1;
	//根据调整好的tabRect放置m_ParabolicPDE子对话框
	m_ParabolicPDE.SetWindowPos(NULL,tabRect.left,tabRect.top,
		tabRect.Width(),tabRect.Height(),SWP_HIDEWINDOW);
	m_BiharmonicPDE.SetWindowPos(NULL,tabRect.left,tabRect.top,
		tabRect.Width(),tabRect.Height(),SWP_SHOWWINDOW);

	INIT_EASYSIZE;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPDEDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPDEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPDEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPDEDlg::OnHelpAbout()
{
	// TODO: Add your command handler code here
	CAboutDlg aboutBox;
	aboutBox.DoModal();
}


void CPDEDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CRect tabRect;		//标签控件客户区的Rect

	//获取标签控件客户区Rect
	m_tab.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 22;
	tabRect.bottom -= 1;

	switch(m_tab.GetCurSel())
	{
	case 0:		//选择"重调和方程"
		m_ParabolicPDE.SetWindowPos(NULL,tabRect.left,tabRect.top,
			tabRect.Width(),tabRect.Height(),SWP_HIDEWINDOW);
		m_BiharmonicPDE.SetWindowPos(NULL,tabRect.left,tabRect.top,
			tabRect.Width(),tabRect.Height(),SWP_SHOWWINDOW);
		break;

	case 1:		//选择"抛物型方程"
		m_ParabolicPDE.SetWindowPos(NULL,tabRect.left,tabRect.top,
			tabRect.Width(),tabRect.Height(),SWP_SHOWWINDOW);
		m_BiharmonicPDE.SetWindowPos(NULL,tabRect.left,tabRect.top,
			tabRect.Width(),tabRect.Height(),SWP_HIDEWINDOW);
		break;
	}
}


void CPDEDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	UPDATE_EASYSIZE;			//顺序不能变
	if(m_tab.m_hWnd != NULL)	//句柄是否有效
	{
		CRect tabRect;
		m_tab.GetClientRect(&tabRect);		//获取标签控件客户区Rect
		//调整tabRect,使其覆盖范围适合放置标签页
		tabRect.left += 1;
		tabRect.right -= 1;
		tabRect.top += 22;
		tabRect.bottom -= 1;
		//根据调整好的tabRect放置子对话框
		switch(m_tab.GetCurSel())
		{
		case 0:		//选择"重调和方程"
			m_ParabolicPDE.SetWindowPos(NULL,tabRect.left,tabRect.top,
				tabRect.Width(),tabRect.Height(),SWP_HIDEWINDOW);
			m_BiharmonicPDE.SetWindowPos(NULL,tabRect.left,tabRect.top,
				tabRect.Width(),tabRect.Height(),SWP_SHOWWINDOW);
			break;

		case 1:		//选择"抛物型方程"
			m_ParabolicPDE.SetWindowPos(NULL,tabRect.left,tabRect.top,
				tabRect.Width(),tabRect.Height(),SWP_SHOWWINDOW);
			m_BiharmonicPDE.SetWindowPos(NULL,tabRect.left,tabRect.top,
				tabRect.Width(),tabRect.Height(),SWP_HIDEWINDOW);
			break;
		}
	}
}


void CPDEDlg::OnToolsCalc()
{
	// TODO: Add your command handler code here
	WinExec("calc.exe",SW_SHOW);
}
