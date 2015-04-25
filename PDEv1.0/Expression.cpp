#include "stdafx.h"
#include "Expression.h"

//常量
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

//处理表达式
int Expression::DealString(std::string& exp)
{

	//全体字符变小写
	std::transform(exp.begin(),exp.end(),exp.begin(),tolower);

	std::string::size_type pos;
	//删除表达式中的空格、制表符和等号
	pos = exp.find(' ');
	if(pos != std::string::npos)
		exp.erase(pos,1);
	pos = exp.find('\t');
	if(pos != std::string::npos)
		exp.erase(pos,1);
	pos = exp.find('=');
	if(pos != std::string::npos)
		exp.erase(pos,1);

	//替换字符
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

//转化表达式
bool Expression::TransExp(const std::string& exp,std::string& postexp) const
{
	char temp,szbuffer[254];
	std::string::size_type pos;
	std::string::size_type idx,n;
	Stack<char> opstack;

	if(!CharacterFilter(exp))
		return false;

	idx = pos = 0;
	n = exp.size();			//要处理的字符串的字符个数
	postexp.clear();		//清除所有字符
	opstack.push('=');		//初始化栈
	while(idx < n)
	{
		temp = exp.at(idx);
		//开头是负号
		if((idx == 0 && temp == '-') || 
			(idx != 0 && exp.at(idx-1) == '(' && temp == '-'))
		{
			postexp += "0";
			postexp += "#";		
		}
		
		//如果不是运算符
		if(!InOp(temp))
		{
			//判断是否为自变量符号(x,y等等)
			unsigned int k=0;
			for(k=0;k<m_variable_symbol.size();k++)
			{
				pos = m_variable_symbol.find(temp);
				if(pos != std::string::npos)		//当前字符是自变量符号
				{
					postexp += temp;
					postexp += "#";
					idx++;		//下一个字符
					break;
				}		
			}
			if(k < m_variable_symbol.size())
				continue;

			if(temp == 'E')				//如果字符为E(特殊字符)
			{
				_snprintf_s(szbuffer,254,"%f",E);
				postexp += szbuffer;
				postexp += "#";
				idx++;
				continue;
			}
			else if(temp == '.')		//处理只有一个"."开头的数字
			{
				postexp += "0.";
				idx++;
				if(idx>=n)				//超过字符串范围
				{
					postexp += "0#";
					break;
				}
				temp = exp.at(idx);		//获得下一个字符
			}

			//获取数字
			int calc = 0;
			while((temp>='0' && temp<='9') || (temp == '.'))
			{
				postexp += temp;
				idx++;
				if(idx>=n)		//如果超过字符串最大字符数
					break;
				temp = exp.at(idx);				
			}
			postexp += "#";
		}
		else		//如果扫描的到字符为运算符
		{
			switch(PrecedeOp(opstack.top(),temp))
			{
			case -1:					//栈顶运算符的优先级低
				opstack.push(temp);		//入栈
				idx++;					//扫描其他字符
				break;

			case 0:						//只有括号满足这种情况
				opstack.pop();			//出栈
				idx++;					//扫描其他字符
				break;

			case 1:						//退栈并保存到转化后的字符串中
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

//计算表达式的值
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
			case '+':					//加法"+"
				a = numstack.pop();
				b = numstack.pop();
				c = a+b;
				numstack.push(c);
				break;

			case '-':					//减法"-"
				a = numstack.pop();
				b = numstack.pop();
				c = b - a;
				numstack.push(c);
				break;

			case '*':					//乘法"*"
				a = numstack.pop();
				b = numstack.pop();
				c = a*b;
				numstack.push(c);
				break;


			case '/':					//除法"/"
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

				//处理小数
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

//一部到位
bool Expression::Computate(const std::string& exp,std::vector<double>& values,double& result)
{
	std::string exp_t,postexp;

	exp_t = exp;
	DealString(exp_t);
	if(!TransExp(exp_t,postexp))		//转换表达式为后缀表达式
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
			else if(temp == 'E')	//判断是否有指数e^x
				continue;
			else if(temp == '.')
				continue;
			idx = m_variable_symbol.find(temp);
			if(idx == std::string::npos)	//不是自变量符号
				return false;
		}
		else if(temp == '(')
		{
			//入栈
			stack.push(temp);
		}
		else if(temp == ')')
		{
			if(stack.empty())
				return false;
			//出栈
			stack.pop();
		}
	}
	return true;
}