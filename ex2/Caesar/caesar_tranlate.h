#ifndef CAESAR_TRANSLATE_H
#define CAESAR_TRANSLATE_H

#include <stdio.h>
#include <string.h>

#define CAPITAL 'A'
#define SMALL 'a'
#define DIGIT '0'
#define TRUE 1
#define FALSE 0

char char_type(char input_char);

char char_through_caesar(char input_char, int key);

int str2int(char* key);

int key_is_positive(char key);

#endif // CAESAR_TRANSLATE_H

