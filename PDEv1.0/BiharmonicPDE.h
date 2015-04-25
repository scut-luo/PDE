#pragma once
#include "afxwin.h"

#include "EasySize.h"
/*
#include <string>
using namespace std;

//Struct Definition

struct Biharmonic_PDE_Data
{
	double xh;			//x轴步长
	double yh;			//y轴步长
	double xLeft;		//x轴的区间左端点
	double xRight;		//x轴的区间右端点
	double yLeft;		//y轴的区间左端点
	double yRight;		//y轴的区间右端点
	double a;			//△^2(u)-a△u+bu=f
	double b;			//△^2(u)-a△u+bu=f
	string gExp;		//边界条件	u(x,y)=g1
	string hExp;		//边界条件	△u(x,y)=g2
	string fExp;		//函数f(x)表达式
	string var_symbol;	//自变量符号

	Biharmonic_PDE_Data() : xh(0),yh(0),xLeft(0),xRight(0),
		yLeft(0),yRight(0),a(0),b(0),var_symbol("xy")
	{

	}
};
*/
// CBiharmonicPDE dialog

class CBiharmonicPDE : public CDialogEx
{
	DECLARE_EASYSIZE	//EasySize
	DECLARE_DYNAMIC(CBiharmonicPDE)

public:
	CBiharmonicPDE(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBiharmonicPDE();

// Dialog Data
	enum { IDD = IDD_BIHARMONIC_PDE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//Calculation Parameters
private:
	double m_a;
	double m_b;
	double m_xh;
	double m_yh;
	double m_xLeft;
	double m_xRight;
	double m_yLeft;
	double m_yRight;
	CString m_g1exp;
	CString m_g2exp;
	CString m_fexp;
	CString m_var;

private:
	BOOL StartCalc();		//开始计算

public:
	afx_msg void OnBnClickedBpdeRun();
	afx_msg void OnClickedBpdeCalcCheck();
};
