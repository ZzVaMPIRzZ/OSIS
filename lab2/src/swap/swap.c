#include "swap.h"

void Swap(char *left, char *right)
{
	char x = *left;
	*left = *right;
	*right = x;
}
