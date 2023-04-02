//
// Created by Леонид Шайхутдинов on 27.03.2023.
//

#include "mem_ops.h"
#include <sys/stat.h>
#include <stdlib.h>
#include "dirent.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>


#define MAX_BUF_SIZE 1024

char* crt_filename(char *path)
{
	unsigned long filename_ind = 0;
	unsigned long n = strlen(path);
	for (unsigned long i = (n - 1); i != 0; i--)
	{
		if (path[i] == '/')
		{
			filename_ind = (i + 1);
			break;
		}
	}
	char* filename = (char*)mallocs(n - filename_ind);
	strcpy(filename, path+filename_ind);
	return filename;
}


mode_t convert_string_mode(char* mode_str)
{
	char* invalid_char;
	mode_t mode = strtol(mode_str, &invalid_char, 8);
	if (*invalid_char != '\0')
	{
		printf("Invalid character in mode argument %c\n", *invalid_char);
		return 0;
	}
	return mode;
}

int crt_new_dir(char* path, char* mode_str)
{
	mode_t mode = convert_string_mode(mode_str);
	if (mode == 0)
	{
		printf("wrong mode\n");
		return -1;
	}
	mkdir(path, mode);
	return 0;
}

int crt_file(char* path, char* mode_str)
{
	mode_t mode = convert_string_mode(mode_str);
	if (mode == 0)
		return -1;

	int fd = creat(path, mode);
	if (fd == -1)
		return -1;
	close(fd);
	return fd;
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

	char* buf = (char*)mallocs(sizeof(char) * buf_size);

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
		buf = (char*)mallocs(sizeof(char) * residual_chars);
		fread(buf, 1, residual_chars+1, file);
		printf("%s", buf);
	}

	free(buf);

	fclose(file);

	return 0;
}

int print_dir_content(char* path)
{
	DIR* cur_dir = opendir(path);
	if (cur_dir == NULL)
	{
		printf("Opening %s failed\n", path);
		return -1;
	}

	struct dirent *entry;

	while (entry = readdir(cur_dir), entry != NULL)
	{
		printf("%s %llu %d\n", entry->d_name, entry->d_ino, entry->d_type);
	}

	return 0;
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

// Returns 0 if success, else returns -1.
int del_file(char* file_path)
{
	if (remove(file_path) == -1)
	{
		printf("remove(%s) is failed\n", file_path);
		return -1;
	}
	return 0;
}

int del_dir(char* path)
{
	if (remove(path) == -1)
	{
		DIR* cur_dir = opendir(path);
		if (cur_dir == NULL)
		{
			printf("Opening %s failed\n", path);
			return -1;
		}

		// Execute readdir(cur_dir) twice for skip "." and ".." links.
		struct dirent *entry = readdir(cur_dir);
		entry = readdir(cur_dir);
		char* cur_file_path = mallocs(sizeof(char));

		while (entry = readdir(cur_dir), entry != NULL)
		{
			cur_file_path = crt_file_path(cur_file_path, path, entry->d_name);
			printf("cur_file_path: %s\n", cur_file_path);

			if (remove(cur_file_path) == 0)
			{
				printf("%s %llu %d removed\n", entry->d_name, entry->d_ino, entry->d_type);
			}

		}

		if (rmdir(path) != 0)
		{
			printf("rmdir(%s) failed\n", path);
			return -1;
		}
	}

	return 0;
}

char* crt_link_path(char* target)
{
	char* add_text = "_link";
	char* result = (char*)mallocs(sizeof(target) + strlen(add_text));
	strcpy(result, target);
	strcat(result, add_text);
	return result;
}

char* crt_hardlink_path(char* oldpath)
{
	char* add_text = "_hardlink";
	char* result = (char*)mallocs(sizeof(oldpath) + strlen(add_text));
	strcpy(result, oldpath);
	strcat(result, add_text);
	return result;
}

int crt_symlink(char* target)
{
	char* linkpath = crt_link_path(target);
	crt_link_path(target);
	if (symlink(target, linkpath) == -1)
	{
		printf("symlink(%s, %s) failed\n", target, linkpath);
		return -1;
	}
	return 0;
}

// Returns int link_content_size if success, else returns -1
int print_symlink_content(char* linkpath)
{
	char* buf = (char*)mallocs(MAX_BUF_SIZE);
	int link_content_size = (int)readlink(linkpath, buf, MAX_BUF_SIZE);
	if (link_content_size > -1)
	{
		printf("link content:\n%s\n", buf);
	}
	else
	{
		printf("readlink(%s) failed\n", linkpath);
		return -1;
	}
	return link_content_size;
}

// Returns 0 if success, else returns -1;
int del_symlink(char* linkpath)
{
	if (unlink(linkpath) == -1)
	{
		printf("unlink(%s) failed", linkpath);
		return -1;
	}
	return 0;
}


int crt_hardlink(char* oldpath)
{
	char* new_linkpath = crt_hardlink_path(oldpath);
	crt_link_path(oldpath);
	if (link(oldpath, new_linkpath) == -1)
	{
		printf("symlink(%s, %s) failed\n", oldpath, new_linkpath);
		return -1;
	}
	return 0;
}

int del_hardlink(char* linkpath)
{
	return (del_symlink(linkpath));
}

int print_stat(char* path)
{
	struct stat filestat;

	if(stat(path, &filestat) == -1)
	{
		printf("stat(%s) failed\n", path);
		return -1;
	}

	printf("File '%s':\n", path);
	printf("Permissions: %o\n", filestat.st_mode & 0777);
	printf("Hardlinks: %hu\n", filestat.st_nlink);

	return 0;
}

int changemode(char* path, char* mode_str)
{
	mode_t mode = convert_string_mode(mode_str);
	if (chmod(path, mode) == -1)
	{
		printf("chmod(%s, %s) failed\n", path, mode_str);
		return -1;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	char *path = argv[1];
	char *mode_str;

	char *exec_filename = crt_filename(argv[0]);

	printf("%s\n", exec_filename);
	if (argc > 2)
	{
		mode_str = argv[2];
		if (strcmp("crt_new_dir", exec_filename) == 0)
			return crt_new_dir(path, mode_str);
		if (strcmp("crt_file", exec_filename) == 0)
			return crt_file(path, mode_str);
		if (strcmp("changemode", exec_filename) == 0)
			return changemode(path, mode_str);
	}

	if (strcmp("print_file_content", exec_filename) == 0)
		return print_file_content(path);
	if (strcmp("print_dir_content", exec_filename) == 0)
		return print_dir_content(path);
	if (strcmp("del_file", exec_filename) == 0)
		return del_file(path);
	if (strcmp("del_dir", exec_filename) == 0)
		return del_dir(path);
	if (strcmp("crt_symlink", exec_filename) == 0)
		return crt_symlink(path);
	if (strcmp("del_symlink", exec_filename) == 0)
		return del_symlink(path);
	if (strcmp("crt_hardlink", exec_filename) == 0)
		return crt_hardlink(path);
	if (strcmp("del_hardlink", exec_filename) == 0)
		return del_hardlink(path);
	if (strcmp("print_symlink_content", exec_filename) == 0)
		return print_symlink_content(path);
	if (strcmp("print_stat", exec_filename) == 0)
		return print_stat(path);

	else
	{
		printf("Func not found\n");
		return -1;
	}
}