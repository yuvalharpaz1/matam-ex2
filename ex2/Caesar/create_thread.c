// CreateThreadTidyDemonstration.c

/*
* This is the same program as Example03, written 
* 
* This program demonstrates simple thread usage.
* It solves the problem demonstrated in Example02 by running each task in
* it's own thread.
* By doing so, it delegates the problem of scheduling the tasks to the OS.
* Because the OS scheduler implenets a round robin mechanism, the tasks
* appear to be running in parallel.
*/

// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
// Constants -------------------------------------------------------------------

#define BRUTAL_TERMINATION_CODE 0x55
#define ERROR_CODE ((int)(-1))

// Function Declarations -------------------------------------------------------

/*
* A simplified API for creating threads.
* Input Arguments:
*   p_start_routine: A pointer to the function to be executed by the thread.
*     This pointer represents the starting address of the thread.
*     The function has to have this specific signature:
*       DWORD WINAPI FunctionName(LPVOID lpParam);
*     With FunctionName being replaced with the function's name.
* Output Arguments:
*   p_thread_id: A pointer to a variable that receives the thread identifier.
*     If this parameter is NULL, the thread identifier is not returned.
* Return:
*   If the function succeeds, the return value is a handle to the new thread.
*   If the function fails, the return value is NULL.
*   To get extended error information, call GetLastError.
* Notes:
*   This function is just a wrapper for CreateThread.
*/
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id);

// Function Definitions --------------------------------------------------------

int create_threads(int threads_num)
{
	HANDLE* p_thread_handles=(HANDLE*)malloc(threads_num * sizeof(HANDLE));
	if (NULL == p_thread_handles)
	{
		free(p_thread_handles);
		fprintf(stderr, "Error: memory allocation failed\n");
		exit(-1);
	}
	DWORD* p_thread_ids = (DWORD*)malloc(threads_num * sizeof(DWORD));
	if (NULL == p_thread_ids)
	{
		free(p_thread_ids);
		fprintf(stderr, "Error: memory allocation failed\n");
		exit(-1);
	}
	DWORD wait_code;
	BOOL ret_val;
	size_t i;
	// Create two threads, each thread performs on task.
	p_thread_handles[0] = CreateThreadSimple(IOThread, &p_thread_ids[0]);
	p_thread_handles[1] = CreateThreadSimple(CommunicationThread, &p_thread_ids[1]);

	// Wait for IO thread to receive exit command and terminate
	wait_code = WaitForSingleObject(p_thread_handles[0], INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting");
		return ERROR_CODE;
	}

	// Terminate the other thread
	// Normally, we would avoid terminating a thread so brutally,
	// because it might be in the middle of an operation that should not
	// be interrupted (like writing a file).
	// There are gentler ways of terminating a thread.
	ret_val = TerminateThread(p_thread_handles[1], BRUTAL_TERMINATION_CODE);
	if (false == ret_val)
	{
		printf("Error when terminating\n");
		return ERROR_CODE;
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
}

static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	thread_handle =  CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		NULL,            /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		exit(ERROR_CODE);
	}

	return thread_handle;
}
