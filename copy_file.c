#include <stdio.h>
#include <mm_malloc.h>

#define BUF_SIZE 256

int copy_file(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("Few args\n");
		return 1;
	}
	if (argc > 3)
	{
		printf("Too much argc\n");
		return 2;
	}

	char* buf = (char*)malloc(sizeof(char) * BUF_SIZE);
	FILE *from_file = fopen(argv[1], "r");
	FILE *to_file = fopen(argv[2], "w+");

	while (fgets(buf, BUF_SIZE, from_file) != NULL)
	{
		fprintf(to_file, "%s", buf);
	}

	free(buf);
	fclose(from_file);
	fclose(to_file);
	return 0;
}

