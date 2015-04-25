/************************************************************************/
/* 表达式解析                                                           */
/************************************************************************/

#pragma once

#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;

#include "Stack.h"

class Expression
{
public:
	Expression();
	Expression(const std::string& symbol,double precision = 1e-6);
	~Expression();
	int DealString(std::string& exp);		//处理字符串(去除空格)

	//多维情形
public:
	bool TransExp(const std::string& exp,std::string& postexp) const;
	bool CompValue(const std::string& postexp,std::vector<double>& values,double& result) const;

	//一部到位
public:
	bool Computate(const std::string& exp,std::vector<double>& values,double& result);

private:
	int LeftPri(const char op) const;							//获得左运算符优先级
	int RightPri(const char op) const;							//获得右运算符优先级
	bool InOp(const char ch) const;								//判断ch是否为运算符
	int	PrecedeOp(const char op1,const char op2) const;			//比较运算符的优先级 
	bool CharacterFilter(const std::string& exp) const;		//判断表达式是否非法字符 还有括号是否双双配对

private:
	static const double E;		//E的值
	static const double PI;		//圆周率
	double m_eps;				//计算精度
	std::string m_variable_symbol;	//自变量符号
};