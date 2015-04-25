/************************************************************************/
/* ���ʽ����                                                           */
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
	int DealString(std::string& exp);		//�����ַ���(ȥ���ո�)

	//��ά����
public:
	bool TransExp(const std::string& exp,std::string& postexp) const;
	bool CompValue(const std::string& postexp,std::vector<double>& values,double& result) const;

	//һ����λ
public:
	bool Computate(const std::string& exp,std::vector<double>& values,double& result);

private:
	int LeftPri(const char op) const;							//�������������ȼ�
	int RightPri(const char op) const;							//�������������ȼ�
	bool InOp(const char ch) const;								//�ж�ch�Ƿ�Ϊ�����
	int	PrecedeOp(const char op1,const char op2) const;			//�Ƚ�����������ȼ� 
	bool CharacterFilter(const std::string& exp) const;		//�жϱ��ʽ�Ƿ�Ƿ��ַ� ���������Ƿ�˫˫���

private:
	static const double E;		//E��ֵ
	static const double PI;		//Բ����
	double m_eps;				//���㾫��
	std::string m_variable_symbol;	//�Ա�������
};