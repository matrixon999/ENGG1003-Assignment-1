#ifndef _SUB_CIPHER_H
#define _SUB_CIPHER_H

#include "util.h"

int encrypt_substitution(char *plain_text, char **cipher_text, char key[26]);
int decrypt_substitution(char *cipher_text, char **plain_text, char key[26]);

int encrypt_substitution(char *plain_text, char **cipher_text, char key[26])
{
    // convert key to uppercase
    for(int i = 0; i < 26; i++)
    {
        if(key[i] < 65 || key[i] > 122 || (key[i] > 90 && key[i] < 97))
        {
            printf("ERROR: Key must contain alphabetic characters only.");
            return 1;
        }

        if(key[i] > 90) key[i] -= 32;
    }

    // we use the length of the string multiple times so throw it in a variable
    size_t string_length = strlen(plain_text);

    // allocate a buffer equal to the length of the plain text to hold the encrypted message
    char* cipher_text_buffer = (char*)malloc(string_length);
    // check to make sure the buffer was allocated, otherwise return error
    if(cipher_text_buffer == NULL)
    {
        printf("ERROR: Failed to allocate memory for cipher text.\n");
        return 1;
    }

    for(int i = 0; i < string_length; i++)
    {
        // if encounter not alphabetic character (eg. numbers, punctuation, whitespace), ignore it and move on
        if(!is_alpha(plain_text[i]))
        {
            cipher_text_buffer[i] = plain_text[i];
            continue;
        }

        cipher_text_buffer[i] = key[plain_text[i] - 65];
    }

    // if you don't terminate c strings you're going to have a bad time
    cipher_text_buffer[string_length] = '\0';

    // make the cipher_text parameter point to the created cipher_text_buffer
    *cipher_text = cipher_text_buffer;

    // encountered no errors, return 0 for success
    return 0;
}

int decrypt_substitution(char *cipher_text, char **plain_text, char key[26])
{
    // convert key to uppercase
    for(int i = 0; i < 26; i++)
    {
        if(key[i] < 65 || key[i] > 122 || (key[i] > 90 && key[i] < 97))
        {
            printf("ERROR: Key must contain alphabetic characters only.");
            return 1;
        }

        if(key[i] > 90) key[i] -= 32;
    }

    // we use the length of the string multiple times so throw it in a variable
    size_t string_length = strlen(cipher_text);

    // allocate a buffer equal to the length of the plain text to hold the encrypted message
    char* plain_text_buffer = (char*)malloc(string_length);
    // check to make sure the buffer was allocated, otherwise return error
    if(plain_text_buffer == NULL)
    {
        printf("ERROR: Failed to allocate memory for cipher text.\n");
        return 1;
    }

    for(int i = 0; i < string_length; i++)
    {
        // if encounter non alphabetic character (eg. numbers, punctuation, whitespace), ignore it and move on
        if(!is_alpha(cipher_text[i]))
        {
            plain_text_buffer[i] = cipher_text[i];
            continue;
        }


        for(int j = 0; j < 26; j++)
        {
            if(cipher_text[i] == key[j])
            {
                plain_text_buffer[i] = j + 65;
                break;
            }
        }   
    }

    // if you don't terminate c strings you're going to have a bad time
    plain_text_buffer[string_length] = '\0';

    // make the cipher_text parameter point to the created cipher_text_buffer
    *plain_text = plain_text_buffer;

    // encountered no errors, return 0 for success
    return 0;
}

#endif