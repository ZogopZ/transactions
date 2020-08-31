#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "tools.h"
#include "graph.h"
#include "doc_utils.h"


void read_inputfile(char *argv)
{
    int c, chars, username_count, weight, username1_length, username2_length;
    char *username1, *username2, *weight_str;
    struct a *inputA, *inputB;

    FILE *fp;


    chars = 0;
    username_count = 0;

    if((fp = fopen(argv, "r")) == NULL)
    {
        printf("cannot open file \n");
        exit(EXIT_FAILURE);
    }
    printf(">>> Reading transactions from input file. Please wait...\n");
    while (1)
    {
        c = fgetc(fp);
        if (c == EOF)
            break;
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'
                 || (c >= '0' && c <= '9'))             //Find a username or a transaction.
            chars++;                                    //Count its characters.
        else if (chars == 0)                            //Ignore any consecutive (or not consecutive),
            continue;                                   //non-alphanumeric characters (case of /t/t or /t/n or else).
        else
        {                                               //In case of non-alphanumeric characters.
            fseek(fp, -(chars + 1), SEEK_CUR);          //Set the fp back. +1 for whitespace.
            if (username_count == 0)
            {
                username_count++;
                username1 = mallocNcheck(chars);
                fread(username1, chars, 1, fp);         //Get the first username from input file of each line.
                username1_length = chars;
                chars = 0;
            }
            else if (username_count == 1)
            {
                username_count++;
                username2 = mallocNcheck(chars);
                fread(username2, chars, 1, fp);         //Get the second username from input file of each line.
                username2_length = chars;
                chars = 0;
            }
            else if (username_count == 2)
            {
                weight_str = mallocNcheck(chars);
                fread(weight_str, chars, 1, fp);        //Get the transaction between the above users.
                weight = atoi(weight_str);

                if (strcmp(username1, username2) != 0)  //Create an assisting struct for data saving, from input file.
                {
                    inputA = new_a_node(username1, username1_length, weight, 0);
                    inputB = new_a_node(username2, username2_length, weight, 1);
                }
                else                                    //Transaction to the same username.
                    inputA = inputB = new_a_node(username1, username1_length, weight, 2);
                if (weight == 0)
                   printf(">>> Warning. Zero transaction between users |%s| and |%s| Aborting insertion\n", inputA->username, inputB->username);
                else
                    add_to_graph(inputA, inputB);

                username_count = 0;
                chars = 0;
                free(username1);
                free(username2);
                free(weight_str);
                if (inputA != inputB)
                {
                    free(inputA);
                    free(inputB);
                }
                else
                    free(inputA);
            }
        }
    }
    fclose(fp);
    return;
}
