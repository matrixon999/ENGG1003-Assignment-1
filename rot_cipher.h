#ifndef _ROT_CIPHER_H
#define _ROT_CIPHER_H

#include "util.h"

// function declarations. Not needed to compile, but you know, good practice
int encrypt_rotation(char *plain_text, char **cipher_text, int key);
int decrypt_rotation(char *cipher_text, char **plain_text, int key);
int break_rotation_cipher(char *cipher_text, char **plain_text, int *key);

// this function will take plain text and encrypt it with the provided key using the rotation cipher algorithm,
// on success it will return 0, and the cipher text will be stored in the cipher_text parameter
int encrypt_rotation(char *plain_text, char **cipher_text, int key)
{
    // because c modulo (%) is not real modulo, it doesn't deal well with negative numbers
    // this will shift the key to a positive that will have the same effect
    while(key < 0) key += 26;

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

    // loop over every character in the plain text, apply (m + k)(mod 26), and place the encrypted character
    // in the new cipher text buffer
    for(int i = 0; i < string_length; i++)
    {
        // if encounter not alphabetic character (eg. numbers, punctuation, whitespace), ignore it and move on
        if(plain_text[i] < 65 || plain_text[i] > 122 || (plain_text[i] > 90 && plain_text[i] < 97))
        {
            cipher_text_buffer[i] = plain_text[i];
            continue;
        }

        // calculate a normalisation value based on whether it is a lower case or upper case character
        int normalize_value = (plain_text[i] < 97) ? 65 : 97;
        // m + k where m is normalised to 0-26 by subtracting the normalise value
        char e = plain_text[i] - 65 + key;
        // assign the new value to the buffer
        cipher_text_buffer[i] = e % (26) + normalize_value;
    }

    // if you don't terminate c strings you're going to have a bad time
    cipher_text_buffer[string_length] = '\0';

    // make the cipher_text parameter point to the created cipher_text_buffer
    *cipher_text = cipher_text_buffer;

    // encountered no errors, return 0 for success
    return 0;
}

// this function will take cipher text and decrypt it with the provided key using the rotation cipher algorithm,
// on success it will return 0, and the plain text will be stored in the plain_text parameter
int decrypt_rotation(char *cipher_text, char **plain_text, int key)
{
    // because c modulo (%) is not real modulo, it doesn't deal well with negative numbers
    // this will shift the key to a positive that will have the same effect
    while(key < 0) key += 26;

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

    // loop over every character in the plain text, apply (m + k)(mod 26), and place the encrypted character
    // in the new cipher text buffer
    for(int i = 0; i < string_length; i++)
    {
        // if encounter not alphabetic character (eg. numbers, punctuation, whitespace), ignore it and move on
        if(cipher_text[i] < 65 || cipher_text[i] > 122 || (cipher_text[i] > 90 && cipher_text[i] < 97))
        {
            plain_text_buffer[i] = cipher_text[i];
            continue;
        }

        // calculate a normalisation value based on whether it is a lower case or upper case character
        int normalize_value = (cipher_text[i] < 97) ? 65 : 97;
        // m + k where m is normalised to 0-26 by subtracting the normalise value
        char e = cipher_text[i] - 65 - key;
        // if we go below zero, add 26 so it plays nicely with modulo
        if(e < 0) e += 26;
        // assign the new value to the buffer
        plain_text_buffer[i] = e % (26) + normalize_value;
    }

    // if you don't terminate c strings you're going to have a bad time
    plain_text_buffer[string_length] = '\0';

    // make the cipher_text parameter point to the created cipher_text_buffer
    *plain_text = plain_text_buffer;

    // encountered no errors, return 0 for success
    return 0;
}

int break_rotation_cipher(char *cipher_text, char **plain_text, int *key)
{
    printf("Attempting to break rotation cipher for message: %s\n", cipher_text);

    // QUICK NOTE (DELETE): make this a measure of percentage correct overall
    int record_key = -1;
    float record_similarity = -1;

    // loop 0-25, generate all strings, find one with most real words, return that with key
    for(int i = 0; i < 26; i++)
    {
        printf("Trying key %d\n", i);
        char *plain_text_tmp;
        int result = decrypt_rotation(cipher_text, &plain_text_tmp, i);
        // if failed, just skip to next key
        if(result != 0) continue;

        printf("Trying key %d: %s\n", i, plain_text_tmp);

        char** words;
        int word_count = string_parse(plain_text_tmp, &words);
        int correct_words = 0;
        for(int j = 0; j < word_count; j++)
        {
            printf("Checking if word '%s' is in list\n", words[j]);
            bool result = is_in_word_list2(words[j]);
            printf("'%s' is%s in list\n", words[j], result ? "" : " not");
            if(result) correct_words++;
        }
        
        float similarity = correct_words / word_count;
        if(record_similarity < similarity)
        {
            record_similarity = similarity;
            record_key = i;
        }
    }

    if(record_key == -1)
    {
        printf("Error: Unable to decrypt cipher text\n");
        return 1;
    }

    return 0;
}

#endif