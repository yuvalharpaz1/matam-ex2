#ifndef TRANSLATE_THREAD_H
#define TRANSLATE_THREAD_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "hard_coded_data.h"

typedef struct Thread_Info
{
	char infile[_MAX_PATH];
	char outfile[_MAX_PATH];
	int start;
	int end;
	int key;

}thread_info;

DWORD WINAPI translate_thread(LPVOID lpParam);

char char_type(char input_char);

char char_through_caesar(char input_char, int key);

#endif