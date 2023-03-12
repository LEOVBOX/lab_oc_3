#include <stdio.h>
#include <mm_malloc.h>
#include <fcntl.h>

#define MAX_BUF_SIZE 1024

long opened_file_size(FILE* file)
{
	fseek(file, 0, SEEK_END);
	long result = ftell(file);
	fseek(file, 0, SEEK_SET);
	return result;
}

int copy_file_reverse(char* from_file_path, char* to_file_path)
{
	FILE *from_file = fopen(from_file_path, "r");
	long from_file_size = opened_file_size(from_file);
	int buf_size;
	int residual_chars = 0;
	int inter_count = 1;
	if (from_file_size < MAX_BUF_SIZE)
	{
		buf_size = (int)from_file_size;
	}
	else
	{
		buf_size = MAX_BUF_SIZE;
		inter_count = (int) from_file_size / buf_size;
		residual_chars = (int) from_file_size % buf_size;
	}
	char* buf = (char*)malloc(sizeof(char) * buf_size);

	FILE *to_file = fopen(to_file_path, "w+");
	if (to_file == NULL)
	{
		int res = creat(to_file_path, 0666);
		to_file = fopen(to_file_path, "w");
	}
	for (int i = 0; i < inter_count; i++)
	{
		fgets(buf, buf_size+1, from_file);
		fprintf(to_file, "%s", buf);
	}
	fclose(from_file);
	fclose(to_file);
	return 0;
}

