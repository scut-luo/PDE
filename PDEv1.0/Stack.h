/************************************************************************/
/* 自定义Stack(源自《C++标准程序》                                      */
/************************************************************************/

#include <deque>
#include <exception>

template<class T>
class Stack
{
protected:
	std::deque<T> c;		//容器

public:
	class ReadEmptyStack : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return "Read empty stack";
		}
	};

	//返回元素个数
	typename std::deque<T>::size_type size() const
	{
		return c.size();
	}

	//判断Stack是否为空
	bool empty() const
	{
		return c.empty();
	}

	//将元素压入栈
	void push(const T& item)
	{
		c.push_back(item);
	}

	//将元素弹出栈
	T pop()
	{
		if(c.empty())
			throw ReadEmptyStack();
		T elem(c.back());
		c.pop_back();
		return elem;
	}

	//返回栈顶元素
	T& top()
	{
		if(c.empty())
			throw ReadEmptyStack();
		return c.back();
	}
};