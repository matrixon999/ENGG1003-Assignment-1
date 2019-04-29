#include "rot_cipher.h"
#include "sub_cipher.h"

void handle_cmdline(int argc, char *argv[]);
int handle_file_input(char *filename);
void rot_menu();
void sub_menu();
void help_menu();
void main_menu();
void break_menu();

// this function displays the help menu, obviously, but the spec says put a comment above each function
void help_menu()
{
    printf("ENGG1003 - Assignment 1 - Mathew Rixon - C3274723\n");
    printf("Press Ctrl+C to quit at any time\n");
    printf("Or choose the help option to display this menu again\n");
    printf("\n");
    printf("Input number of menu item and press Enter:\n");
    printf("1. Rotation Cipher\n");
    printf("2. Substitution Cipher\n");
    printf("3. Break Cipher\n");
    printf("4. Help\n");
    printf("5. File Input\n");
    printf("6. Quit\n");
}

void rot_menu()
{
    // loop, stay in this menu until back option is chosen
    while(true)
    {
        printf("Rotation Cipher\n");
        printf("1. Encrypt\n");
        printf("2. Decrypt\n");
        printf("4. Back\n");

        // get menu choice from user
        int input = get_int_input();
        printf("\n");

        // handle choice
        switch(input)
        {
            // chose encrypt
            case 1:
            {
                // request and receive key
                printf("Please input key for Rotation Cipher: ");
                int key = get_int_input();
                // request and receive input text
                printf("Please input plain text to encrypt: ");
                char *plain_text = get_ustring_input();

                // create variable to store cipher text
                char *cipher_text;
                // encrypt and store in variable
                int result = encrypt_rotation(plain_text, &cipher_text, key);
                // check encryption was successful
                if(result != 0)
                {
                    // if error, alert user
                    printf("Error: Encryption failed\n");
                    //printf("Returning to main menu\n\n");
                }
                else
                {
                    // on success, print cipher text
                    printf("Cipher text: %s\n", cipher_text);
                }

                // free memory
                free(plain_text);
                free(cipher_text);

            } break;

            // chose decrypt
            case 2:
            {
                // request and receive key
                printf("Please input key for Rotation Cipher: ");
                int key = get_int_input();
                // request and receive input text
                printf("Please input cipher text to decrypt: ");
                char *cipher_text = get_ustring_input();

                // create variable to store plain text
                char *plain_text;
                // decrypt and store in variable
                int result = decrypt_rotation(cipher_text, &plain_text, key);
                // check decryption was successful
                if(result != 0)
                {
                    // if error, alert user
                    printf("Error: Decryption failed\n");
                    //printf("Returning to main menu\n\n");
                }
                else
                {
                    // on success, print plain text
                    printf("Plain text: %s\n", plain_text);
                }

                // free memory
                free(cipher_text);
                free(plain_text);

            } break;

            // back chosen, return to main menu
            case 4: return;

            // invalid input chosen, alert user
            default: printf("Invalid input. Try again\n\n");
        }
        printf("\n");
    }
}

void sub_menu()
{
    // loop, stay in this menu until back option is chosen
    while(true)
    {
        printf("Substitution Cipher\n");
        printf("1. Encrypt\n");
        printf("2. Decrypt\n");
        printf("3. Back\n");

        // get menu choice from user
        int input = get_int_input();
        printf("\n");

        // handle choice
        switch(input)
        {
            // chose encrypt
            case 1:
            {
                // request and receive key
                printf("Please input key string for Substitution Cipher: ");
                char *key = get_ustring_input();
                // request and receive input text
                printf("Please input plain text to encrypt: ");
                char *plain_text = get_ustring_input();

                // create variable to store cipher text
                char *cipher_text;
                // encrypt and store in variable
                int result = encrypt_substitution(plain_text, &cipher_text, key);
                // check encryption was successful
                if(result != 0)
                {
                    // if error, alert user
                    printf("Error: Encryption failed\n");
                    //printf("Returning to main menu\n\n");
                }
                else
                {
                    // on success, print cipher text
                    printf("Cipher text: %s\n", cipher_text);
                }

                // free memory
                free(plain_text);
                free(cipher_text);

            } break;

            // chose decrypt
            case 2:
            {
                // request and receive key
                printf("Please input key string for Substitution Cipher: ");
                char *key = get_string_input();
                // request and receive input text
                printf("Please input cipher text to decrypt: ");
                char *cipher_text = get_ustring_input();

                // create variable to store plain text
                char *plain_text;
                // decrypt and store in variable
                int result = decrypt_substitution(cipher_text, &plain_text, key);
                // check decryption was successful
                if(result != 0)
                {
                    // if error, alert user
                    printf("Error: Decryption failed\n");
                    //printf("Returning to main menu\n\n");
                }
                else
                {
                    // on success, print plain text
                    printf("Plain text: %s\n", plain_text);
                }

                // free memory
                free(cipher_text);
                free(plain_text);

            } break;

            // back chosen, return to main menu
            case 3: return;

            // invalid input chosen, alert user
            default: printf("Invalid input. Try again\n\n");
        }
        printf("\n");
    }
}

// menu for guiding user through breaking ciphers
// most of this should be pretty self explanaroty
void break_menu()
{
    printf("This menu will assist in breaking a block of cipher text for which details such as the key and cipher algorithm may be unknown.\n");

    // get cipher text
    printf("Please enter cipher text:\n");
    char *cipher_text = get_ustring_input();

    // ask if user knows cipher used
    printf("\n");
    printf("What cipher algorithm is it encrypted with?\n");
    printf("1. Rotation\n");
    printf("2. Substitution\n");
    printf("3. Unknown\n");
    int cipher_choice = get_int_input();

    // based on whether cipher is known, chose best option
    switch(cipher_choice)
    {
        // if user knows rotation was used
        case 1:
        {
            // run break cipher function
            char *plain_text;
            int key;
            int result = break_rotation_cipher(cipher_text, &plain_text, &key);
            // if successful, print results
            if(result == 0)
            {
                printf("Got it!\n");
                printf("Key: %d\n", key);
                printf("Message: %s\n", plain_text);
                free(plain_text);
            }
            // if unsuccessful, just print all possibilities
            else
            {
                printf("Couldn't get it. So here's all of them, you decide\n");

                for(int i = 0; i < 26; i++)
                {
                    char *pt;
                    decrypt_rotation(cipher_text, &pt, i);
                    printf("Key %d - %s", i, pt);
                    free(pt);
                }
            }
            
        } break;
        
        case 2:
        {
            //break_substitution_menu();
        } break;

        case 3:
        {
            //break_unknown_menu();
        } break;

        default: printf("Invalid choice\n");
    }

    // make sure to free memory
    free(cipher_text);
}

void main_menu()
{
    //help_menu();

    // loop in menu until user wants to exit
    while(1)
    {
        // display options
        help_menu();
        // get user option
        int input = get_int_input();
        printf("\n");

        switch(input)
        {
            case 1: rot_menu(); break;
            case 2: sub_menu(); break;
            case 3: break_menu(); break;
            case 4: help_menu(); break;
            case 5:
            {
                // file input chosen, ask for filepath
                printf("Please input path to file\n");
                char *filename = get_string_input();
                // send to file handling function
                handle_file_input(filename);
                // free input data
                free(filename);
                printf("\n\n");
            } break;

            case 6: printf("Quitting\n"); return;
            
            default: printf("Invalid input. Please choose an option from the menu\n\n");
        }
    }
}

// this function reads in a command file and executes the commands within it
int handle_file_input(char *filename)
{
    // read the file data and store instructions in FileCommand struct
    FileCommand command = read_command_file(filename);

    // if command is returned with the mode value set to Invalid, return error
    if(command.mode == Invalid) {
        printf("Command file contained invalid commands.\n");
        return 1;
    }

    // pretty self explanatory from here
    if(command.mode == Encrypt)
    {
        printf("Encrypt\n");
        if(command.cipher == Rotation)
        {
            printf("Rotation\n");
            char *cipher_text;
            int result = encrypt_rotation(command.message, &cipher_text, command.rot_key);
            if(result != 0)
            {
                printf("Encryption failed\n");
            }

            printf("Plain Text: %s\n", command.message);
            printf("Cipher text: %s\n", cipher_text);

            free(cipher_text);
        }
        else if(command.cipher == Substitution)
        {
            printf("Substitution\n");
            char *cipher_text;
            int result = encrypt_substitution(command.message, &cipher_text, command.sub_key);
            if(result != 0)
            {
                printf("Encryption failed\n");
            }

            printf("Plain Text: %s\n", command.message);
            printf("Cipher text: %s\n", cipher_text);

            free(cipher_text);
        }
    }
    else if(command.mode == Decrypt)
    {
        printf("Decrypt\n");
        if(command.cipher == Rotation)
        {
            printf("Rotation\n");
            char *plain_text;
            int result = decrypt_rotation(command.message, &plain_text, command.rot_key);
            if(result != 0)
            {
                printf("Encryption failed\n");
            }

            printf("Cipher text: %s\n", command.message);
            printf("Decrypted text: %s\n", plain_text);

            free(plain_text);
        }
        else if(command.cipher == Substitution)
        {
            printf("Substitution\n");
            char *plain_text;
            int result = decrypt_substitution(command.message, &plain_text, command.sub_key);
            if(result != 0)
            {
                printf("Encryption failed\n");
            }

            printf("Cipher text: %s\n", command.message);
            printf("Decrypted text: %s\n", plain_text);

            free(plain_text);
        }
    }
    else if(command.mode == Break)
    {
        printf("Break\n");
        if(command.cipher == Rotation)
        {
            // if break rotation, try to break it using function
            printf("Rotation\n");
            char *plain_text;
            int key;
            int result = break_rotation_cipher(command.message, &plain_text, &key);
            printf("Cipher text %s\n\n", command.message);
            printf("Plain text %s\n", plain_text);
            if(result) free(plain_text);

            // ask user if it was correct, if yes, exit
            printf("\n");
            printf("Was this correct?\n");
            printf("1. Yes\n2. No\n");

            int input = get_int_input();

            if(input == 1)
            {
                return 0;
            }
            // if no, print all possibilities
            else
            {
                printf("Printing all possibilities\n");

                for(int i = 0; i < 26; i++)
                {
                    char *pt;
                    decrypt_rotation(command.message, &pt, i);
                    printf("Key %d - %s\n\n", i, pt);
                    free(pt);
                }
            }
            

        }
        else if(command.cipher == Substitution)
        {
            printf("Substitution\n");
            char *plain_text;
            char *key;
            int result = break_substitution_cipher(command.message, &plain_text, &key);
            if(result != 0)
            {
                printf("Sorry, couldn't get it.\n");
            }
            else
            {
                printf("\n\n");
                printf("If one of those was correct i'll be super surprised.\n");
                printf("Only had time to implement basic letter frequency analysis\n");
                //printf("Key: %s - Message: %s\n", key, plain_text);
                //free(plain_text);
            }
        }
        // if unknown cipher, go through both possibilities
        else
        {
            printf("Unknown Cipher\n");
            printf("Testing Rotation Cipher\n");

            char *plain_text;
            int key;
            int result = break_rotation_cipher(command.message, &plain_text, &key);
            if(result == 0)
            {
                printf("Got it!\n");
                printf("Key: %d\n", key);
                printf("Message: %s\n", plain_text);
                free(plain_text);
            }
            else
            {
                printf("Couldn't get it. So here's all of them, you decide\n");

                for(int i = 0; i < 26; i++)
                {
                    char *pt;
                    decrypt_rotation(command.message, &pt, i);
                    printf("Key %d - %s", i, pt);
                    free(pt);
                }
            }

            printf("Testing Substitution Cipher\n");

            char *plain_text2;
            char key2[26];
            int result2 = break_substitution_cipher(command.message, &plain_text2, &key2);
            if(result2 != 0)
            {
                printf("Sorry, couldn't get it.\n");
            }
            else
            {
                printf("Key: %s\nMessage: %s\n", key2, plain_text2);
                free(plain_text2);
            }
        }
    }

    // make sure we clean up stuff
    if(command.message != NULL) free(command.message);

    return 0;
}

// this function reads the given command line arguments and performs actions based on them
// currently only file inputs are supported of the form 'a.exe -f {filename}'
void handle_cmdline(int argc, char *argv[])
{
    printf("Command line arguments given\n");
    
    if(strcmp(argv[1], "-f") == 0)
    {
        printf("Reading file: %s\n", argv[2]);
        int result = handle_file_input(argv[2]);
        if(result != 0)
        {
            printf("Something went wrong reading the input file\n");
        }
    }
    else
    {
        printf("Unknown command line arguments given\n");
        printf("Must be in form: '-f {filename}'\n");
    }
}

int main(int argc, char *argv[])
{
    // make sure that more than 1 arguments were given as first is always executable name
    if(argc > 1)
    {
        handle_cmdline(argc, argv);
        return 0;
    }

    // run main menu
    main_menu();

    return 0;
}
