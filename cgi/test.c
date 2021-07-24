#include "stdio.h"

int main(int ac, char **av, char **ev)
{
	printf("Content-Type: text/html\r\n\r\n");
	printf("<font size=+10>Environment</font><br>");

	for(int i = 0; ev[i]; i++)
		printf("%s\n", ev[i]);
	return 0;
}
