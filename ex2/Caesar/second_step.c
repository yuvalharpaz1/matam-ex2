#include "second_step.h"

void check_alloc_int(int* str)
{
	if (NULL == str)
	{
		free(str);
		fprintf(stderr, "Error: memory allocation failed\n");
		return ERROR;
	}
}

void lines_in_thread(int* array, int thread_num, int lines)
{
	int counter = 0;
	for (int i = 0; i < thread_num; i++)
	{
		array[i] = 0;
	}
	while (counter < lines)
	{
		array[counter % thread_num]++;
		counter++;
	}
}

char* file_path(char* infile)
{
	char path[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	size_t sizeInCharacters = 100;
	_splitpath_s(infile, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
	strcpy_s(path, _MAX_PATH, drive);
	strcat_s(path, _MAX_PATH, dir);
	return path;
}