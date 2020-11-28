#include "info_to_threads.h"

void first_run(int** threads, char* file_path, int thread_num)
{
	int lines=0;
	FILE *infile;
	errno_t err;
	err = fopen_s(&infile, file_path, "r");
	if (err != 0)
	{
		fprintf(stderr, "Error: opening ezer text failed\n");
		return ERROR;
	}
	else
	{
		lines = count_lines(infile);
	}
	printf("lines:%d\n", lines);
	if (lines == -1)
	{
		return ERROR;
	}

	lines_in_thread(*threads, thread_num, lines);

	fclose(infile);
}

location* second_run(char* in_path, char* out_path, int* threads, int thread)
{
	HANDLE h_file, h_append;
	DWORD bytes_read, bytes_write;
	char read_buffer, translate;
	location* locations = (location*)malloc(thread * sizeof(location));
	if (NULL == locations)
	{
		free(locations);
		fprintf(stderr, "Error: memory allocation failed\n");
		return ERROR;
	}

	h_file = CreateFileA(in_path,               // file to open
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
		fprintf(stderr, "Could not open %s!.\n", in_path);
		CloseHandle(h_file);
		return ERROR;
	}
	// Open the existing file, or if the file does not exist,
	// create a new file.

	h_append = CreateFileA(out_path, // open mytestfiletwo.txt
		GENERIC_WRITE, // open for writing
		FILE_SHARE_WRITE, // share for writing
		NULL, // default security
		CREATE_ALWAYS, // open or create
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL); // no attribute template

	if (h_append == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Could not open %s!.\n", out_path);
		CloseHandle(h_append);
		CloseHandle(h_file);
		return ERROR;
	}

	SetFilePointer(h_file, 0, 0, FILE_BEGIN);
	//Read file
	locations[0].start = 0;
	int thread_num = 0, line_counter = 0, char_counter = 0;
	do
	{
		if (FALSE == ReadFile(h_file, &read_buffer, 1, &bytes_read, NULL))
		{
			fprintf(stderr, "File not read.\n");
			CloseHandle(h_file);
			return ERROR;
		}
		else //if(bytes_read==1)
		{
			//fprintf(stderr,"File successfully read!\n");
			if (read_buffer == '\n' || bytes_read == 0)
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
				char_counter++;
			}
		}
	} while (bytes_read != 0);
	SetFilePointer(h_file, 0, 0, FILE_BEGIN);
	SetFilePointer(h_append, char_counter, 0, FILE_BEGIN);
	SetEndOfFile(h_append);

	if (CloseHandle(h_file) == 0)
		fprintf(stderr, "%s file handle not closed successfully!\n", in_path);

	if (CloseHandle(h_append) == 0)
		fprintf(stderr, "%s file handle not closed successfully!\n", out_path);
	return(locations);
}

void lines_in_thread(int* thread_array, int thread_num, int lines)
{
	int counter = 0;
	for (int i = 0; i < thread_num; i++)
	{
		thread_array[i] = 0;
	}
	while (counter < lines)
	{
		thread_array[counter % thread_num]++;
		counter++;
	}
}

char* file_path(char* infile)
{
	char path[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	size_t sizeInCharacters = 100;
	_splitpath_s(infile, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
	strcpy_s(path, _MAX_PATH, drive);
	strcat_s(path, _MAX_PATH, dir);
	return path;
}

int count_lines(FILE* infile)
{
	char c;
	int char_counter = 0;
	int count;
	c = getc(infile);
	if (c == EOF)
	{
		fprintf(stderr, "Error: empty text\n");
		return ERROR;
	}
	else
	{
		count = 1;
		for (c; c != EOF; c = getc(infile))
		{
			if (c == '\n') { // Increment count if this character is newline 
				count++;
			}
		}
	}
	return count;
}

int str2int(char* str)
{
	char c;
	int result = 0;
	int i = 0;
	if (str[0] == '-')
	{
		if (47 < str[1] < 58)
		{
			i++;
		}
		else
			return ERROR;
	}
	while (str[i] != '\0')
	{
		c = str[i];
		if (c >= '0' && c <= '9')
		{
			result *= 10;
			result += c - '0';
		}
		else
			break;
		i++;
	}
	return result;
}

int arg_is_positive(char key)
{
	if (key == '-')
		return FALSE;
	else
		return TRUE;
}