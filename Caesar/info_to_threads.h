#ifndef INFO_TO_THREADS_H
#define INFO_TO_THREADS_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hard_coded_data.h"

typedef struct Location
{
	int start;
	int end;
}location;

int first_run(int** threads, char* file_path, int thread_num);

location* second_run(char* in_path, char* out_path, int* threads, int thread);

void lines_in_thread(int* thread_array, int thread_num, int lines);

char* file_path(char* infile);

int count_lines(FILE* infile);

int str2int(char* str);

int arg_is_positive(char key);

#endif // !INFO_TO_THREADS_H

