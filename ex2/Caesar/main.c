#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <strsafe.h>
#include "caesar_tranlate.h"
#include "first_step.h"
#include "second_step.h"

int main(int argc, char* argv[])
{
	HANDLE h_file, h_append;
	int key, thread, lines;
	int char_counter = 0;
	char read_buffer, translate, c, path[_MAX_PATH];
	DWORD bytes_read, bytes_write;
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
		return ERROR;
	}
	// Open the existing file, or if the file does not exist,
	// create a new file.
	strcpy_s(path, _MAX_PATH, file_path(argv[1]));
	strcat_s(path, _MAX_PATH, "decrypted.txt");
	printf("%s\n", path);
	h_append = CreateFileA("decrypted.txt", // open mytestfiletwo.txt
		GENERIC_WRITE, // open for writing
		FILE_SHARE_WRITE, // share for writing
		NULL, // default security
		OPEN_ALWAYS, // open or create
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL); // no attribute template

	if (h_append == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Could not open %s!.\n", "decrypted.txt");
		return ERROR;
	}
	for (int j = 0; j < argc; j++)
	{
		printf("%s\n", argv[j]);
	}

	location* locations=(location*)malloc(thread * sizeof(location));
	if (NULL == locations)
	{
		free(locations);
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
	lines=count_lines(argv[1]);
	printf("%d\n", lines);
	if (lines == -1)
	{
		return ERROR;
	}
	lines_in_thread(threads, thread, lines);
	
	for(int i=0;i<thread;i++)
	{
		printf("%d\n",threads[i]);
	}
	//Read file
	locations[0].start = 0;
	int thread_num = 0;
	int line_counter = 0;
	int i = 0;
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
				translate = char_through_caesar(read_buffer, key);
				if (FALSE == WriteFile(h_append, &translate, 1, &bytes_write, NULL))
					fprintf(stderr, "File not written to.\n");
				else {
					//fprintf(stderr, "File successfully written to.\n");
				}
			}
		}
		/*if (i == 102)
			i = i;
		i++;*/
	} while (bytes_read != 0);
	//printf("%d\n", i);

	/*for(int i=0;i<thread;i++)
	{
		printf("%d\n", locations[i].start, locations[i].end);
	}*/
	//printf("%d\n", char_counter);
	//printf("%d\n", threads[2]);
	free(threads);
	// Close both files.
	if (CloseHandle(h_file) != 0)
		fprintf(stderr,"\n%s file handle closed successfully!\n", argv[1]);
	if (CloseHandle(h_append) != 0)
		fprintf(stderr,"%s file handle closed successfully!\n", "decrypted.txt");
	return 0;
}
