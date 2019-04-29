#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// enum typedefs to make it easier to read what is going on
typedef enum
{
    Invalid,
    Encrypt,
    Decrypt,
    Break,
} Mode;

typedef enum
{
    Rotation,
    Substitution,
} Cipher;

// struct to hold information about commands given by input file
typedef struct
{
    Mode mode;
    Cipher cipher;
    char *message;
    char sub_key[26];
    int rot_key;
} FileCommand;

#ifdef _WIN32
size_t getline(char **lineptr, size_t *n, FILE *stream);
#endif
int to_uppercase(char** input);
bool is_alpha(char c);
int get_int_input();
char* get_string_input();
char* get_ustring_input();
FileCommand read_command_file(char *filename);
bool is_in_word_list(char* word);
int string_parse(char *inp, char **array_of_words_p[]);
int split_by_first_word(char *in_string, char **out_string1, char **out_string2);

#ifdef _WIN32
// so apparently windows doesn't have getline. What in the ass is wrong with microsoft?!
// wrote this myself, I fully expect it to break. But it works on my machine.
// addendum, i lied, it doesn't work on my machine either, it just breaks sometimes, don't know why
// i'm going to try crying all over my keyboard, see if that works...
size_t getline(char **lineptr, size_t *n, FILE *stream)
{
    if(feof(stream))
    {
        return -1;
    }

    int i = 0;
    char *buffer = malloc(i);

    int c;
    while(c = fgetc(stream))
    {
        if(c == EOF) break;
        //if(c == '\n') break;
        //if(c == '\r') break;

        i++;
        buffer = realloc(buffer, i);

        buffer[i-1] = c;

        if(c == '\n') break;
    }

    fseek(stream, i, ftell(stream));

    *n = i;
    buffer[i] = '\0';

    *lineptr = buffer;

    return i - 1;
}

#endif

// function to convert string to all uppercase
int to_uppercase(char** input)
{
    // for each item (letter) in array (string)
    for(int i = 0; i < strlen(*input); i++)
    {
        // make sure item is an ASCII letter
        if((*input)[i] < 65 || (*input)[i] > 122 || ((*input)[i] > 90 && (*input)[i] < 97))
        {
            // if find non alphabetic character, error and return
            //printf("ERROR: Input must contain alphabetic characters only.");
            //return 1;
            continue;
        }

        // if lower case value, subtract 32 to make upper case
        if((*input)[i] > 90) (*input)[i] -= 32;
    }

    // return success
    return 0;
}

// returns true if given character is an alphabetic ASCII character
bool is_alpha(char c)
{
    // if within ranges of ASCII letters
    return !(c < 65 || c > 122 || (c > 90 && c < 97));
}

// gets input from user of type int
int get_int_input()
{
    int input;
    int have_valid = 0;
    // loop until valid input is given
    while(have_valid == 0)
    {
        // use scanf with %d to get integer and store in variable
        int result = scanf("%d", &input);
        // if no valid input given, alert and restart
        if(result != 1)
        {
            printf("Invalid input given. Must be an integer. Try again.\n");
        }
        else
        {
            have_valid = 1;
        }

        // make sure nothing is left in stdin because it's a massive jerk and loves to hold on to StUFf aND THEN RuIN EVERYTHING AFTER IT!!!!
        // so yeah, make sure that doesn't happen
        // also doesn't work if you compile under visual studio because microsoft still thinks people
        // use digital typewriters so they add a carriage return to newlines like fucking idiots
        fflush(stdin);
        getchar();
    }

    return input;
}

// gets input string from user
// max size is 1024, probably could change to use getchar(), but if you're going to be typing that much into the console, you're insane,
// and yes you can copy and paste into the console, but.. gross... stop it. Use the file input method.
char* get_string_input()
{
    // allocate string of 1024 characters
    char *str = malloc(1024);
    // use scanf to get string. %[^\n]s makes sure that whitespace is handled correctly
    int result = scanf("%[^\n]s", str);
    // if didn't receive input, make sure buffer has empty string, otherwise bad things happen
    if(str <= 0)
    {
        strcpy(str, "");
    }

    // see above
    fflush(stdin);
    getchar();
    
    return str;
}

// same as get_string_input(), but it also converts to uppercase
char* get_ustring_input()
{
    char* str = get_string_input();
    to_uppercase(&str);
    return str;
}

FileCommand read_command_file(char *filename)
{
    FileCommand command;
    // open file
    FILE *f = fopen(filename, "r");
    // check file opened correctly
    if(f == NULL)
    {
        // if not, alert user and return invalid
        printf("Failed to open file: %s\n", filename);
        command.mode = Invalid;
        return command;
    }

    // variables for reading line by line
    char *line;
    size_t length = 0;

    // loop over each line
    while(getline(&line, &length, f) != -1)
    {
        // getline leaves \r and \n characters on the end
        // this gets rid of them
        int len = strlen(line);
        if(line[len-2] == '\r') line[len-2] = '\0';
        if(line[len-1] == '\n') line[len-1] = '\0';

        // variables for line format of {argument type} {argument value}, e.g. 'mode encrypt'
        char *arg;
        char *value;
        split_by_first_word(line, &arg, &value);

        // handle argument 'mode'
        if(strcmp(arg, "mode") == 0)
        {
            if(strcmp(value, "encrypt") == 0) command.mode = Encrypt;
            else if(strcmp(value, "decrypt") == 0) command.mode = Decrypt;
            else if(strcmp(value, "break") == 0) command.mode = Break;
        }
        // handle argument 'cipher'
        else if(strcmp(arg, "cipher") == 0)
        {
            if(strcmp(value, "rotation") == 0) command.cipher = Rotation;
            else if(strcmp(value, "substitution") == 0) command.cipher = Substitution;
        }
        // handle argument 'rot_key'
        else if(strcmp(arg, "rot_key") == 0)
        {
            // key was read in as string, attempt to parse as integer
            int rot_result = sscanf(value, "%d", &command.rot_key);
            // if failed, given key probably wasn't valid, or some other error occurred, abort
            if(rot_result != 1)
            {
                printf("Invalid Rotation cipher key given. Aborting...\n");
                command.mode = Invalid;
                return command;
            }
        }
        // handle argument 'sub_key'
        else if(strcmp(arg, "sub_key") == 0)
        {
            if(strlen(value) != 26)
            {
                printf("Given substitution cipher key not valid. It must be 26 characters long. Aborting...\n");
                command.mode = Invalid;
                return command;
            }
            strcpy(command.sub_key, value);
        }
        // handle argument 'message'
        else if(strcmp(arg, "message") == 0)
        {
            command.message = value;
        }
    }

    // maybe add some checks to filecommand to make sure everything lines up.
    // could just rely on file input being correct, but banking on users doing the right thing is a bad idea

    fclose(f);

    printf("Successfully read file.\n");
    return command;
}

bool is_in_word_list(char* word)
{
    FileCommand command;
    // open file
    FILE *f = fopen("words_alpha.txt", "r");
    // check file opened correctly
    if(f == NULL)
    {
        // if not, alert user and return invalid
        printf("Failed to open file: %s\n", "words_alpha.txt");
        return false;
    }

    // variables for reading line by line
    char *line;
    size_t length = 0;

    // loop over each line
    while(getline(&line, &length, f) != -1)
    {
        //printf("%s - %s\n", line, word);
        if(strcmp(line, word) == 0) return true;
    }

    fclose(f);

    return false;
}

bool is_in_word_list2(char* word)
{
    FileCommand command;

    FILE *f = fopen("words_alpha.txt", "r");

    if(f == NULL)
    {
        printf("Failed to open file: %s\n", "words_alpha.txt");
        return false;
    }

    char *line;
    size_t length = 0;

    // for each word
    while(getline(&line, &length, f) != -1)
    {
        // getline leaves \r and \n characters on the end
        // this gets rid of them
        int len = strlen(line);
        if(line[len-2] == '\r') line[len-2] = '\0';
        if(line[len-1] == '\n') line[len-1] = '\0';

        // make sure the word is uppercase
        to_uppercase(&line);

        //printf("%s - %s\n", word, line);

        // if the words are different lengths, it's not that one
        if(strlen(line) != strlen(word)) continue;

        // used for continuing outer loop
        bool continue_outer_loop = false;

        // for each letter
        for(int i = 0; i < strlen(word); i++)
        {
            // if letters are different, not that one
            if(line[i] != word[i])
            {
                // need to continue outer loop so set true and break from this loop
                continue_outer_loop = true;
                break;
            }
        }

        // if we set continue outer loop, do so
        if(continue_outer_loop) continue;

        fclose(f);
        return true;
    }

    fclose(f);

    return false;
}

int string_parse(char *inp, char **array_of_words_p[]) {
  // copy string to a local variable for manipulation
    char* str = malloc(strlen(inp) + 1);
    strcpy(str, inp);

    // start off with just a pointer
    char** array_of_words = malloc(0);

    // delimiter is just a single space
    char* delim = " ";

    // first call to strtok
    char* tok = strtok(str, delim);

    int string_count = 0;

    // if tok isn't NULL, we have a string
    while(tok != 0)
    {
      string_count++;
      // reallocate memory to allow for another char* to be added.
      array_of_words = realloc(array_of_words, sizeof(char*) * string_count);

      // allocate tok in new_string on the heap
      char* new_string = malloc(strlen(tok));
      // copy the string to the new allocating
      strcpy(new_string, tok);
      // add new_string to the array of words
      array_of_words[string_count-1] = new_string;

      tok = strtok(NULL, delim);
    }

    // assign input pointer
    *array_of_words_p = array_of_words;

    return string_count;
}

int split_by_first_word(char *in_string, char **out_string1, char **out_string2)
{
    // find first space
    int index = 0;
    while(in_string[index++] != ' ');
    // allocate strings
    char *tmp1 = malloc(index);
    char *tmp2 = malloc(strlen(in_string) - index);

    // copy data to strings
    strncpy(tmp1, in_string, index-1);
    strncpy(tmp2, &in_string[index], strlen(in_string) - index + 1);

    // assign data to input pointers
    *out_string1 = tmp1;
    *out_string2 = tmp2;

    return 0;
}

// this function returns the frequency of a letter in a string as a number between 1 and 0
float letter_frequency(char c, char *str)
{
    int str_length = strlen(str);

    int count = 0;

    for(int i = 0; i < str_length; i++)
    {
        if(str[i] == c) count++;
    }

    return (float)count / (float)str_length;
}

#endif