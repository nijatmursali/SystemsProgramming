#include "concat.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *concat(char *format, ...){
	va_list args;
	int position = 0;
	int len = strlen(format);
	char *result;
	va_start(args, format);
	result = malloc(len+1);
	while (*format != '\0') {
        if (*format == 'i') {
			char *temporary;
			sprintf(temporary, "%d", va_arg(args, char *));
			strcat(result, temporary);
        } else if (*format == 'c') {
			strcat(result, va_arg(args, char *));
        }
        ++position;
        ++format;
    }
	va_end(args);
	return result;
}
