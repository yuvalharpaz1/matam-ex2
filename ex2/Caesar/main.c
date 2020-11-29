//  written by Yuval Harpaz, ID 318849817, and Adar Gur, ID 318853868

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <strsafe.h>
#include "create_thread.h"
#include "info_to_threads.h"

//final version
int main(int argc, char* argv[])
{	
	int key, thread; 
	int char_counter = 0;
	char path[_MAX_PATH];
	
	if (argc != 5)
	{
		fprintf(stderr, "Error: invalid number of arguments (%d instead of 1)\n", argc - 1);
		return ERROR_CODE;
	}
	key = str2int(argv[2]);
	if (key < 0)
	{
		fprintf(stderr, "Error: invalid key\n");
		return ERROR_CODE;
	}
	if (arg_is_positive(argv[2][0])==FALSE)
	{				
		key = -1 * str2int(argv[2]);
	}	
	thread = str2int(argv[3]);
	if (thread < 1 ||arg_is_positive(argv[3][0])==FALSE)
	{
		fprintf(stderr, "Error: invalid thread\n");
		return ERROR_CODE;
	}
	strcpy_s(path, _MAX_PATH, file_path(argv[1]));

	if (strcmp(argv[4],"-e")==0)
	{
		key = -key;
		strcat_s(path, _MAX_PATH, ENCRYPTION);
	}
	else if(strcmp(argv[4], "-d") == 0)
		strcat_s(path, _MAX_PATH, DECRYPTION);
	else
	{
		fprintf(stderr, "Error: invalid encryption flag\n");
		return ERROR_CODE;
	}
	int* threads = (int*)malloc(thread * sizeof(int));
	if (NULL == threads)
	{
		free(threads);
		fprintf(stderr, "Error: memory allocation failed\n");
		return ERROR_CODE;
	}

	location* locations = (location*)malloc(thread * sizeof(location));
	if (NULL == locations)
	{
		free(locations);
		free(threads);
		fprintf(stderr, "Error: memory allocation failed\n");
		return ERROR_CODE;
	}

	if (first_run(&threads, argv[1], thread) != 0) //  first run on input file
	{	
		free(locations);
		free(threads);
		return ERROR_CODE;
	}
	locations = second_run(argv[1], path, threads, thread); //  second run on input file
	if(locations==-1)
	{
		free(locations);
		free(threads);
		fprintf(stderr, "Error in second run\n");
		return ERROR_CODE;
	}

	int retval;
	retval=create_threads(thread, argv[1], path, locations, key); //create the threads simultaneity

	if(retval!=0)
	{
		fprintf(stderr,"Could not create thread!\n");
		free(locations);
		free(threads);
		return ERROR_CODE;
	}
	free(locations);
	free(threads);
	return 0;
}//end