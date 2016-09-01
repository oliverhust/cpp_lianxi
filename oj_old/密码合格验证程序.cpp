#include <iostream>

#include <string>

using namespace std;




int main()

{

	string str[100];

	int  flag1, flag2, flag3, flag4, num, temp, i = 0;

	bool bl;

	while (getline(cin, str[i]))

	{

		i++;

	}




	for (int j = 0; j<i; j++)

	{

		bl = false;

		int len = str[j].length();




		//changdu

		if (len <= 8)

		{

			cout << "NG" << endl;

			bl = true;

		}




		//zhonglei

		if (!bl)

		{

			flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0;

			for (int k = 0; k<len; k++)

			{

				if (str[j][k] >= 'A'&&str[j][k] <= 'Z')

					flag1 = 1;

				else if (str[j][k] >= 'a'&&str[j][k] <= 'z')

					flag2 = 1;

				else if (str[j][k] >= '0'&&str[j][k] <= '9')

					flag3 = 1;

				else

					flag4 = 1;

			}




			if ((flag1 + flag2 + flag3 + flag4)<3)

			{

				cout << "NG" << endl;

				bl = true;

			}

		}




		//zichuangchongfu

		if (!bl)

		{

			for (int k = 0; k<len - 3; k++)

			{
				temp = k;

				for (int s = k + 1; s<len - 2; s++)

				{

					num = 0;

					while ((str[j][temp] == str[j][s]) && temp<len&&s<len)

					{

						num++;

						temp++;

						s++;

					}

					if (num >= 3)

					{

						cout << "NG" << endl;

						bl = true;

						break;

					}

				}

				if (num >= 3)

					break;



			}

		}





		if (!bl)

			cout << "OK" << endl;

	}




	return 0;

}

