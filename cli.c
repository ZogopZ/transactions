#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cli.h"
#include "tools.h"
#include "graph.h"
#include "cmd_line_utils.h"


void command_line_user(cmd_line_operation_t parser, char *argv)
{
    int command;
    char *str, *tokenspace, *token1, *token2, *token3, *token4;
    struct a *inputA, *inputB;
    struct g *g_node, *g_node_delete;

    str = mallocNcheck(MAX_ARG_STR);

    printf(">>> Entering user command line...\n");
    while (1)
    {
        command = VALID;                            //Flag for correct arguments for commands delete edge and modify edge.
        token1 = token2 = token3 = token4 = NULL;
        printf(">>> ");
        fgets (str, MAX_ARG_STR, stdin);            //Read a line from stdin and store it to str
        tokenspace = strtok(str, " \t\n");          //Get first argument from stdin. Use strtok with space, tab and new line deliminators.

        if (strcmp(str, "\n") == 0)                                         //When users just types enter, cli continues
            continue;
        else if (strcmp(tokenspace, "i") == 0)                              /*Insert node case.*/
        {
            while ((tokenspace = strtok(NULL, " \t\n")) != NULL)            //Use strtok with space, tab and new line deliminators.
            {                                                               //Repeat for more than one usernames.
                inputA = new_a_node(tokenspace, strlen(tokenspace), 0, -1);
                g_node = user_dup_check(inputA);
                insert_node(inputA, g_node);
                free(inputA);
            }
        }
        else if (strcmp(tokenspace, "n") == 0)                              /*Insert edge case.*/
        {
            while (((tokenspace = strtok(NULL, " \t\n")) != NULL))          //Read all arguments from command line.
            {
                if (token1 == NULL)
                    token1 = tokenspace;
                else if (token2 == NULL)
                    token2 = tokenspace;
                else if (token3 == NULL)
                    token3 = tokenspace;
                else
                {
                    command = INVALID;
                    break;
                }
            }
            if ((command == INVALID) || (token1 == NULL) || (token2 == NULL) || (token3 == NULL) || (atoi(token3) == 0))
            {
                printf("\t- none\n");
                continue;
            }
            else if (strcmp(token1, token2) == 0)   //Transaction between the same username.
            {
                inputA = new_a_node(token1, strlen(token1), atoi(token3), 2);
                g_node = user_dup_check(inputA);
                if (inputA->dupe == NOT_FOUND)
                    g_node = insert_node(inputA, g_node);
                edge_insertion(g_node, g_node, inputA, inputA);
                printf("\t- Inserted |%s|->%d->|%s|\n", inputA->username, inputA->weight, inputA->username);
                free(inputA);
            }
            else
            {
                inputA = new_a_node(token1, strlen(token1), atoi(token3), 0);
                inputB = new_a_node(token2, strlen(token2), atoi(token3), 1);
                add_to_graph(inputA, inputB);
                printf("\t- Inserted |%s|->%d->|%s|\n", inputA->username, inputA->weight, inputB->username);
                free(inputA);
                free(inputB);
            }
        }
        else if (strcmp(tokenspace, "d") == 0)                              /*Delete graph node case.*/
        {
            while (((tokenspace = strtok(NULL, " \t\n")) != NULL))
            {
                inputA = new_a_node(tokenspace, strlen(tokenspace), 0, -1);
                g_node_delete = user_dup_check(inputA);
                if (inputA->dupe == FOUND)
                {
                    delete_g_node(g_node_delete);
                    printf("\t- Deleted |%s|\n", inputA->username);
                }
                else
                    printf("\t- Node |%s| does not exist - abort-d;\n", inputA->username);
                free(inputA);
            }
        }
        else if (strcmp(tokenspace, "l") == 0)                              /*Delete edges case.*/
        {
            while (((tokenspace = strtok(NULL, " \t\n")) != NULL))          //Read all arguments from command line.
            {
                if (token1 == NULL)
                    token1 = tokenspace;
                else if (token2 == NULL)
                    token2 = tokenspace;
                else if (token3 == NULL)
                    token3 = tokenspace;
                else
                {
                    command = INVALID;
                    break;
                }
            }
            if ((command == INVALID) || (token1 == NULL) || (token2 == NULL))
            {
                printf("\t- none\n");
                continue;
            }
            else if (token3 == NULL)                                        //No weight defined. Delete all edges between two usernames.
            {
                if (strcmp(token1, token2) == 0)                            //Delete all edges between the same username.
                {
                    inputA = new_a_node(token1, strlen(token1), 0, -1);
                    find_edges_S(inputA);
                    free(inputA);
                }
                else                                                        //Delete all edges between different usernames.
                {
                    inputA = new_a_node(token1, strlen(token1), 0, -1);
                    inputB = new_a_node(token2, strlen(token2), 0, -1);
                    find_edges(inputA, inputB);
                    free(inputA);
                    free(inputB);
                }
            }
            else                                                            //Delete an edge between usernames.
            {
                if (strcmp(token1, token2) == 0)                            //Delete an edge between the same username.
                {
                    inputA = new_a_node(token1, strlen(token1), atoi(token3), -1);
                    find_edge_S(inputA, EDGE_DELETION);
                    free(inputA);
                }
                else                                                        //Delete an edge between different usernames.
                {
                    inputA = new_a_node(token1, strlen(token1), atoi(token3), -1);
                    inputB = new_a_node(token2, strlen(token2), atoi(token3), -1);
                    find_edge(inputA, inputB, EDGE_DELETION);
                    free(inputA);
                    free(inputB);
                }
            }
        }
        else if (strcmp(tokenspace, "m") == 0)                              /*Modify edge case.*/
        {
            while (((tokenspace = strtok(NULL, " \t\n")) != NULL))
            {
                if (token1 == NULL)
                    token1 = tokenspace;
                else if (token2 == NULL)
                    token2 = tokenspace;
                else if (token3 == NULL)
                    token3 = tokenspace;
                else if (token4 == NULL)
                    token4 = tokenspace;
                else
                {
                    command = INVALID;
                    break;
                }
            }
            if (command == INVALID || (token1 == NULL) || (token2 == NULL) || (token3 == NULL) || (token4 == NULL))
            {
                printf(">>> Error. Wrong arguments for modify edge command.\n");
                continue;
            }
            else if (strcmp(token1, token2) == 0)                               //Modify an edge between the same username.
                modify_edge_S(token1, token3, token4);
            else                                                                //Modify an edge between different usernames.
                modify_edge(token1, token2, token3, token4);
        }
        else if (strcmp(tokenspace, "r") == 0)                              /*Print receiving edges case.*/
        {
            while (((tokenspace = strtok(NULL, " \t\n")) != NULL))
            {
                if (token1 == NULL)
                    token1 = tokenspace;
                else
                {
                    command = INVALID;
                    break;
                }
            }
            if (command == INVALID || (token1 == NULL))
            {
                printf(">>> Error. Wrong arguments for receiving edges command.\n");
                continue;
            }
            else
                print_receiving(token1);
        }
        else if (strcmp(tokenspace, "c") == 0)                              /*Find simple circles case.*/
        {
            while (((tokenspace = strtok(NULL, " \t\n")) != NULL))
            {
                if (token1 == NULL)
                    token1 = tokenspace;
                else
                {
                    command = INVALID;
                    break;
                }
            }
            if (command == INVALID || (token1 == NULL))
            {
                printf(">>> Error. Wrong arguments for circlefind command.\n");
                continue;
            }
            else
                simple_circles(token1);
        }
        else if (strcmp(tokenspace, "f") == 0)                              /*Find complex circles case.*/
        {
            while (((tokenspace = strtok(NULL, " \t\n")) != NULL))
            {
                if (token1 == NULL)
                    token1 = tokenspace;
                else if (token2 == NULL)
                    token2 = tokenspace;
                else
                {
                    command = INVALID;
                    break;
                }
            }
            if (command == INVALID || (token1 == NULL) || (token2 == NULL))
            {
                printf(">>> Error. Wrong arguments for findcircles command.\n");
                continue;
            }
            else
               {};
        }
        else if (strcmp(tokenspace, "pall") == 0)
            print_graph_nodes();
        else if (strcmp(tokenspace, "p") == 0)
        {
            if ((tokenspace = strtok(NULL, " \t\n")) != NULL)
            {
                g_node = g_head;
                while ((g_node != NULL) && (strcmp(g_node->username, tokenspace) != 0))
                {
                    g_node = g_node->g_next;
                }
                if (g_node == NULL)
                {
                    printf("\t |%s| not found...\n", tokenspace);
                    continue;
                }
                else
                    print_g_node(g_node);
            }
            else if (g_head == NULL)
                printf(">>> Warning. Graph is empty.\n");
            else
                print_graph();
        }
        else if (strcmp(tokenspace, "e") == 0)
        {
            if (((tokenspace = strtok(NULL, " \t\n")) != NULL))
               printf("\t- none\n");
            else
            {
                if (parser == IO_OK || parser == NO_INPUT)
                    file_write(argv);
                if (g_head == NULL)
                    printf(">>> Graph is empty\n");
                else
                {
                    free_graph(g_head);
                    printf("\t- exit program\n");
                }
                free(str);
                return;
            }
        }
    }
}
