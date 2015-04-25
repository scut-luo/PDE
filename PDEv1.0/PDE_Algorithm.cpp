#include "stdafx.h"
#include "PDE_Algorithm.h"

//��ֵ��ת�����ַ���string

static const double eps = 1e-6;

template<class T>
void NumtoString(string& result,const T& t)
{
	ostringstream oss;
	oss<<t;
	result = oss.str();
}

//�ص��ͷ���
bool Biharmonic_PDE(const Biharmonic_PDE_Data& pde,vector<double>& result)
{
	double u,temp;
	string str;

	temp = pde.a*pde.a - 4*pde.b;
	if(temp <= 0)		// a^2-4b <= 0 ������
		return false;
	u = 0.5*(pde.a + sqrt(temp));

	Elliptic_PDE_Data pde_e;
	pde_e.var_symbol = pde.var_symbol;
	pde_e.xh = pde.xh;	pde_e.yh = pde.yh;
	pde_e.xLeft = pde.xLeft;	pde_e.xRight = pde.xRight;
	pde_e.yLeft = pde.yLeft;	pde_e.yRight = pde.yRight;
	pde_e.pExp = "1";
	NumtoString<double>(pde_e.qExp,pde.b/u);
	NumtoString<double>(str,-1/u);	pde_e.fExp = str + "*(" + pde.fExp + ")";
	NumtoString<double>(str,1/u);	str = str + "*(" + pde.hExp + ")" + "-(" + pde.gExp + ")";
	pde_e.xLeftExp = pde_e.xRightExp = pde_e.yLeftExp = pde_e.yRightExp = str;

	vector<double> result_t;
	//�������Բ�η��̵ķ������
	if(!Elliptic_PDE(pde_e,result_t))
		return false;


	{
		NumtoString<double>(pde_e.qExp,u);
		pde_e.xLeftExp = pde_e.xRightExp = pde_e.yLeftExp = pde_e.yRightExp = pde.gExp;
		for(vector<double>::size_type i=0;i<result_t.size();i++)
			result_t.at(i) = result_t.at(i) * (-u);
		
		int dim,M,N;
		string exp,postexp_xL,postexp_xR,postexp_yL,postexp_yR,
			postexp_p,postexp_q;
		Expression exp_calc(pde_e.var_symbol);

		M = (int)((pde.xRight - pde.xLeft)/pde.xh);
		N = (int)((pde.yRight - pde.yLeft)/pde.yh);

		dim = (N-1)*(M-1);
		Matrix<double> matrix = Matrix<double>(dim,dim+1,0.0);		//�������
		result = vector<double>(dim);
		result.clear();
		
		//��ʼ������
		exp = pde_e.xLeftExp;
		exp_calc.DealString(exp);
		exp_calc.TransExp(exp,postexp_xL);
		exp = pde_e.xRightExp;
		exp_calc.DealString(exp);
		exp_calc.TransExp(exp,postexp_xR);
		exp = pde_e.yLeftExp;
		exp_calc.DealString(exp);
		exp_calc.TransExp(exp,postexp_yL);
		exp = pde_e.yRightExp;
		exp_calc.DealString(exp);
		exp_calc.TransExp(exp,postexp_yR);
		exp = pde_e.pExp;
		exp_calc.DealString(exp);
		exp_calc.TransExp(exp,postexp_p);
		exp = pde_e.qExp;
		exp_calc.DealString(exp);
		exp_calc.TransExp(exp,postexp_q);

		//�����������
		vector<double> values(2);
		int pos,level;
		double xi,yj;
		double ftemp,r1,r2,r3,r4,temp,sum;
		for(int j=1;j<=N-1;j++)
		{
			for(int i=1;i<=M-1;i++)
			{
				level = (i-1) + (j-1)*(M-1);		//�ж�u(i,j)��Ӧϵ������ĵڼ���(��0��ʼ)

				//����f(i,j) �ɽ����һ����Բ�ͷ��̵Ľ���
				ftemp = result_t.at(level);
				/*
				xi = pde.xLeft + i*pde.xh;
				yj = pde.yLeft + j*pde.yh;
				values.at(0) = xi;	values.at(1) = yj;
				if(!exp_calc.CompValue(postexp_f,values,ftemp))
					return false;
				*/

				//��(i,j)��Χ����з���
				for(int k=1;k<=4;k++)
				{
					switch(k)
					{
					case  1:		//(i+1,j)��
						//����p(i+1/2,j)
						xi = pde.xLeft + (i+0.5)*pde.xh;
						yj = pde.yLeft + j*pde.yh;
						values.at(0) = xi;	values.at(1) = yj;
						if(!exp_calc.CompValue(postexp_p,values,r1))
							return false;

						r1 = r1 / (pde.xh*pde.xh);
						if(i+1 == M)		//����ڱ߽�
						{
							//����u(M,j)
							xi = pde.xLeft + M*pde.xh;
							yj = pde.yLeft + j*pde.yh;
							values.at(0) = xi;	values.at(1) = yj;
							if(!exp_calc.CompValue(postexp_yR,values,temp))
								return false;
							ftemp += temp*r1;
						}
						else	//������ڱ߽�
						{
							pos = (i+1-1) + (j-1)*(M-1);
							matrix(level,pos) = -r1;
						}
						break;

					case 2:		//(i,j+1)��
						//����p(i,j+1/2)
						xi = pde.xLeft + i*pde.xh;
						yj = pde.yLeft + (j+0.5)*pde.yh;
						values.at(0) = xi;	values.at(1) = yj;
						if(!exp_calc.CompValue(postexp_p,values,r2))
							return false;

						r2 = r2 / (pde.yh*pde.yh);
						if(j+1 == N)		//����ڱ߽���
						{
							//����u(i,j+1)
							xi = pde.xLeft + i*pde.xh;
							yj = pde.yLeft + N*pde.yh;
							values.at(0) = xi;	values.at(1) = yj;
							if(!exp_calc.CompValue(postexp_xR,values,temp))
								return false;

							ftemp += temp*r2;
						}
						else
						{
							pos = (i-1) + (j+1-1)*(M-1);
							matrix(level,pos) = -r2;
						}
						break;

					case 3:		//����(i-1,j)��
						//����p(i-1/2,j)
						xi = pde.xLeft + (i-0.5)*pde.xh;
						yj = pde.yLeft + j*pde.yh;
						values.at(0) = xi;	values.at(1) = yj;
						if(!exp_calc.CompValue(postexp_p,values,r3))
							return false;

						r3 = r3 / (pde.xh*pde.xh);
						if(i-1 == 0)
						{
							//����u(i-1,j)
							xi = pde.xLeft;
							yj = pde.yLeft + j*pde.yh;
							values.at(0) = xi;	values.at(1) = yj;
							if(!exp_calc.CompValue(postexp_yL,values,temp))
								return false;
							ftemp += temp*r3;
						}
						else
						{
							pos = (i-1-1) + (j-1)*(M-1);
							matrix(level,pos) = -r3;
						}
						break;

					case 4:		//����(i,j-1)
						//����p(i,j-1/2)
						xi = pde.xLeft + i*pde.xh;
						yj = pde.yLeft + (j-0.5)*pde.yh;
						values.at(0) = xi;	values.at(1) = yj;
						if(!exp_calc.CompValue(postexp_p,values,r4))
							return false;

						r4 = r4 / (pde.yh*pde.yh);
						if(j-1 == 0)
						{
							//����u(i,j-1)
							xi = pde.xLeft + i*pde.xh;
							yj = pde.yLeft;
							values.at(0) = xi;	values.at(1) = yj;
							if(!exp_calc.CompValue(postexp_xL,values,temp))
								return false;
							ftemp += temp*r4;
						}
						else
						{
							pos = (i-1) + (j-1-1)*(M-1);
							matrix(level,pos) = -r4;
						}
						break;
					}
				}

				//����q(i,j)
				xi = pde.xLeft + i*pde.xh;
				yj = pde.yLeft + j*pde.yh;
				values.at(0) = xi;	values.at(1) = yj;
				if(!exp_calc.CompValue(postexp_q,values,temp))
					return false;
				sum = r1+r2+r3+r4;
				pos = (i-1) + (j-1)*(M-1);
				matrix(level,pos) = sum + temp;
				matrix(level,dim) = ftemp;
			}
		}

		//ʹ��Gauss���㷽����
		if(!Gauss(matrix,result))
			return false;
	}
	return true; 
}

//��Բ�η���
bool Elliptic_PDE(const Elliptic_PDE_Data& pde,vector<double>& result)
{
	int dim,M,N;
	string exp,postexp_xL,postexp_xR,postexp_yL,postexp_yR,
		postexp_p,postexp_q,postexp_f;
	Expression exp_calc(pde.var_symbol);

	if(!(pde.xRight>pde.xLeft && pde.xh>0))
		return false;
	if(!(pde.yRight>pde.yLeft && pde.yh>0))
		return false;
	M = (int)((pde.xRight - pde.xLeft)/pde.xh);
	N = (int)((pde.yRight - pde.yLeft)/pde.yh);

	dim = (N-1)*(M-1);
	Matrix<double> matrix = Matrix<double>(dim,dim+1,0.0);		//�������
	result = vector<double>(dim);		//����result������С
	result.clear();

	//��ʼ������
	exp = pde.xLeftExp;
	exp_calc.DealString(exp);
	exp_calc.TransExp(exp,postexp_xL);
	exp = pde.xRightExp;
	exp_calc.DealString(exp);
	exp_calc.TransExp(exp,postexp_xR);
	exp = pde.yLeftExp;
	exp_calc.DealString(exp);
	exp_calc.TransExp(exp,postexp_yL);
	exp = pde.yRightExp;
	exp_calc.DealString(exp);
	exp_calc.TransExp(exp,postexp_yR);
	exp = pde.pExp;
	exp_calc.DealString(exp);
	exp_calc.TransExp(exp,postexp_p);
	exp = pde.qExp;
	exp_calc.DealString(exp);
	exp_calc.TransExp(exp,postexp_q);
	exp = pde.fExp;
	exp_calc.DealString(exp);
	exp_calc.TransExp(exp,postexp_f);

	//�����������
	vector<double> values(2);
	int pos,level;
	double xi,yj;
	double ftemp,r1,r2,r3,r4,temp,sum;
	for(int j=1;j<=N-1;j++)
	{
		for(int i=1;i<=M-1;i++)
		{
			level = (i-1) + (j-1)*(M-1);		//�ж�u(i,j)��Ӧϵ������ĵڼ���(��0��ʼ)
			
			//����f(i,j)
			xi = pde.xLeft + i*pde.xh;
			yj = pde.yLeft + j*pde.yh;
			values.at(0) = xi;	values.at(1) = yj;
			if(!exp_calc.CompValue(postexp_f,values,ftemp))
				return false;

			//��(i,j)��Χ����з���
			for(int k=1;k<=4;k++)
			{
				switch(k)
				{
				case  1:		//(i+1,j)��
					//����p(i+1/2,j)
					xi = pde.xLeft + (i+0.5)*pde.xh;
					yj = pde.yLeft + j*pde.yh;
					values.at(0) = xi;	values.at(1) = yj;
					if(!exp_calc.CompValue(postexp_p,values,r1))
						return false;

					r1 = r1 / (pde.xh*pde.xh);
					if(i+1 == M)		//����ڱ߽�
					{
						//����u(M,j)
						xi = pde.xLeft + M*pde.xh;
						yj = pde.yLeft + j*pde.yh;
						values.at(0) = xi;	values.at(1) = yj;
						if(!exp_calc.CompValue(postexp_yR,values,temp))
							return false;
						ftemp += temp*r1;
					}
					else	//������ڱ߽�
					{
						pos = (i+1-1) + (j-1)*(M-1);
						matrix(level,pos) = -r1;
					}
					break;

				case 2:		//(i,j+1)��
					//����p(i,j+1/2)
					xi = pde.xLeft + i*pde.xh;
					yj = pde.yLeft + (j+0.5)*pde.yh;
					values.at(0) = xi;	values.at(1) = yj;
					if(!exp_calc.CompValue(postexp_p,values,r2))
						return false;

					r2 = r2 / (pde.yh*pde.yh);
					if(j+1 == N)		//����ڱ߽���
					{
						//����u(i,j+1)
						xi = pde.xLeft + i*pde.xh;
						yj = pde.yLeft + N*pde.yh;
						values.at(0) = xi;	values.at(1) = yj;
						if(!exp_calc.CompValue(postexp_xR,values,temp))
							return false;

						ftemp += temp*r2;
					}
					else
					{
						pos = (i-1) + (j+1-1)*(M-1);
						matrix(level,pos) = -r2;
					}
					break;

				case 3:		//����(i-1,j)��
					//����p(i-1/2,j)
					xi = pde.xLeft + (i-0.5)*pde.xh;
					yj = pde.yLeft + j*pde.yh;
					values.at(0) = xi;	values.at(1) = yj;
					if(!exp_calc.CompValue(postexp_p,values,r3))
						return false;

					r3 = r3 / (pde.xh*pde.xh);
					if(i-1 == 0)
					{
						//����u(i-1,j)
						xi = pde.xLeft;
						yj = pde.yLeft + j*pde.yh;
						values.at(0) = xi;	values.at(1) = yj;
						if(!exp_calc.CompValue(postexp_yL,values,temp))
							return false;
						ftemp += temp*r3;
					}
					else
					{
						pos = (i-1-1) + (j-1)*(M-1);
						matrix(level,pos) = -r3;
					}
					break;

				case 4:		//����(i,j-1)
					//����p(i,j-1/2)
					xi = pde.xLeft + i*pde.xh;
					yj = pde.yLeft + (j-0.5)*pde.yh;
					values.at(0) = xi;	values.at(1) = yj;
					if(!exp_calc.CompValue(postexp_p,values,r4))
						return false;

					r4 = r4 / (pde.yh*pde.yh);
					if(j-1 == 0)
					{
						//����u(i,j-1)
						xi = pde.xLeft + i*pde.xh;
						yj = pde.yLeft;
						values.at(0) = xi;	values.at(1) = yj;
						if(!exp_calc.CompValue(postexp_xL,values,temp))
							return false;
						ftemp += temp*r4;
					}
					else
					{
						pos = (i-1) + (j-1-1)*(M-1);
						matrix(level,pos) = -r4;
					}
					break;
				}
			}

			//����q(i,j)
			xi = pde.xLeft + i*pde.xh;
			yj = pde.yLeft + j*pde.yh;
			values.at(0) = xi;	values.at(1) = yj;
			if(!exp_calc.CompValue(postexp_q,values,temp))
				return false;
			sum = r1+r2+r3+r4;
			pos = (i-1) + (j-1)*(M-1);
			matrix(level,pos) = sum + temp;
			matrix(level,dim) = ftemp;
		}
	}

	/*
	//����������ļ�
	ofstream outf("out_ellipptic.txt");
	streambuf *default_buf = cout.rdbuf();
	cout.rdbuf(outf.rdbuf());
	cout<<matrix;
	cout.rdbuf(default_buf);
	*/
	//ʹ��Gauss���㷽����
	if(!Gauss(matrix,result))
		return false;
	return true;
}