#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "caesar_tranlate.h"

DWORD WINAPI translate_thread(char* input_file, char* output_file, int start, int end, int key)
{
	HANDLE h_file, h_append;
	DWORD bytes_read, bytes_write;
	char read_buffer, translate;
	h_file = CreateFileA(input_file,               // file to open
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
		fprintf(stderr, "Could not open %s!.\n", input_file);
		return ERROR;
	}
	// Open the existing file, or if the file does not exist,
	// create a new file.

	h_append = CreateFileA(output_file, // open mytestfiletwo.txt
		GENERIC_WRITE, // open for writing
		FILE_SHARE_WRITE, // share for writing
		NULL, // default security
		OPEN_ALWAYS, // open or create
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL); // no attribute template

	if (h_append == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Could not open %s!.\n", output_file);
		return ERROR;
	}

	SetFilePointer(h_file, start, 0, FILE_BEGIN);
	SetFilePointer(h_append, start, 0, FILE_BEGIN);
	for (int i = 0; i <= end - start; i++)
	{
		if (FALSE == ReadFile(h_file, &read_buffer, 1, &bytes_read, NULL))
		{
			fprintf(stderr, "File not read.\n");
			CloseHandle(h_file);
			return ERROR;
		}
		else
		{
			translate = char_through_caesar(read_buffer, key);
			if (FALSE == WriteFile(h_append, &translate, 1, &bytes_write, NULL))
			{
				fprintf(stderr, "File not written to.\n");
				return ERROR;
			}
		}
	}
	CloseHandle(h_file);
	closehandle(h_append);
	return 0;
}