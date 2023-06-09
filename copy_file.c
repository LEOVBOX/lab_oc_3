#include <stdio.h>
#include <mm_malloc.h>
#include <fcntl.h>
#include "string_ops.h"
#include "mem_ops.h"

#define MAX_BUF_SIZE 7

long opened_file_size(FILE* file)
{
	fseek(file, 0, SEEK_END);
	long result = ftell(file);
	return result;
}

int copy_file_reverse(char* from_file_path, char* to_file_path)
{
	FILE *from_file = fopen(from_file_path, "r");
	if (from_file == NULL)
	{
		printf("Open %s failed\n", to_file_path);
	}

	long from_file_size = opened_file_size(from_file);
	int buf_size;
	int residual_chars = 0;
	int inter_count = 1;
	if (from_file_size < MAX_BUF_SIZE)
	{
		buf_size = (int)from_file_size;
		//buf_size = MAX_BUF_SIZE;
	}
	else
	{
		buf_size = MAX_BUF_SIZE;
		inter_count = (int) from_file_size / buf_size;
		residual_chars = (int) from_file_size % buf_size;
	}

	char* buf = (char*)mallocs(sizeof(char) * buf_size);

	FILE *to_file = fopen(to_file_path, "w+");
	if (to_file == NULL)
	{
		printf("Open %s failed\n", to_file_path);
	}
	for (int i = 0; i < inter_count; i++)
	{
		fseek(from_file, -buf_size, SEEK_CUR);
		fgets(buf, buf_size+1, from_file);
		string_reverse(buf);
		fprintf(to_file, "%s", buf);
	}
	if (residual_chars > 0)
	{
		free(buf);
		buf = (char*)mallocs(sizeof(char) * residual_chars);
		if (buf == NULL)
		{
			printf("Realloc error\n");
			exit(1);
		}

		if (fseek(from_file, 0, SEEK_SET) == -1)
			printf("fseek failed\n");
		fgets(buf, residual_chars+1, from_file);
		string_reverse(buf);
		fprintf(to_file, "%s", buf);
	}

	free(buf);
	fclose(from_file);
	fclose(to_file);
	return 0;
}

