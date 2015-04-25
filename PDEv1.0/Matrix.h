/************************************************************************/
/* ������                                                               */
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
	Matrix(int r = 0 ,int c = 0,T noValue = T());					//���캯��--����һ��r��c�еľ���
	Matrix(const Matrix<T>& m);						//���ƹ��캯��
	~Matrix();										//��������

public:
	T& operator()(int i,int j);						//����()
	const T& operator()(int i,int j) const;			//����()
	Matrix<T>& operator=(const Matrix<T>& m);		//����=
	int get_rows() const { return m_rows; }			//���ؾ��������
	int get_columns() const { return m_columns; }   //���ؾ��������
private:
	T* m_data;										//�����������
	int m_rows;										//��������
	int m_columns;									//��������
};


template<class T>
Matrix<T>::Matrix(int r /* = 0  */,int c /* = 0 */,T noValue)
{
	if(r<0 || c<0)
		throw invalid_argument("�����С����!");
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
	m_data = new T [m_rows * m_columns];  //����ռ�

	//����Ԫ��
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
		throw out_of_range("Խ�����!"+i+j);
	return m_data[i * m_columns + j];
}

template<class T>
const T& Matrix<T>::operator()(int i,int j) const
{
	if(i<0 || i>=m_rows || j<0 || j>=m_columns)
		throw out_of_range("Խ�����!");
	return m_data[i * m_columns + j];
}

template<class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m)
{
	if(this == &m)
		return *this;
	
	T* orig_data = m_data;  //��סԭ�ȵ�m_data
	m_data = new T [m.m_rows * m.m_columns];		//����ռ�
	m_rows = m.m_rows;
	m_columns = m.m_columns;
	for(int i=0;i<m_rows * m_columns;i++)
		m_data[i] = m.m_data[i];
	delete [] orig_data;	//ɾ��ԭ��m_dataָ��Ŀռ�

	return *this;
}

//�������
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