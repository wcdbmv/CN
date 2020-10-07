#include "my_lltoa.h"

#include <stdbool.h>

void swap(char *a, char *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

void reverse(char *s, char *f)
{
	while (s < f) {
		swap(s++, f--);
	}
}

char *my_lltoa(long long int value, char *str, int radix) {
	char *str_start = str;
	bool is_neg = false;

	if (!value) {
		*str++ = '0';
		*str = '\0';
		return str_start;
	}

	if (value < 0 /* && radix == 10 */) {
		is_neg = true;
		value = -value;
	}

	while (value != 0) {
		long long int r = value % (long long int) radix;
		*str++ = (char) (r > 9 ? r - 10 + 'a' : r + '0');
		value /= (long long int) radix;
	}

	if (is_neg) {
		*str++ = '-';
	}

	*str = '\0';

	reverse(str_start, str - 1);

	return str_start;
}
