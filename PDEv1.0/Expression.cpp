#include "stdafx.h"
#include "Expression.h"

//����
const double Expression::E = 2.718281828459;
const double Expression::PI = 3.14159265359;

Expression::Expression()
{

}

Expression::Expression(const std::string& symbol,double precision)
	: m_variable_symbol(symbol),m_eps(precision)
{

}

Expression::~Expression()
{

}

//������ʽ
int Expression::DealString(std::string& exp)
{

	//ȫ���ַ���Сд
	std::transform(exp.begin(),exp.end(),exp.begin(),tolower);

	std::string::size_type pos;
	//ɾ�����ʽ�еĿո��Ʊ���͵Ⱥ�
	pos = exp.find(' ');
	if(pos != std::string::npos)
		exp.erase(pos,1);
	pos = exp.find('\t');
	if(pos != std::string::npos)
		exp.erase(pos,1);
	pos = exp.find('=');
	if(pos != std::string::npos)
		exp.erase(pos,1);

	//�滻�ַ�
	struct transform
	{
		std::string oldStr;
		std::string newStr;
	};
	transform trans[] = { {"sin","S"},{"cos","C"},
						  {"tan","C"},{"ln","L"},
						  {"log","G"},{"sqrt","R"},
						  {"exp","E"},{"e","E"}
	};
	for(int i=0;i<sizeof(trans)/sizeof(trans[0]);i++)
	{
		do 
		{
			pos = exp.find(trans[i].oldStr);
			if(pos != std::string::npos)
				exp.replace(pos,trans[i].oldStr.size(),trans[i].newStr);
		} while (pos != std::string::npos);
		//pos = exp.find(trans[i].oldStr);
		//if(pos != std::string::npos)
		//	exp.replace(pos,trans[i].oldStr.size(),trans[i].newStr);
	}
	return 0;
}

//ת�����ʽ
bool Expression::TransExp(const std::string& exp,std::string& postexp) const
{
	char temp,szbuffer[254];
	std::string::size_type pos;
	std::string::size_type idx,n;
	Stack<char> opstack;

	if(!CharacterFilter(exp))
		return false;

	idx = pos = 0;
	n = exp.size();			//Ҫ������ַ������ַ�����
	postexp.clear();		//��������ַ�
	opstack.push('=');		//��ʼ��ջ
	while(idx < n)
	{
		temp = exp.at(idx);
		//��ͷ�Ǹ���
		if((idx == 0 && temp == '-') || 
			(idx != 0 && exp.at(idx-1) == '(' && temp == '-'))
		{
			postexp += "0";
			postexp += "#";		
		}
		
		//������������
		if(!InOp(temp))
		{
			//�ж��Ƿ�Ϊ�Ա�������(x,y�ȵ�)
			unsigned int k=0;
			for(k=0;k<m_variable_symbol.size();k++)
			{
				pos = m_variable_symbol.find(temp);
				if(pos != std::string::npos)		//��ǰ�ַ����Ա�������
				{
					postexp += temp;
					postexp += "#";
					idx++;		//��һ���ַ�
					break;
				}		
			}
			if(k < m_variable_symbol.size())
				continue;

			if(temp == 'E')				//����ַ�ΪE(�����ַ�)
			{
				_snprintf_s(szbuffer,254,"%f",E);
				postexp += szbuffer;
				postexp += "#";
				idx++;
				continue;
			}
			else if(temp == '.')		//����ֻ��һ��"."��ͷ������
			{
				postexp += "0.";
				idx++;
				if(idx>=n)				//�����ַ�����Χ
				{
					postexp += "0#";
					break;
				}
				temp = exp.at(idx);		//�����һ���ַ�
			}

			//��ȡ����
			int calc = 0;
			while((temp>='0' && temp<='9') || (temp == '.'))
			{
				postexp += temp;
				idx++;
				if(idx>=n)		//��������ַ�������ַ���
					break;
				temp = exp.at(idx);				
			}
			postexp += "#";
		}
		else		//���ɨ��ĵ��ַ�Ϊ�����
		{
			switch(PrecedeOp(opstack.top(),temp))
			{
			case -1:					//ջ������������ȼ���
				opstack.push(temp);		//��ջ
				idx++;					//ɨ�������ַ�
				break;

			case 0:						//ֻ�����������������
				opstack.pop();			//��ջ
				idx++;					//ɨ�������ַ�
				break;

			case 1:						//��ջ�����浽ת������ַ�����
				postexp += opstack.pop();				
				break;
			}
		}
	}

	while(opstack.top() != '=')
	{
		postexp += opstack.pop();
	}
	return true;
}

//������ʽ��ֵ
bool Expression::CompValue(const std::string& postexp,std::vector<double>& values,double& result) const
{
	char temp,*pstr = NULL;
	std::string::size_type idx,n,pos;
	double a,b,c,d;
	Stack<double> numstack;
	string number;

	idx = 0;
	n = postexp.size();
	
	try
	{
		while(idx < n)
		{
			temp = postexp.at(idx);
			switch(temp)
			{
			case '+':					//�ӷ�"+"
				a = numstack.pop();
				b = numstack.pop();
				c = a+b;
				numstack.push(c);
				break;

			case '-':					//����"-"
				a = numstack.pop();
				b = numstack.pop();
				c = b - a;
				numstack.push(c);
				break;

			case '*':					//�˷�"*"
				a = numstack.pop();
				b = numstack.pop();
				c = a*b;
				numstack.push(c);
				break;


			case '/':					//����"/"
				a = numstack.pop();
				b = numstack.pop();
				if(fabs(a) > m_eps)
				{
					c = b / a;
					numstack.push(c);
				}
				else
					return false;
				break;

			case 'S':
				a = numstack.pop();
				c = sin(a);
				numstack.push(c);
				break;

			case 'C':
				a = numstack.pop();
				c = cos(a);
				numstack.push(c);
				break;

			case 'T':
				a = numstack.pop();
				c = tan(a);
				numstack.push(c);
				break;

			case 'L':
				a = numstack.pop();
				if(a < 0)
					return false;
				c = log(a);
				numstack.push(c);
				break;

			case 'G':
				a = numstack.pop();
				if(a < 0)
					return false;
				c = log10(a);
				numstack.push(c);
				break;

			case 'R':
				a = numstack.pop();
				if(a < 0)
					return false;
				c = sqrt(a);
				numstack.push(c);
				break;

			case '^':
				a = numstack.pop();
				b = numstack.pop();
				c = pow(b,a);
				numstack.push(c);
				break;

			default:
				d = 0.0;
				pos = m_variable_symbol.find(temp);
				if(pos != std::string::npos)
				{
					numstack.push(values.at(pos));
					idx++;
					break;
				}

				//����С��
				number.clear();
				while(temp != '#')
				{
					number += temp;
					idx++;
					temp = postexp.at(idx);
				}
				d = strtod(number.c_str(),&pstr);
				numstack.push(d);
				break;
			}

			idx++;
		}

	}
	catch(out_of_range)
	{
		return false;
	}

	if(numstack.size() != 1)
		return false;
	result = numstack.pop();
	return true;
}

//һ����λ
bool Expression::Computate(const std::string& exp,std::vector<double>& values,double& result)
{
	std::string exp_t,postexp;

	exp_t = exp;
	DealString(exp_t);
	if(!TransExp(exp_t,postexp))		//ת�����ʽΪ��׺���ʽ
		return false;
	if(!CompValue(postexp,values,result))
		return false;

	return true;
}

int Expression::LeftPri(const char op) const
{
	switch(op)
	{
	case '=':
		return 0;
	case '(':
		return 1;
	case '+':
	case '-':
		return  3;
	case '*':
	case '/':
		return 5;
	case 'S':
	case 'C':
	case 'T':
	case 'L':
	case 'G':
	case 'R':
	case '^':
		return 6;
	case ')':
		return 10;
	}
	return -1;
}

int Expression::RightPri(const char op) const
{
	switch(op)
	{
	case '=':
		return 0;
	case '(':
		return 10;
	case '+':
	case '-':
		return  2;
	case '*':
	case '/':
		return 4;
	case 'S':
	case 'C':
	case 'T':
	case 'L':
	case 'G':
	case 'R':
	case '^':
		return 7;
	case ')':
		return 1;
	}
	return -1;
}

bool Expression::InOp(const char ch) const
{
	int	  nNumOfOp;
	const char	operators_array[] = {'=','(',')','+','-','*','/','S', \
		'C','T','L','G','R','^'};

	nNumOfOp = sizeof(operators_array) / sizeof(operators_array[0]);

	for(int i=0;i<nNumOfOp;i++)
	{
		if(ch == operators_array[i])
			return true;
	}
	return false;
}

int Expression::PrecedeOp(const char op1,const char op2) const
{
	if(LeftPri(op1) == RightPri(op2))
		return 0;
	else if(LeftPri(op1) < RightPri(op2))
		return -1;
	return 1;
}

bool Expression::CharacterFilter(const std::string& exp) const
{
	char temp;
	Stack<char> stack;
	std::string::size_type idx,pos,n;

	n = exp.size();

	for(pos=0;pos!=n;pos++)
	{
		temp = exp.at(pos);
		if(!InOp(temp))
		{
			if(temp>='0' && temp<='9')
				continue;
			else if(temp == 'E')	//�ж��Ƿ���ָ��e^x
				continue;
			else if(temp == '.')
				continue;
			idx = m_variable_symbol.find(temp);
			if(idx == std::string::npos)	//�����Ա�������
				return false;
		}
		else if(temp == '(')
		{
			//��ջ
			stack.push(temp);
		}
		else if(temp == ')')
		{
			if(stack.empty())
				return false;
			//��ջ
			stack.pop();
		}
	}
	return true;
}