#pragma once

//test
#include <fstream>

#include <sstream>
#include <string>
#include <cmath>
using namespace std;

#include "Matrix.h"
#include "Expression.h"
#include "Matrix_Algorithm.h"

struct Elliptic_PDE_Data
{
	double xh;			//x轴步长
	double yh;			//y轴步长
	double xLeft;		//x轴的区间左端点
	double xRight;		//x轴的区间右端点
	double yLeft;		//y轴的区间左端点
	double yRight;		//y轴的区间右端点
	string xLeftExp;	//边界条件(y固定)
	string xRightExp;	//边界条件(y固定)
	string yLeftExp;	//边界条件(x固定)
	string yRightExp;	//边界条件(x固定)
	string pExp;		//函数p(x)表达式
	string qExp;		//函数q(x)表达式
	string fExp;		//函数f(x)表达式
	string var_symbol;	//自变量符号变量

	Elliptic_PDE_Data() : var_symbol("xy")
	{

	}
};

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

	Biharmonic_PDE_Data() : var_symbol("xy")
	{

	}
};

//重调和方程
bool Biharmonic_PDE(const Biharmonic_PDE_Data& pde,vector<double>& result);
//椭圆形方程
bool Elliptic_PDE(const Elliptic_PDE_Data& pde,vector<double>& result);