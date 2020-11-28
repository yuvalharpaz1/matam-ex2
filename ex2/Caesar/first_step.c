#include "first_step.h"


int count_lines(FILE* infile)
{
    char c;
    int char_counter = 0;
    int count;
    c = getc(infile);
    if (c == EOF)
    {
        fprintf(stderr, "Error: empty text\n");
        return -1;
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

/*void locations_of_lines(FILE* infile, location **line_locations, int lines)
{
    char c;
    int char_location = -1;
    c = getc(infile);
    if (c == EOF)
    {
        fprintf(stderr, "Error: empty text\n");
        lines = -1;
        return;
    }
    else
    {
        line_locations[0]->start = 0;
        lines = 1;
        for (c; c != EOF; c = getc(infile))
        {
            char_location++;
            if (c == '\n')
            {
                printf("hey\n");
                char_location++;
                line_locations[lines - 1]->end = char_location;
                lines++;
                if (NULL == (line_locations = (location*)realloc(line_locations, lines * sizeof(location))))
                {
                    free(line_locations);
                    fprintf(stderr, "Error: memory allocation failed\n");
                    lines = -1;
                    return;
                }
                line_locations[lines - 1]->start = char_location + 1;
            }
        }
    }
}*/