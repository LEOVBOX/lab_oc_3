//
// Created by Леонид Шайхутдинов on 11.03.2023.
//

#include "mm_malloc.h"
#include "string.h"

char* string_reverse(const char* string)
{
	unsigned long n = strlen(string);
	char* result = (char*)malloc(n * sizeof(char));
	for (int i = 0; i < n; i++)
	{
		result[i] = string[n - i - 1];
	}
	return result;
}

char* string_cut(const char* string, const unsigned long* begin_ind, const unsigned long* end_ind)
{
	char *result = (char*)malloc(sizeof(char) * *end_ind);
	for (unsigned long i = *begin_ind, j = 0; j < *end_ind; i++, j++)
	{
		result[j] = string[i];
	}
	return result;

}