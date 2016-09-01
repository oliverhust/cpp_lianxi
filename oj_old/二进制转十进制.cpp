#include <stdio.h>
#include <string.h>
#include <ctype.h>

void main()
{
	unsigned int s;

	scanf_s("0x%X", &s, sizeof(unsigned int));

	printf("%d", s);

}