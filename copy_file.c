#include <stdio.h>
#include <mm_malloc.h>
#include <fcntl.h>

#define BUF_SIZE 256

int copy_file(char* from_file_path, char* to_file_path)
{

	char* buf = (char*)malloc(sizeof(char) * BUF_SIZE);
	FILE *from_file = fopen(from_file_path, "r");
	FILE *to_file = fopen(to_file_path, "w+");
	if (to_file == NULL)
	{
		int res = creat(to_file_path, 0666);
		to_file = fopen(to_file_path, "w");
	}
	while (fgets(buf, BUF_SIZE, from_file) != NULL)
	{
		fprintf(to_file, "%s", buf);
	}

	free(buf);
	fclose(from_file);
	fclose(to_file);
	return 0;
}

