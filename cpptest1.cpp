#include <iostream>
#include <string>

using namespace std;

int main()
{
	char str[100];

	memcpy(str, "abc", 4);
	printf("%s\n", str);

	cout << "abcdeg1" << endl;
	cout << "abcdeg2" << endl;

	return 0;
}