#ifndef SECOND_STEP_H
#define SECOND_STEP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -1

typedef struct Location
{
	int start;
	int end;
}location;

void check_alloc_int(int* str);

void lines_in_thread(int* array, int thread_num, int lines);

char* file_path(char* infile);

#endif // !SECOND_STEP_H

