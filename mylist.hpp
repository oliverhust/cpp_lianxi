
#ifndef _MY_LIST_H_
#define _MY_LIST_H_


template <typename T>
class mylist
{
private:
	T *pHead_Next;
	T *pHead_Prev;
	T data;

public:
	mylist() : pHead_Next(nullptr), pHead_Prev(nullptr) {}
	mylist(const my_list &old_list);
	~mylist();
	int append(const T &dat);
	T &get_last();
	int push(const T &dat);
	int pop(T &dat);
	int 
};





#endif
