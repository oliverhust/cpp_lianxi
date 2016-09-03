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
	l << "Msecond" << "Third";
	l.append("Aaabcbdbab");
	
	mylist<string> init_l = l;
	init_l[1] = "New_string";

	mylist<string> eq_l;
	eq_l = init_l;
	eq_l = l;	
	eq_l[-1] = "last_string";

	list_puts(cout, l);
	cout << "------------copy-------------" << endl;
	list_puts(cout, init_l);
	cout << "------------equal-------------" << endl;
	list_puts(cout, eq_l);

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