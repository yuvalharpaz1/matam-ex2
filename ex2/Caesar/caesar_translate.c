#include "caesar_tranlate.h"

char char_type(char input_char)
{
	if (input_char >= 48 && input_char <= 57)
		return DIGIT;
	if (input_char >= 65 && input_char <= 90)
		return CAPITAL;
	if (input_char >= 97 && input_char <= 122)
		return SMALL;
	else return NULL;
}

char char_through_caesar(char input_char, int key)
{
	char output_char, type;
	type = char_type(input_char);
	switch (type)
	{
		case CAPITAL:
			output_char = CAPITAL + (input_char - CAPITAL - key+26) % 26;
			break;
		case SMALL:
			output_char = SMALL + (input_char - SMALL - key+26) % 26;
			break;
		case DIGIT:
			output_char = DIGIT + (input_char - DIGIT - key+10) % 10;
			break;
		default:
			output_char = input_char;
	}
	return output_char;
}

int str2int(char *str)
{
	char c;
    int result = 0;
    int sign = 1;
	int i = 0;
	if (str[0] == '-')
	{
		i++;
		sign = -1;
	}
    while(str[i]!='\0') 
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

int key_is_positive(char key)
{
	if (key == '-')
		return FALSE;
	else
		return TRUE;
}
 