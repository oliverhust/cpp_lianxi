#include <iostream>
#include <sstream>
#include <string>
#include "mylist.hpp"

using namespace std;


int test1()
{
	mylist<string> l;
	cout << "Hello, world!" << endl;
	l.append("First");	
	l.append("Msecond");
	l.append("Third");
	l.append("Fifth");
	l.append("Aaabcbdbab");
	list_puts(cout, l);
	cout << "-------------------------" << endl;
	mylist<string> init_l = l;
	list_puts(cout, init_l);
	return 0;
}

void test_str()
{
	string str = "Haha";
	cout << "str = " << str << endl;
	str += int(65);
	cout << "str = " << str << endl;
}

int main()
{
	test1();
	return 0;
}