#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "concat.h"

int main()
{
	char *result = concat("ccci", "v", "g", "a", 5);
	printf("%s\n", result);
	
	fdinfo(atoi(argv[1])); //task5
	
	return 0;
}
