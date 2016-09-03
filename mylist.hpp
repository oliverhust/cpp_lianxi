
#ifndef _MY_LIST_H_
#define _MY_LIST_H_

template <typename T> std::ostream &list_puts(std::ostream &os, const T &l);


template <typename T>
class mylist
{
	typedef struct tagNode
	{
		tagNode *pstNext;
		tagNode *pstPrev;
		T data;
	}Node_S;

private:
	Node_S m_stHead;
	int m_count;
	bool m_bFailed;

public:
	mylist() : m_count(0), m_bFailed(false) { m_stHead.pstNext = nullptr; m_stHead.pstPrev = nullptr; }
	mylist(const mylist<T> &old_list);
	~mylist();

	int append(const T &dat);
	int push(const T &dat);
	int pop(T &dat);
	int count() { return m_count; }

	const mylist<T> &operator=(const mylist<T> &old_list);
	mylist<T> &operator<<(const T &dat);
	T &operator[](int index);
	const T &operator[](int index) const;
	operator bool() const;

	friend std::ostream &list_puts <> (std::ostream &os, const mylist<T> &l);

protected:
	void _copyCreate(const mylist<T> &old_list);

};


template <typename T>
int mylist<T>::append(const T &dat)
{
	Node_S *pstData;	

	if (m_stHead.pstNext == nullptr)
	{
		m_stHead.data = dat;
		m_stHead.pstNext = m_stHead.pstPrev = &m_stHead;
		m_count = 1;
		return 0;
	}

	pstData = new Node_S;
	std::cout << "New: 0x" << (int *)pstData << endl;
	if (nullptr == pstData)
	{
		return 1;
	}
	pstData->data = dat;
	
	pstData->pstPrev = m_stHead.pstPrev;
	pstData->pstNext = &m_stHead;
	m_stHead.pstPrev->pstNext = pstData;
	m_stHead.pstPrev = pstData;
	m_count++;
	
	return 0;
}


template <typename T>
mylist<T>::~mylist()
{
	if (nullptr == m_stHead.pstNext)
	{
		return ;
	}

	Node_S *pstFree, *pstNext;
	pstFree = m_stHead.pstNext;
	while (pstFree != &m_stHead)
	{
		pstNext = pstFree->pstNext;
		delete pstFree;
		std::cout << "Delete: 0x" << (int *)pstFree << endl;
		pstFree = pstNext;
	}
	
	this->mylist<T>::mylist();
}


template <typename T>
void mylist<T>::_copyCreate(const mylist<T> &old_list)
{
	this->mylist<T>::mylist();
	if (nullptr == old_list.m_stHead.pstNext)
	{
		return;
	}

	const Node_S *pstSrc;
	pstSrc = &old_list.m_stHead;
	do
	{
		if (0 != append(pstSrc->data))
		{
			break;
		}
		pstSrc = pstSrc->pstNext;
	} while (pstSrc != &old_list.m_stHead);
}


template <class T>
mylist<T>::mylist(const mylist<T> &old_list)
{
	_copyCreate(old_list);
}


template <typename T>
const mylist<T> &mylist<T>::operator=(const mylist<T> &old_list)
{
	if (this == &old_list)
	{
		return *this;
	}

	this->~mylist();
	_copyCreate(old_list);
	
	return *this;
}

template <typename T>
mylist<T> &mylist<T>::operator<<(const T &dat)
{
	if (false != m_bFailed)
	{
		return *this;
	}

	if (0 != append(dat))
	{
		m_bFailed = true;
	}

	return *this;
}

template <typename T>
mylist<T>::operator bool() const
{
	return m_bFailed;
}

template <typename T>
T & mylist<T>::operator[](int index)
{
	int iPos, iReal;
	iReal = index >= 0 ? index : index + m_count;
	if (iReal < 0)
	{
		iReal = 0;
	}
	if (iReal >= m_count)
	{
		iReal = m_count - 1;
	}

	Node_S *pstCurr = &m_stHead;
	
	for (iPos = 0; iPos < iReal; iPos++)
	{
		pstCurr = pstCurr->pstNext;
	}

	return pstCurr->data;	
}


template <typename T>
const T &mylist<T>::operator[](int index) const
{
	int iPos, iReal;
	iReal = index >= 0 ? index : index + m_count;
	if (iReal < 0)
	{
		iReal = 0;
	}
	if (iReal >= m_count)
	{
		iReal = m_count - 1;
	}

	const Node_S *pstCurr = &m_stHead;

	for (iPos = 0; iPos < iReal; iPos++)
	{
		pstCurr = pstCurr->pstNext;
	}

	return pstCurr->data;
}


template <typename T> 
std::ostream &list_puts(std::ostream &os, const mylist<T> &l)
{
	os << "print mylist, count = "<<l.m_count<<":" << endl;
	if (l.m_stHead.pstNext == nullptr)
	{
		os << "No data" << endl;
		return os;
	}

	const mylist<T>::Node_S *pstPrint = &l.m_stHead;
	do
	{
		os << pstPrint->data << " --> ";
		pstPrint = pstPrint->pstNext;
	} while (pstPrint != &l.m_stHead);
	os << endl;

	return os;
}




#endif
