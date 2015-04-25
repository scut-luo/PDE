/************************************************************************/
/* �Զ���Stack(Դ�ԡ�C++��׼����                                      */
/************************************************************************/

#include <deque>
#include <exception>

template<class T>
class Stack
{
protected:
	std::deque<T> c;		//����

public:
	class ReadEmptyStack : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return "Read empty stack";
		}
	};

	//����Ԫ�ظ���
	typename std::deque<T>::size_type size() const
	{
		return c.size();
	}

	//�ж�Stack�Ƿ�Ϊ��
	bool empty() const
	{
		return c.empty();
	}

	//��Ԫ��ѹ��ջ
	void push(const T& item)
	{
		c.push_back(item);
	}

	//��Ԫ�ص���ջ
	T pop()
	{
		if(c.empty())
			throw ReadEmptyStack();
		T elem(c.back());
		c.pop_back();
		return elem;
	}

	//����ջ��Ԫ��
	T& top()
	{
		if(c.empty())
			throw ReadEmptyStack();
		return c.back();
	}
};