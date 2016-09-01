#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 8192


char ConvertChar(char c)
{
	char r = c;

	if (isupper(c))
	{
		if ('Z' != c)
		{
			r = tolower(c) + 1;
		}
		else
		{
			r = 'a';
		}
	}
	else if (islower(c))
	{
		switch (c)
		{
		case 'a':
		case 'b':
		case 'c': r = '2'; break;
		case 'd':
		case 'e':
		case 'f': r = '3'; break;
		case 'g':
		case 'h':
		case 'i': r = '4'; break;
		case 'j':
		case 'k':
		case 'l': r = '5'; break;
		case 'm':
		case 'n':
		case 'o': r = '6'; break;
		case 'p':
		case 'q':	
		case 'r':
		case 's': r = '7'; break;
		case 't':
		case 'u':
		case 'v': r = '8'; break;
		case 'w':
		case 'x':
		case 'y':
		case 'z': r = '9'; break;
		}
	}

	return r;		
}

void main()
{
	int i;
	char buf[MAXSIZE];

	scanf_s("%s", buf, sizeof(buf));

	for (i = 0; i < (int)strlen(buf); i++)
	{
		buf[i] = ConvertChar(buf[i]);
	}

	printf("%s", buf);

}