//#include "copy_file.h"
#include "stdio.h"
#include "dirent.h"
#include "mm_malloc.h"
#include "string.h"
#include "sys/types.h"
#include "unistd.h"
#include "sys/stat.h"

 unsigned long find_dir_name_ind(const char *dir_path, unsigned long *n)
{
	long result = 0;
	for (unsigned long i = (*n - 1); i != 0; i--)
	{
		if (dir_path[i] == '/' || dir_path[i] == '.')
			return (i + 1);
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

char* crt_new_dir_path(char* old_dir_path, const unsigned long* old_dir_path_len, const unsigned long* begin_ind,
		const char* new_dir_name)
{
	char *result = (char*)malloc(sizeof(char) * *old_dir_path_len);
	strcpy(result, old_dir_path);
	for (unsigned long i = *begin_ind, j = 0; i < *old_dir_path_len; i++, j++)
	{
		result[i] = new_dir_name[j];
	}
	return result;
}

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

int main(int argc, char* argv[])
{
	if (argc > 2)
	{
		printf("Too much args\n");
		return 1;
	}
	if (argc < 2)
	{
		printf("Few args\n");
		return 1;
	}

	DIR *old_dir = opendir(argv[1]);
	if (old_dir == NULL)
	{
		printf("No such directory %s\n", argv[1]);
		return 2;
	}
	unsigned long old_dir_path_len = strlen(argv[1]);
	unsigned long old_dir_name_ind = find_dir_name_ind(argv[1], &old_dir_path_len);
	char* old_dir_name = string_cut(argv[1], &old_dir_name_ind, &old_dir_path_len);

	char* new_dir_name = string_reverse(old_dir_name);
	char* new_dir_path = crt_new_dir_path(argv[1], &old_dir_path_len, &old_dir_name_ind, new_dir_name);

	printf("new dir path: %s\n", new_dir_path);
	mkdir(new_dir_path, 0644);
	free(old_dir_name);
	free(new_dir_name);
	closedir(old_dir);
	return 0;
}

/* TODO
 * Скопировать файлы из старой директории в новую 
 *
 */