#include "stdafx.h"
#include "Matrix_Algorithm.h"

static const double eps = 1e-6;

void ConvertMatrix(Matrix<double>& matrix,const int k)
{
	double max = matrix(k,k),temp;
	int convert = k;
	int n = matrix.get_rows();

	for(int i=k;i<n;i++)
	{
		if(fabs(matrix(i,k)) > fabs(max))
		{
			convert = i;
			max = matrix(convert,i);
		}
	}

	for(int j=k;j<n+1;j++)
	{
		temp = matrix(k,j)	;
		matrix(k,j) = matrix(convert,j);
		matrix(convert,j) = temp;
	}
}

bool Gauss(const Matrix<double>& data,vector<double>& result)
{
	int n;
	double L;
	Matrix<double> matrix = data;	//复制一份矩阵数据

	if(matrix.get_columns() != matrix.get_rows()+1)
		return false;
	n = matrix.get_rows();
	result = vector<double>(n);

	for(int k=0;k<n-1;k++)
	{
		ConvertMatrix(matrix,k);
		if(fabs(matrix(k,k)) < eps)		//矩阵奇异
			return false;
		
		for(int i=k+1;i<n;i++)
		{
			if(fabs(matrix(k,k)) < eps)
				return false;
			L = matrix(i,k) / matrix(k,k);
			for(int j=k;j<n+1;j++)
				matrix(i,j) -= matrix(k,j)*L;
		}
	}

	//回代
	double sum;
	if(fabs(matrix(n-1,n-1)) < eps)
		return false;
	result.at(n-1) = matrix(n-1,n) / matrix(n-1,n-1);
	for(int i=n-2;i>=0;i--)
	{
		sum = 0.0;
		for(int j=i+1;j<n;j++)
			sum += matrix(i,j)*result.at(j);
		if(fabs(matrix(i,i)) < eps)
			return false;
		result.at(i) = (matrix(i,n) - sum) / matrix(i,i);
	}
	return true;
}