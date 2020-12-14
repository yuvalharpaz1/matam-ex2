#ifndef CREATE_THREAD_H
#define CREATE_THREAD_H

// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include "info_to_threads.h"
#include "translate_thread.h"
#include "hard_coded_data.h"

// Constants -------------------------------------------------------------------

int create_threads(int threads_num, char input_file[_MAX_PATH], char output_file[_MAX_PATH], location* locations, int key);

static HANDLE create_thread_simple(LPTHREAD_START_ROUTINE p_start_routine, LPDWORD p_thread_id, thread_info *arguments);

#endif // !CREATE_THREAD_H

