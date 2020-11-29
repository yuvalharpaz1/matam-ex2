//  create_threads.c will include functions that will receive data from main.c and create 
//  threads according to it

#include "create_thread.h"

#pragma warning(push)
#pragma warning(disable:6386 6385)

//  A function that creates handles to threads simultaneity that read all the file in parallel
//  Parameters:2 integers, 2 strings and a pointer to locations (struct location) array 
//  Return: integer that evaluates if the function succeeded (0) or failed (-1)
int create_threads(int threads_num, char input_file[_MAX_PATH], char output_file[_MAX_PATH], location* locations, int key)
{
	HANDLE* p_thread_handles=(HANDLE*)malloc(threads_num * sizeof(HANDLE));
	if (NULL == p_thread_handles)
	{
		free(p_thread_handles);
		fprintf(stderr, "Error: memory allocation failed\n");
		return ERROR_CODE;
	}
	DWORD* p_thread_ids = (DWORD*)malloc(threads_num * sizeof(DWORD));
	if (NULL == p_thread_ids)
	{
		free(p_thread_ids);
		free(p_thread_handles);
		fprintf(stderr, "Error: memory allocation failed\n");
		return ERROR_CODE;
	}
	DWORD wait_code;
	BOOL ret_val;
	int i;
	thread_info* arguments=(thread_info*)malloc(threads_num * sizeof(thread_info));
	if (NULL == arguments)
	{
		free(arguments);
		free(p_thread_ids);
		free(p_thread_handles);
		fprintf(stderr, "Error: memory allocation failed\n");
		return ERROR_CODE;
	}
	// Create threads, each thread performs a part of the file.
	for(i=0;i<threads_num;i++)
	{
		strcpy_s(arguments[i].infile, _MAX_PATH, input_file);
		strcpy_s(arguments[i].outfile , _MAX_PATH, output_file);
		arguments[i].key = key;
		arguments[i].start = locations[i].start;
		arguments[i].end = locations[i].end;
		p_thread_handles[i] = create_thread_simple(translate_thread, &p_thread_ids[i], &arguments[i]);
	}
	printf("Handles successfully created!\n");

	DWORD exit_code;

	// Wait for multiple threads to receive exit command and terminate
	wait_code = WaitForMultipleObjects((DWORD)threads_num, p_thread_handles, TRUE, TIME_WAIT);
	switch (wait_code)
	{
	case WAIT_OBJECT_0:
		printf("success when waiting\n");
		break;
	case WAIT_TIMEOUT:
		printf("Error when waiting\n");
		for (int i = 0; i < threads_num; i++)
		{
			ExitThread(GetLastError());
			if (!GetExitCodeThread(p_thread_handles[i], &exit_code))
			{
				printf("Error when terminating\n");
				return ERROR_CODE;
				break;
			}
		}	
	default:
		printf("Error code is %x\n", wait_code);
	}
	// Close thread handles
	for (i = 0; i < threads_num; i++)
	{
		ret_val = CloseHandle(p_thread_handles[i]);
		if (false == ret_val)
		{
			printf("Error when closing\n");
			return ERROR_CODE;
		}
	}
	fprintf(stderr,"Handles successfully closed!\n");
	free(arguments);
	free(p_thread_handles);
	free(p_thread_ids);
	return 0;
}

//  A function that creates handle to thread with its arguments
//  Parameters: pointer to a function that notifies the host that a thread has started to execute, 
//  pointer to DWORD and a pointer to the arguments (struct thread_info) array 
//  Return: Handle to thread
static HANDLE create_thread_simple(LPTHREAD_START_ROUTINE p_start_routine, LPDWORD p_thread_id, thread_info *arguments)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread\n");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread\n");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	thread_handle =  CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		arguments,       /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		exit(ERROR_CODE);
	}

	return thread_handle;
}
