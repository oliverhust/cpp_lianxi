#include <iostream>
#include <string>

using namespace std;

int test1()
{
	int n1;
	string str1, str2, str3;

	if (!(cin >> str1))
	{
		cout << "Input str1 error." << endl;
		return 1;
	}
	cout << "Cin state1:" << bool(cin) << endl;

	if (!(cin >> str2))
	{
		cout << "Input str2 error." << endl;
		return 2;
	}
	cout << "Cin state2:" << bool(cin) << endl;

	getline(cin, str3);
	cout << "Cin state3:" << bool(cin) << endl;

	cout << "str1 = [" << str1 << "]" << endl;
	cout << "str2 = [" << str2 << "]" << endl;
	cout << "str3 = [" << str3 << "]" << endl;

	return 0;
}

int test2()
{
#define STR_MAX_SIZE				16
	
	char str1[STR_MAX_SIZE], str2[STR_MAX_SIZE], str3[STR_MAX_SIZE];

	if (!(cin.get(str1, sizeof(str1))))
	{
		cout << "Input str1 error." << endl;
	}
	cout << "Cin state1:" << bool(cin) << endl;

	if (!(cin.get(str2, sizeof(str2))))
	{
		cout << "Input str2 error." << endl;
	}
	cout << "Cin state2:" << bool(cin) << endl;

	if (!(cin.getline(str3, sizeof(str3))))
	{
		cout << "Input str3 error." << endl;
	}
	cout << "Cin state3:" << bool(cin) << endl;

	cout << "str1 = [" << str1 << "]" << endl;
	cout << "str2 = [" << str2 << "]" << endl;
	cout << "str3 = [" << str3 << "]" << endl;

	return 0;
}

int main()
{
	test1();

	return 0;
}


