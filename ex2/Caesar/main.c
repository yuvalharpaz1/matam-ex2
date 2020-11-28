#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <strsafe.h>
#include "create_thread.h"
#include "info_to_threads.h"
//final version
int main(int argc, char* argv[])
{
	// Info to threads using f_open
	
	int key, thread; 
	int char_counter = 0;
	char path[_MAX_PATH];
	
	if (argc != 5)
	{
		fprintf(stderr, "Error: invalid number of arguments (%d instead of 1)\n", argc - 1);
		return ERROR;
	}
	key = str2int(argv[2]);
	if (key < 0)
	{
		fprintf(stderr, "Error: invalid key\n");
		return ERROR;
	}
	if (arg_is_positive(argv[2][0])==FALSE)
	{				
		key = -1 * str2int(argv[2]);
	}	
	thread = str2int(argv[3]);
	if (thread < 1 ||arg_is_positive(argv[3][0])==FALSE)
	{
		fprintf(stderr, "Error: invalid thread\n");
		return ERROR;
	}
	printf("thread is:%d\n", thread);
	printf("before key is:%d\n", key);
	strcpy_s(path, _MAX_PATH, file_path(argv[1]));

	if (strcmp(argv[4],"-e")==0)
	{
		key = -key;
		strcat_s(path, _MAX_PATH, "encrypted.txt");
	}
	else if(strcmp(argv[4], "-d") == 0)
		strcat_s(path, _MAX_PATH, "decrypted.txt");
	else
	{
		fprintf(stderr, "Error: invalid encryption flag\n");
		return ERROR;
	}
	printf("after key is:%d\n", key);

	printf("path is:%s\n", path);

	int* threads = (int*)malloc(thread * sizeof(int));
	if (NULL == threads)
	{
		free(threads);
		fprintf(stderr, "Error: memory allocation failed\n");
		return -1;
	}

	location* locations = (location*)malloc(thread * sizeof(location));
	if (NULL == locations)
	{
		free(locations);
		fprintf(stderr, "Error: memory allocation failed\n");
		return ERROR;
	}

	first_run(&threads, argv[1], thread);
	
	locations = second_run(argv[1], path, threads, thread);
	if(locations==-1)
	{
		free(locations);
		fprintf(stderr, "Error in second run\n");
		return ERROR;
	}
	//// end of second open

	int retval;
	retval=create_threads(thread, argv[1], path, locations, key);
	if(retval!=0)
	{
		fprintf(stderr,"Could not create thread!\n");
		return ERROR;
	}

	free(locations);
	free(threads);
	return 0;
}
