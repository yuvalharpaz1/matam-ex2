#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <strsafe.h>
#include "caesar_tranlate.h"
#include "first_step.h"
#include "second_step.h"
#include "create_thread.h"

int main(int argc, char* argv[])
{
	// Info to threads using f_open
	
	int key, thread; 
	int lines=0;
	int char_counter = 0;
	char path[_MAX_PATH];
	
	if (argc != 4)
	{
		fprintf(stderr, "Error: invalid number of arguments (%d instead of 1)\n", argc - 1);
		return ERROR;
	}
	if (key_is_positive(argv[2][0]))
		key = str2int(argv[2]);
	else
		key = -1 * str2int(argv[2]);
	thread = str2int(argv[3]);
	if (thread < 1)
	{
		fprintf(stderr, "Error: invalid number of threads\n");
		return ERROR;
	}

	strcpy_s(path, _MAX_PATH, file_path(argv[1]));
	strcat_s(path, _MAX_PATH, "decrypted.txt");
	printf("%s\n", path);

	location* line_locations = (location*)malloc(sizeof(location));
	if (NULL == line_locations)
	{
		free(line_locations);
		fprintf(stderr, "Error: memory allocation failed\n");
		return -1;
	}

	int* threads = (int*)malloc(thread * sizeof(int));
	if (NULL == threads)
	{
		free(threads);
		fprintf(stderr, "Error: memory allocation failed\n");
		return -1;
	}

	FILE *infile;
	errno_t err;
	err = fopen_s(&infile, argv[1], "r");
	if (err != 0)
	{
		fprintf(stderr, "Error: opening ezer text failed\n");
		return -1;
	}
	else
	{
		lines = count_lines(infile);
		//locations_of_lines(infile, &line_locations, lines);
	}
	printf("%d\n", lines);
	if (lines == -1)
	{
		return ERROR;
	}
	/*for (int i = 0; i < thread; i++)
	{
		printf("%d %d\n", line_locations[i].start, line_locations[i].end);
	}*/
	lines_in_thread(threads, thread, lines);

	for (int i = 0; i < thread; i++)
	{
		printf("%d\n", threads[i]);
	}

	location* locations = (location*)malloc(thread * sizeof(location));
	if (NULL == locations)
	{
		free(locations);
		fprintf(stderr, "Error: memory allocation failed\n");
		return -1;
	}

	fclose(infile);
	
	///////////////////////////////////////////////////////

	HANDLE h_file, h_append;
	DWORD bytes_read, bytes_write;
	char read_buffer, translate;

	h_file = CreateFileA(argv[1],               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template
	if (h_file == INVALID_HANDLE_VALUE)
	{
		//DisplayError(TEXT("CreateFile"));
		//_tprintf(TEXT("Terminal failure: unable to open file \"%s\" for read.\n"), argv[1]);
		fprintf(stderr, "Could not open %s!.\n", argv[1]);
		CloseHandle(h_file);
		return ERROR;
	}
	// Open the existing file, or if the file does not exist,
	// create a new file.
	
	h_append = CreateFileA(path, // open mytestfiletwo.txt
		GENERIC_WRITE, // open for writing
		FILE_SHARE_WRITE, // share for writing
		NULL, // default security
		CREATE_ALWAYS, // open or create
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL); // no attribute template

	if (h_append == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Could not open %s!.\n", path);
		CloseHandle(h_append);
		CloseHandle(h_file);
		return ERROR;
	}

	for (int j = 0; j < argc; j++)
	{
		printf("%s\n", argv[j]);
	}
	SetFilePointer(h_file, 0, 0, FILE_BEGIN);
	//Read file
	locations[0].start = 0;
	int thread_num = 0;
	int line_counter = 0;
	do
	{
		if (FALSE == ReadFile(h_file, &read_buffer, 1, &bytes_read, NULL))
		{
			fprintf(stderr,"File not read.\n");
			CloseHandle(h_file);
			return ERROR;
		}
		else //if(bytes_read==1)
		{
			//fprintf(stderr,"File successfully read!\n");
			if (read_buffer == '\n' || bytes_read==0)
			{
				line_counter++;
				if (line_counter == threads[thread_num])
				{
					//printf("loop:\n");
					//printf("%d\n", char_counter);
					locations[thread_num].end = char_counter;
					line_counter = 0;
					thread_num++;
					if (thread_num < thread)
						locations[thread_num].start = char_counter + 1;
				}
			}
			if (bytes_read == 1)
			{
				//printf("%d\n", char_counter);
				char_counter++;
				/*translate = char_through_caesar(read_buffer, key);
				if (FALSE == WriteFile(h_append, &translate, 1, &bytes_write, NULL))
					fprintf(stderr, "File not written to.\n");
				else {
					//fprintf(stderr, "File successfully written to.\n");
				}*/
			}
		}
	} while (bytes_read != 0);
	SetFilePointer(h_file, 0, 0, FILE_BEGIN);
	for (int i = 0; i < thread; i++)
	{
		printf("%d ,%d\n", locations[i].start, locations[i].end);
	}
	printf("%d\n", char_counter);
	SetFilePointer(h_append, char_counter, 0, FILE_BEGIN);
	SetEndOfFile(h_append);

	if (CloseHandle(h_file) != 0)
		fprintf(stderr,"\n%s file handle closed successfully!\n", argv[1]);

	if (CloseHandle(h_append) != 0)
		fprintf(stderr, "%s file handle closed successfully!\n", path);

	//// end of second open

	int retval;
	retval=create_threads(thread, argv[1], path, locations, key);
	if(retval!=0)
	{
		fprintf(stderr,"Could not create thread!\n");
	}
	free(line_locations);
	free(locations);
	free(threads);
	return 0;
}
