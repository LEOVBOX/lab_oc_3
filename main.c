#include "copy_file.h"
#include "stdio.h"
#include "dirent.h"
#include "mm_malloc.h"
#include "string.h"
#include "sys/stat.h"
#include "string_ops.h"
#include "mem_ops.h"

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



char* crt_file_path(char* file_path, char* dir_path, char* file_name)
{
	if (strlen(file_path) < strlen(dir_path))
		file_path = reallocs(file_path, strlen(dir_path) + 1 + strlen(file_name));
	strcpy(file_path, dir_path);
	strcat(file_path, "/");
	strcat(file_path, file_name);
	return file_path;
}

void crt_new_dir_path(char* old_dir_path, const unsigned long* begin_ind, char* new_dir_path)
{
	new_dir_path = reallocs(new_dir_path, strlen(old_dir_path));
	strcpy(new_dir_path, old_dir_path);
	// Reversing dir name
	string_reverse(new_dir_path + (*begin_ind));
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


	char* new_dir_path = (char*)mallocs(sizeof(char) * old_dir_path_len);
	crt_new_dir_path(old_dir_path, &old_dir_name_ind, new_dir_path);

	printf("new dir path: %s\n", new_dir_path);
	mkdir(new_dir_path, 0755);

	struct dirent *entry = readdir(old_dir);

	char* old_file_name = mallocs(sizeof(char));
	char* old_file_path = mallocs(sizeof(char));
	char* new_file_name = mallocs(sizeof(char));
	char* new_file_path = mallocs(sizeof(char));

	while (entry = readdir(old_dir), entry != NULL)
	{
		printf("%llu %d %s\n", entry->d_ino, entry->d_type, entry->d_name);
		if (entry->d_type == DT_REG)
		{
			// Creating path for old file
			string_copy(old_file_name, entry->d_name);
			old_file_path = crt_file_path(old_file_path, old_dir_path, old_file_name);

			// Creating new (reversed) file name.
 			string_copy(new_file_name, old_file_name);
			string_reverse(new_file_name);

			// Creating path for new file
			new_file_path = crt_file_path(new_file_path, new_dir_path, new_file_name);

			copy_file_reverse(old_file_path, new_file_path);
		}
	}

	free(old_file_path);
	free(new_file_name);
	free(new_file_path);

	closedir(old_dir);
	return 0;
}

/* TODO
 * Изменить считывание. Ставить поинтер файла на конец и смещать на количество байт страницы к началу
 * Сделать реалок перед strcpy +
 * Вынести проверки малока и реалока +
 */