#include "first_step.h"
#include "second_step.h"

int count_lines(char* str)
{
    FILE *infile;
    errno_t err;
    char c;
    int char_counter = 0;
    int count;
    err = fopen_s(&infile,str, "r");
	if (err != 0)
	{
		fprintf(stderr, "Error: opening ezer text failed\n");
		return -1;
	}
    else
    { 
        c = getc(infile);
        if (c == EOF)
        {
            fprintf(stderr, "Error: empty text\n");
            return -1;
        }
        else
        {
            count = 1;
            for (c ; c != EOF; c = getc(infile))
                if (c == '\n') // Increment count if this character is newline 
                    count++;
        }
    }
    return count;
}