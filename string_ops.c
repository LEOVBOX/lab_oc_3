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


char* string_concat(char* str1, char* str2)
{
	unsigned long str1_len = strlen(str1);
	unsigned long str2_len = strlen(str2);
	unsigned long new_str_len = str1_len + str2_len;
	char* new_str = (char*)malloc(sizeof(char) * new_str_len);
	strcpy(new_str, str1);
	for (unsigned long i = str1_len, j = 0; i < new_str_len; i++, j++)
	{
		new_str[i] = str2[j];
	}
	return new_str;
}