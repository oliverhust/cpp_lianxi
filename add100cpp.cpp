#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;


double sum(int n)
{
	double s = 0;

	for (int i = 1; i <= n; i++)
	{
		if (i % 2 != 0)
		{
			s += 1.0 / i / i;
		}
		else
		{
			s += 2.0 / i / i;
		}

	}

	return s;
}



int main()
{
	int n;
	double s = 0, e = 0;
	clock_t t_start, t_end;

	n = 600 * 1000 * 1000;

	t_start = clock();
	s = sum(n);
	t_end = clock();
	
	e = (double)(t_end - t_start) / CLK_TCK;
	cout << "Sum " << n << "= " << s << endl;
	cout << "Elape time: " << e << " seconds" << endl;
	 	
	return 0;
}