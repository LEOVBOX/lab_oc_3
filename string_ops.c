//
// Created by Леонид Шайхутдинов on 11.03.2023.
//
#include "mm_malloc.h"
#include "string.h"
#include "mem_ops.h"

void swap_char(char* a, char* b)
{
	char temp = *a;
	*a = *b;
	*b = temp;
}

void string_copy(char* dest, char* source)
{
	size_t source_len = strlen(source);
	if (strlen(dest) < source_len)
		dest = realloc(dest, source_len);
		//dest = reallocs(dest, sizeof(char) * strlen(source));

	strcpy(dest, source);
}

void string_reverse(char* string)
{
	size_t len = strlen(string);
	for (int i = 0; i < len/2; i++)
	{
		swap_char(&string[i], &string[len - i - 1]);
	}
}

