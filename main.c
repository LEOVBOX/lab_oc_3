#include "copy_file.h"
#include "stdio.h"
#include "dirent.h"
#include "mm_malloc.h"
#include "string.h"
#include "sys/stat.h"
#include "string_ops.h"


 unsigned long find_dir_name_ind(const char *dir_path, unsigned long *n)
{
	long result = 0;
	for (unsigned long i = (*n - 1); i != 0; i--)
	{
		if (dir_path[i] == '/')
			return (i + 1);
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

	char *old_dir_path = argv[1];
	DIR *old_dir = opendir(old_dir_path);
	if (old_dir == NULL)
	{
		printf("No such directory %s\n", old_dir_path);
		return 2;
	}

	unsigned long old_dir_path_len = strlen(old_dir_path);
	unsigned long old_dir_name_ind = find_dir_name_ind(old_dir_path, &old_dir_path_len);
	char* old_dir_name = string_cut(old_dir_path, &old_dir_name_ind, &old_dir_path_len);

	char* new_dir_name = string_reverse(old_dir_name);
	char* new_dir_path = crt_new_dir_path(old_dir_path, &old_dir_path_len, &old_dir_name_ind, new_dir_name);


	printf("new dir path: %s\n", new_dir_path);
	mkdir(new_dir_path, 0755);

	struct dirent *entry = readdir(old_dir);

	char* old_file_name = NULL;
	char* old_file_path = NULL;
	char* new_file_name = NULL;
	char* new_file_path = NULL;

	while (entry = readdir(old_dir), entry != NULL)
	{
		printf("%llu %d %s\n", entry->d_ino, entry->d_type, entry->d_name);
		if (entry->d_type == DT_REG)
		{
			old_file_name = entry->d_name;
			old_file_path = string_concat(old_dir_path, "/");
			new_file_path = string_concat(new_dir_path, "/");
			old_file_path = string_concat(old_file_path, old_file_name);
			new_file_name = string_reverse(old_file_name);
			new_file_path = string_concat(new_file_path, new_file_name);
			copy_file_reverse(old_file_path, new_file_path);
		}
	}

	free(old_file_path);
	free(new_file_name);
	free(new_file_path);

	free(old_dir_name);
	free(new_dir_name);
	closedir(old_dir);
	return 0;
}

/* TODO
 * Сделать вывод запись задом наперед
 *
 *
 */