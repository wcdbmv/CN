#include "socket.h"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

int tprintf(const char *format, ...)
{
	const time_t timer = time(NULL);
	printf("[%.19s] ", ctime(&timer));

	va_list ap;
	va_start(ap, format);
	const int rc = vprintf(format, ap);
	va_end(ap);

	return rc;
}
