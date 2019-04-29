#ifndef _SUB_CIPHER_H
#define _SUB_CIPHER_H

#include "util.h"

int encrypt_substitution(char *plain_text, char **cipher_text, char key[26]);
int decrypt_substitution(char *cipher_text, char **plain_text, char key[26]);
int break_substitution_cipher(char *cipher_text, char **plain_text, char *key[26]);

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

// comparison function for sorting algorithm
int compare_float(const void *a, const void *b)
{
    float arg1 = *(const float*)a;
    float arg2 = *(const float*)b;

    if(arg1 < arg2) return 1;
    if(arg1 > arg2) return -1;
    return 0;
}

int break_substitution_cipher(char *cipher_text, char **plain_text, char *key[26])
{
    // english letter frequencies taken from wikipedia
    float english_frequencies[26] = {8.167f, 1.492f, 2.782f, //abc
                                     4.253f, 12.702f, 2.228f, //def
                                     2.015f, 6.094f, 6.966f, //ghi
                                     0.153f, 0.772f, 4.025f, //jkl
                                     2.406f, 6.749f, 7.507f, //mno
                                     1.929f, 0.095f, 5.987f, //pqr
                                     6.327f, 9.056f, 2.758f, //stu
                                     0.978f, 2.360f, 0.150f, //vwx
                                     1.974f, 0.074f}; //yz
    char ordered_letters[26] = {'E', 'T', 'A',
                                'O', 'I', 'N',
                                'S', 'H', 'R',
                                'D', 'L', 'C',
                                'U', 'M', 'W',
                                'F', 'G', 'Y',
                                'P', 'B', 'V',
                                'K', 'J', 'X',
                                'Q', 'Z'};
    // first get letter frequencies
    float frequencies[26];

    for(int i = 'A'; i <= 'Z'; i++)
    {
        frequencies[i - 65] = letter_frequency(i, cipher_text) * 100.0f;
    }

    // temp key
    char *key_tmp = malloc(26 + 1);
    // zero initialise it
    memset(key_tmp, '\0', 26 + 1);

    // for each letter, find the closest english frequency and make that they sub letter
    for(int i = 0; i < 26; i++)
    {
        // keep track of candidate with least difference between frequencies
        float candidate_difference = 100.0f;
        char best_candidate = 0;
        for(int j = 0; j < 26; j++)
        {
            float difference = fabsf(frequencies[i] - english_frequencies[j]);
            if(difference < candidate_difference) {
                candidate_difference = difference;
                best_candidate = j;
            }
        }

        key_tmp[i] = best_candidate + 65;
    }

    printf("Key candidate1: %s\n", key_tmp);

    char *plain_text_tmp;
    int result = decrypt_substitution(cipher_text, &plain_text_tmp, key_tmp);
    printf("plain_text_tmp: %s\n", plain_text_tmp);

    // sort english letters by frequency
    float sorted_english[26];
    for(int i = 0; i < 26; i++) sorted_english[i] = english_frequencies[i];
    qsort(sorted_english, 26, sizeof(float), compare_float);

    // sort my letters by frequency
    float sorted_mine[26];
    for(int i = 0; i < 26; i++) sorted_mine[i] = frequencies[i];
    qsort(sorted_mine, 26, sizeof(float), compare_float);

    // for each letter in sorted frequency
    for(int i = 0; i < 26; i++)
    {
        // for each frequency in english frequencies
        for(int j = 0; j < 26; j++)
        {
            // if letter and frequency match, letter most likely this.
            if(sorted_english[i] == english_frequencies[j])
            {
                key_tmp[i] = ordered_letters[j];
                break;
            }
        }
    }

    printf("Key candidate2: %s\n", key_tmp);

    char *plain_text_tmp2;
    result = decrypt_substitution(cipher_text, &plain_text_tmp2, key_tmp);
    printf("plain_text_tmp: %s\n", plain_text_tmp2);

    //*key = key_tmp;
    //*plain_text = plain_text_tmp;

    return 0;
}

#endif