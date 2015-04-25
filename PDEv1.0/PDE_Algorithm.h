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
	double xh;			//x�Ჽ��
	double yh;			//y�Ჽ��
	double xLeft;		//x���������˵�
	double xRight;		//x��������Ҷ˵�
	double yLeft;		//y���������˵�
	double yRight;		//y��������Ҷ˵�
	string xLeftExp;	//�߽�����(y�̶�)
	string xRightExp;	//�߽�����(y�̶�)
	string yLeftExp;	//�߽�����(x�̶�)
	string yRightExp;	//�߽�����(x�̶�)
	string pExp;		//����p(x)���ʽ
	string qExp;		//����q(x)���ʽ
	string fExp;		//����f(x)���ʽ
	string var_symbol;	//�Ա������ű���

	Elliptic_PDE_Data() : var_symbol("xy")
	{

	}
};

struct Biharmonic_PDE_Data
{
	double xh;			//x�Ჽ��
	double yh;			//y�Ჽ��
	double xLeft;		//x���������˵�
	double xRight;		//x��������Ҷ˵�
	double yLeft;		//y���������˵�
	double yRight;		//y��������Ҷ˵�
	double a;			//��^2(u)-a��u+bu=f
	double b;			//��^2(u)-a��u+bu=f
	string gExp;		//�߽�����	u(x,y)=g1
	string hExp;		//�߽�����	��u(x,y)=g2
	string fExp;		//����f(x)���ʽ
	string var_symbol;	//�Ա�������

	Biharmonic_PDE_Data() : var_symbol("xy")
	{

	}
};

//�ص��ͷ���
bool Biharmonic_PDE(const Biharmonic_PDE_Data& pde,vector<double>& result);
//��Բ�η���
bool Elliptic_PDE(const Elliptic_PDE_Data& pde,vector<double>& result);