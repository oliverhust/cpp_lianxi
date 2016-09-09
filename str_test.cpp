#include <iostream>
#include <vector>
#include <string>

using namespace std;



void display(const string &s)
{
	cout << "s = \"" << s << "\"" << endl;
	cout << "sizeof s = " << sizeof(s) << endl;
	cout << "s capacity: " << s.capacity() << endl;
	cout << "s size = " << s.size() << endl;
	cout << "s length = " << s.length() << endl << endl;
}

int main()
{
	string s1, s2, s3, s4;
	char sz1[64], sz2[64];


	string s5 = "123456789";
	//cout << "int(s5) = " << int(s5) << endl;
	display(s5);

	cout << "-------------resize to 129-----------------" << endl;
	s5.resize(129);
	display(s5);

	//while(1);

	return 0;
}

