/************************************************************************/
/* 矩阵类                                                               */
/************************************************************************/

#pragma once
#include <stdexcept>
#include <iostream>
#include <iomanip>
using namespace std;

template<class T>
class Matrix
{
public:
	Matrix(int r = 0 ,int c = 0,T noValue = T());					//构造函数--创建一个r行c列的矩阵
	Matrix(const Matrix<T>& m);						//复制构造函数
	~Matrix();										//析构函数

public:
	T& operator()(int i,int j);						//重载()
	const T& operator()(int i,int j) const;			//重载()
	Matrix<T>& operator=(const Matrix<T>& m);		//重载=
	int get_rows() const { return m_rows; }			//返回矩阵的行数
	int get_columns() const { return m_columns; }   //返回矩阵的列数
private:
	T* m_data;										//保存矩阵数据
	int m_rows;										//矩阵行数
	int m_columns;									//矩阵列数
};


template<class T>
Matrix<T>::Matrix(int r /* = 0  */,int c /* = 0 */,T noValue)
{
	if(r<0 || c<0)
		throw invalid_argument("矩阵大小错误!");
	else if(r == 0 && c == 0)
	{
		m_rows = 0;
		m_columns = 0;
		m_data = 0;
	}
	else
	{
		m_rows = r;
		m_columns = c;
		m_data = new T [r * c];
		for(int i=0;i<r*c;i++)
			m_data[i] = noValue;
	}
}

template<class T>
Matrix<T>::Matrix(const Matrix<T>& m)
{
	m_rows = m.m_rows;
	m_columns = m.m_columns;
	m_data = new T [m_rows * m_columns];  //申请空间

	//复制元素
	for(int i=0;i<m_rows*m_columns;i++)
		m_data[i] = m.m_data[i];
}

template<class T>
Matrix<T>::~Matrix()
{
	delete [] m_data;
}

template<class T>
T& Matrix<T>::operator()(int i,int j)
{
	if(i<0 || i>=m_rows || j<0 || j>=m_columns)
		throw out_of_range("越界访问!"+i+j);
	return m_data[i * m_columns + j];
}

template<class T>
const T& Matrix<T>::operator()(int i,int j) const
{
	if(i<0 || i>=m_rows || j<0 || j>=m_columns)
		throw out_of_range("越界访问!");
	return m_data[i * m_columns + j];
}

template<class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m)
{
	if(this == &m)
		return *this;
	
	T* orig_data = m_data;  //记住原先的m_data
	m_data = new T [m.m_rows * m.m_columns];		//申请空间
	m_rows = m.m_rows;
	m_columns = m.m_columns;
	for(int i=0;i<m_rows * m_columns;i++)
		m_data[i] = m.m_data[i];
	delete [] orig_data;	//删除原先m_data指向的空间

	return *this;
}

//矩阵输出
template<class T>
ostream& operator<<(ostream& os,const Matrix<T>& m)
{
	for(int i=0;i<m.get_rows();i++)
	{
		//cout<<"i="<<i<<"   ";
		for(int j=0;j<m.get_columns();j++)
		{
			os<<setw(5)<<m(i,j);
		}
		os<<"\n";
	}
	return os;
}