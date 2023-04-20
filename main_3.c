//
// Created by Леонид Шайхутдинов on 17.04.2023.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#define MAX_BUF_SIZE 4096

char* string_cat(char* dest, char* source)
{
    size_t dest_len = strlen(dest);
    size_t source_len = strlen(source);
    if (dest_len < dest_len + source_len);
    {
        dest = (char*)realloc(dest, dest_len + source_len);
        if (dest == NULL)
        {
            printf("realloc failed\n");
        }
    }
    strcat(dest, source);
    return dest;
}

long opened_file_size(FILE* file)
{
	fseek(file, 0, SEEK_END);
	long result = ftell(file);
	fseek(file, 0, SEEK_SET);
	return result;
}

int print_file_content(char* path)
{
	FILE *file= fopen(path, "r");
	if (file == NULL)
	{
		printf("Open %s failed\n", path);
		return -1;
	}
	//printf("file %s opened\n", path);

	long file_size = opened_file_size(file);
	printf("opened file size = %ld\n", file_size);

	int buf_size;
	int residual_chars = 0;
	int iter_count = 1;
	if (file_size < MAX_BUF_SIZE)
	{
		buf_size = (int)file_size;
	}
	else
	{
		buf_size = MAX_BUF_SIZE;
		iter_count = (int) file_size / buf_size;
		residual_chars = (int) file_size % buf_size;
	}

	char* buf = (char*)malloc(sizeof(char) * buf_size);
    if (buf == NULL)
    {
        printf("malloc failed\n");
    }

	for (int i = 0; i < iter_count; i++)
	{
		if (fread(buf, 1, buf_size, file) == 0)
		{
			printf("fread failed\n");
		}
		printf("%s", buf);
	}

	if (residual_chars > 0)
	{
		free(buf);
		buf = (char*)malloc(sizeof(char) * residual_chars);
        if (buf == NULL)
        {
            printf("fread failed\n");
        }
    
		fread(buf, 1, residual_chars+1, file);
        {
            printf("fread failed\n");
        }
		printf("%s", buf);
	}

	free(buf);

	fclose(file);

	return 0;
}

int main()
{
    pid_t pid = getpid();
    char* pid_str = atoi(pid);
    printf("pid = %d\n", pid);
    char* pagemap_str = "/proc/";

    pagemap_str = string_cat(pagemap_str, pid_str);
    pagemap_str = string_cat(pagemap_str, "/pagemap");

    print_file_content(pagemap_str);
    
    return 0;
}