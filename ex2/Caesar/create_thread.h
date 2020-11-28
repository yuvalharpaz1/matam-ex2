#ifndef CREATE_THREAD_H
#define CREATE_THREAD_H

// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include "translate_thread.h"
#include "second_step.h"

// Constants -------------------------------------------------------------------

#define BRUTAL_TERMINATION_CODE 0x55
#define ERROR_CODE ((int)(-1))
#define TIME_WAIT 20000

int create_threads(int threads_num, char* input_file, char* output_file, location* locations, int key);

static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPDWORD p_thread_id, thread_info *arguments);

#endif // !CREATE_THREAD_H

