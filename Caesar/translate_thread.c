//  translate_thread.c will include functions that will receive data from create_thread.c
//  and will translate the info according to the need in the main (key, d/e and so on)

#include "translate_thread.h"

//  A function that translates the threads simultaneity with the info from create_thread.c
//  Parameters: args in create_thread.c
//  Return: DWORD that evaluates if the function succeeded (0) or failed (-1)
DWORD WINAPI translate_thread(LPVOID lpParam)
{
	thread_info* thread_cmd = (thread_info*)lpParam;
	char* input_file = thread_cmd->infile;
	char* output_file = thread_cmd->outfile;
	int key = thread_cmd->key;
	int start = thread_cmd->start;
	int end = thread_cmd->end;
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
		fprintf(stderr, "Could not open %s!.\n", input_file);
		return ERROR_CODE;
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
		return ERROR_CODE;
	}
	SetFilePointer(h_file, start, 0, FILE_BEGIN);
	SetFilePointer(h_append, start, 0, FILE_BEGIN);
	if(end>=0)
	{
		for (int i = 0; i <= end - start; i++)
		{
			if (FALSE == ReadFile(h_file, &read_buffer, 1, &bytes_read, NULL))
			{
			fprintf(stderr, "File not read.\n");
			CloseHandle(h_file);
			return ERROR_CODE;
			}
			else if (bytes_read==1)
			{
			
				translate = char_through_caesar(read_buffer, key);
				if (FALSE == WriteFile(h_append, &translate, 1, &bytes_write, NULL))
				{
					fprintf(stderr, "File not written to.\n");
					return ERROR_CODE;
				}
			}
		}
	}
	// Close both files.
	if (CloseHandle(h_file) == 0)
		fprintf(stderr,"\n%s file handle not closed successfully!\n", input_file);
	if (CloseHandle(h_append) == 0)
		fprintf(stderr,"%s file handle not closed successfully!\n", output_file);
	return 0;
}

//  A function that evaluates if the char is digit/captial/small or special char
//  Parameters: input char
//  Return: char that indicates the type of the char
char char_type(char input_char)
{
	if (input_char >= 48 && input_char <= 57)
		return DIGIT;
	if (input_char >= 65 && input_char <= 90)
		return CAPITAL;
	if (input_char >= 97 && input_char <= 122)
		return SMALL;
	else return ERROR_CODE;
}

//  A function that translates the char according to the key
//  Parameters: input char and integer (key)
//  Return: the translated char according to his key and type
char char_through_caesar(char input_char, int key)
{
	char output_char, type;
	type = char_type(input_char);
	switch (type)
	{
	case CAPITAL:
		output_char = CAPITAL + (input_char - CAPITAL - key + 26) % 26;
		break;
	case SMALL:
		output_char = SMALL + (input_char - SMALL - key + 26) % 26;
		break;
	case DIGIT:
		output_char = DIGIT + (input_char - DIGIT - key + 10) % 10;
		break;
	default:
		output_char = input_char;
	}
	return output_char;
}