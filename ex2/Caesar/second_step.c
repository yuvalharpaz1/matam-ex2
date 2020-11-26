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

/*void find_char_location(int char_counter, int lines, int start, int end)
{

}*/