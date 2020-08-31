#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "graph.h"
#include "tools.h"
#include "cmd_line_utils.h"


void* mallocNcheck(size_t incoming_size)
{
    void *ptr;

    ptr = malloc(incoming_size + 1);            //+1 for terminating character.
    if (ptr == NULL && incoming_size > 0)
    {
        printf("Not enough memory in heap\n");
        exit(EXIT_FAILURE);
    }
    memset(ptr, 0, incoming_size + 1);          //Every allocated string will have a null terminating character.
    return ptr;                                 //Every pointer is initialized to NULL.
}

void file_write(char *argv)
{
    struct g *g_temp;
    struct t *t_temp;
    FILE *fp;

    g_temp = g_head;


    if((fp = fopen(argv, "w")) == NULL)         //Open a file named by arguments of main for writing.
    {                                           //If it already exists, empty it first, ("w") option.
        printf("cannot open file \n");
        exit(EXIT_FAILURE);
    }

    while (g_temp != NULL)
    {
        t_temp = g_temp->gt_node;
        fprintf(fp, "\t|%s|\n", g_temp->username);
        while (t_temp != NULL)
        {
            if (t_temp->direction == 0)
                fprintf(fp, "\t\t->%d->|%s|\n", t_temp->weight, t_temp->to_user->username);
            else if (t_temp->direction == 1)
                fprintf(fp, "\t\t<-%d<-|%s|\n", t_temp->weight, t_temp->to_user->username);
            else
                fprintf(fp, "\t\t<->%d<->|%s|\n", t_temp->weight, t_temp->to_user->username);
            t_temp = t_temp->t_next;
        }
        g_temp = g_temp->g_next;
    }
    fclose(fp);
}

