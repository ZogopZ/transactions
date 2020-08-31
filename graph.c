#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "tools.h"
#include "graph.h"


struct g *g_head = NULL;
struct s *s_head = NULL;
int circles = 0;
int c_circles = 0;

struct s *new_s_node(struct g *g_node)
{
    struct s *s_node;
    s_node = mallocNcheck(g_node->username_length + sizeof(struct s));
    memcpy(s_node->username, g_node->username, g_node->username_length);
    return s_node;
}

struct a *new_a_node(char *username, int username_length, int weight, int direction)
{
    struct a *a_node;

    a_node = mallocNcheck(username_length + sizeof(struct a));
    a_node->dupe = NOT_FOUND;
    a_node->weight = weight;
    a_node->direction = direction;
    a_node->username_length = username_length;
    memcpy(a_node->username, username, username_length);
    return a_node;
}

struct g *new_g_node(struct a *input)
{
    struct g *g_node;

    g_node = mallocNcheck(input->username_length + sizeof(struct g));
    g_node->username_length = input->username_length;
    memcpy(g_node->username, input->username, input->username_length);
    g_node->state = WHITE;
    return g_node;
}

struct t *new_t_node(struct a *input)
{
    struct t *t_node;

    t_node = mallocNcheck(sizeof(struct t));
    t_node->weight = input->weight;
    t_node->direction = input->direction;
    t_node->state = WHITE;
    return t_node;
}

struct g *user_dup_check(struct a *input)
{
    struct g *g_node_temp = g_head;
    struct g *g_node_prev = g_head;

    while (g_node_temp != NULL)
    {
        if (strcmp(g_node_temp->username, input->username) == 0)
        {
            input->dupe = FOUND;                //If duplicate username is found, return it.
            return g_node_temp;
        }
        else
        {
            g_node_prev = g_node_temp;
            g_node_temp = g_node_temp->g_next;  //Walk through the entire graph.
        }
    }
    return g_node_prev;                         //If no duplicate was found, return the last graph node for insertion.
}

struct t *find_last_transaction(struct g *g_node)
{
    struct t *t_node;

    t_node = g_node->gt_node;

    if (t_node != NULL)
    {
        while (t_node->t_next != NULL)
            t_node = t_node->t_next;
    }
    return t_node;
}

void modify_edge_S(char *username, char *weight, char *new_weight)
{
    struct a *input;

    input = new_a_node(username, strlen(username), atoi(weight), atoi(new_weight));     //Use field input->direction for new weight.
    find_edge_S(input, EDGE_MOD);
    free(input);
    return;
}

void modify_edge(char *usernameA, char *usernameB, char *weight, char *new_weight)
{
    struct a *inputA, *inputB;

    inputA = new_a_node(usernameA, strlen(usernameA), atoi(weight), atoi(new_weight));   //Use field inputA->direction for new weight.
    inputB = new_a_node(usernameB, strlen(usernameB), atoi(weight), -1);
    find_edge(inputA, inputB, EDGE_MOD);
    free(inputA);
    free(inputB);
    return;
}

void edge_insertion(struct g *g_tempA, struct g *g_tempB, struct a *inputA, struct a *inputB)
{
    struct t *t_nodeB, *t_nodeA;

    if (g_tempA != g_tempB)                                         //UsernameA transacts to usernameB.
    {
        t_nodeA = find_last_transaction(g_tempA);                   //Returns the last transaction of a graph node,
        t_nodeB = find_last_transaction(g_tempB);                   //or NULL if username has not any transactions.

        if ((t_nodeA != NULL) && (t_nodeB != NULL))                 //Both usernames have previous transactions.
        {
            t_nodeA->t_next = new_t_node(inputA);                   //New transaction node creation.
            t_nodeB->t_next = new_t_node(inputB);                   //New transaction node creation.
            t_nodeA->t_next->to_user = g_tempB;                     //Each transaction node, points to the username it transacts to.
            t_nodeA->t_next->to_user_t = t_nodeB->t_next;           //Each transaction node, also points to the transaction node of the above username.
            t_nodeA->t_next->t_prev = t_nodeA;                      //Set correctly the t_prev of the newly created transaction node.
            t_nodeB->t_next->to_user = g_tempA;
            t_nodeB->t_next->to_user_t = t_nodeA->t_next;
            t_nodeB->t_next->t_prev = t_nodeB;
        }
        else if ((t_nodeA == NULL) && (t_nodeB != NULL))            //One username has no previous transactions.
        {
            g_tempA->gt_node = new_t_node(inputA);                  //Create a head transaction node.
            t_nodeB->t_next = new_t_node(inputB);
            g_tempA->gt_node->to_user = g_tempB;
            g_tempA->gt_node->to_user_t = t_nodeB->t_next;
            t_nodeB->t_next->to_user = g_tempA;
            t_nodeB->t_next->to_user_t = g_tempA->gt_node;
            t_nodeB->t_next->t_prev = t_nodeB;
        }
        else if ((t_nodeA != NULL) && (t_nodeB == NULL))            //One username has no previous transactions.
        {
            t_nodeA->t_next = new_t_node(inputA);
            g_tempB->gt_node = new_t_node(inputB);                  //Create a head transaction node.
            t_nodeA->t_next->to_user = g_tempB;
            t_nodeA->t_next->to_user_t = g_tempB->gt_node;
            t_nodeA->t_next->t_prev = t_nodeA;
            g_tempB->gt_node->to_user = g_tempA;
            g_tempB->gt_node->to_user_t = t_nodeA->t_next;
        }
        else if ((t_nodeA == NULL) && (t_nodeB == NULL))            //Both usernames do not have previous transactions.
        {
            g_tempA->gt_node = new_t_node(inputA);                  //Create a head transaction node.
            g_tempB->gt_node = new_t_node(inputB);                  //Create a head transaction node.
            g_tempA->gt_node->to_user = g_tempB;
            g_tempA->gt_node->to_user_t = g_tempB->gt_node;
            g_tempB->gt_node->to_user = g_tempA;
            g_tempB->gt_node->to_user_t = g_tempA->gt_node;
        }
    }
    else if (g_tempA == g_tempB)                                    //Username transacts to itself.
    {
        t_nodeA = find_last_transaction(g_tempA);                   //Returns the last transaction of a graph node,
                                                                    //or NULL if username has not any transactions.
        if (t_nodeA != NULL)
        {
            t_nodeA->t_next = new_t_node(inputA);
            t_nodeA->t_next->to_user = g_tempA;
            t_nodeA->t_next->to_user_t = t_nodeA->t_next;
            t_nodeA->t_next->t_prev = t_nodeA;
        }
        else if (t_nodeA == NULL)
        {
            g_tempA->gt_node = new_t_node(inputA);
            g_tempA->gt_node->to_user = g_tempA;
            g_tempA->gt_node->to_user_t = g_tempA->gt_node;
        }
    }
    return;
}

void find_edges_S(struct a *input)
{
    struct g *g_temp;
    struct t *t_temp;

    g_temp = user_dup_check(input);
    if (input->dupe == NOT_FOUND)
        printf("\t- |%s| does not exist - abort-l;\n", input->username);
    t_temp = g_temp->gt_node;
    if (t_temp != NULL)
    {
        rec_find_S(t_temp, input);
    }
    return;
}

void rec_find_S(struct t *t_temp, struct a *input)
{
    if (t_temp->t_next != NULL)
        rec_find_S(t_temp->t_next, input);
    if (strcmp(t_temp->to_user->username, input->username) == 0)
    {
        delete_edge_side(t_temp);
        printf("\t- Del-vertex |%s|->%d->|%s|\n", t_temp->to_user->username, t_temp->weight, t_temp->to_user->username);
        free(t_temp);
    }
    return;
}

void find_edges(struct a *inputA, struct a *inputB)
{
    struct g *g_tempA;

    g_tempA = user_dup_check(inputA);
    if (inputA->dupe == NOT_FOUND)
        printf("\t- |%s| does not exist - abort-l;\n", inputA->username);
    user_dup_check(inputB);
    if (inputB->dupe == NOT_FOUND)
        printf("\t- |%s| does not exist - abort-l;\n", inputB->username);
    if ((inputA->dupe == FOUND) && (inputB->dupe == FOUND) && (g_tempA->gt_node != NULL))
    {
        rec_find(g_tempA->gt_node, inputB);
    }
}

void rec_find(struct t *t_temp, struct a *input)
{
    if (t_temp->t_next != NULL)
        rec_find(t_temp->t_next, input);
    if (strcmp(t_temp->to_user->username, input->username) == 0)
    {
        delete_edge_side(t_temp);
        delete_edge_side(t_temp->to_user_t);
        printf("\t- Del-vertex |%s|->%d->|%s|\n", t_temp->to_user_t->to_user->username, t_temp->weight, t_temp->to_user->username);
        free(t_temp->to_user_t);
        free(t_temp);
    }
    return;
}

void find_edge_S(struct a *input, edge_operation_t op)
{
    struct g *g_temp;
    struct t *t_temp;

    g_temp = user_dup_check(input);
    if (input->dupe == NOT_FOUND)
    {
        if (op == EDGE_DELETION)
            printf("\t- |%s| does not exist - abort-l;\n", input->username);
        else
            printf("\t- |%s| does not exist - abort-m;\n", input->username);
    }
    else
    {
        t_temp = g_temp->gt_node;
        while (t_temp != NULL)
        {                                                                               //Delete an edge!
            if ((t_temp->weight == input->weight) && (t_temp->to_user == g_temp) && (op == EDGE_DELETION))
            {
                delete_edge_side(t_temp);
                printf("\t- Del-vertex |%s|->%d->|%s|\n", g_temp->username, t_temp->weight, g_temp->username);
                return;
            }                                                                            //Modify an edge!
            else if ((t_temp->weight == input->weight) && (t_temp->to_user == g_temp) && (op == EDGE_MOD))
            {
                t_temp->weight = input->direction;
                printf("\t- Mod-vertex |%s|->%d->|%s|\n", g_temp->username, t_temp->weight, g_temp->username);
                return;
            }
            t_temp = t_temp->t_next;
        }
        if (op == EDGE_DELETION)
            printf("\t-|%s|->%d->|%s| does not exist - abort-l;\n", input->username, input->weight, input->username);
        else
            printf("\t-|%s|->%d->|%s| does not exist - abort-m;\n", input->username, input->weight, input->username);
    }
    return;
}

void find_edge(struct a *inputA, struct a *inputB, edge_operation_t op)
{
    struct g *g_tempA, *g_tempB;
    struct t *t_temp;

    g_tempA = user_dup_check(inputA);
    if (inputA->dupe == NOT_FOUND)
    {
        if (op == EDGE_DELETION)
            printf("\t- |%s| does not exist - abort-l;\n", inputA->username);
        else
            printf("\t- |%s| does not exist - abort-m;\n", inputA->username);
    }
    g_tempB = user_dup_check(inputB);
    if (inputB->dupe == NOT_FOUND)
    {
        if (op == EDGE_DELETION)
            printf("\t- |%s| does not exist - abort-l;\n", inputB->username);
        else
            printf("\t- |%s| does not exist - abort-m;\n", inputB->username);
    }
    if ((inputA->dupe == FOUND) && (inputB->dupe == FOUND))
    {
        t_temp = g_tempA->gt_node;
        while (t_temp != NULL)                                                      //Search the transactions list of a graph node.
        {                                                                           //Delete one edge!
            if ((t_temp->weight == inputA->weight) && (t_temp->to_user == g_tempB) && (op == EDGE_DELETION))
            {                                                                       //Found a transaction with correct weight and username.
                delete_edge_side(t_temp);
                delete_edge_side(t_temp->to_user_t);
                printf("\t- Del-vertex |%s|->%d->|%s|\n", g_tempA->username, t_temp->weight, g_tempB->username);
                free(t_temp->to_user_t);
                free(t_temp);
                return;
            }                                                                       //Modify an edge!
            else if ((t_temp->weight == inputA->weight) && (t_temp->to_user == g_tempB) && (op == EDGE_MOD))
            {
                t_temp->weight = inputA->direction;
                t_temp->to_user_t->weight = inputA->direction;
                printf("\t- Mod-vertex |%s|->%d->|%s|\n", g_tempA->username, t_temp->weight, t_temp->to_user->username);
                return;
            }
            t_temp = t_temp->t_next;                                                //Step to walk through transactions list.
        }
        if (op == EDGE_DELETION)                                                    //Only for edge deletion.
            printf("\t-|%s|->%d->|%s| does not exist - abort-l;\n", inputA->username, inputA->weight, inputB->username);
        else                                                                        //Only for edge modification.
            printf("\t-|%s|->%d->|%s| does not exist - abort-m;\n", inputA->username, inputA->weight, inputB->username);
    }
    return;
}

void delete_edge_side(struct t *t_temp)
{
    if ((t_temp->t_prev != NULL) && (t_temp->t_next != NULL))
    {
        t_temp->t_prev->t_next = t_temp->t_next;
        t_temp->t_next->t_prev = t_temp->t_prev;
    }
    else if ((t_temp->t_prev != NULL) && (t_temp->t_next == NULL))
        t_temp->t_prev->t_next = NULL;
    else if ((t_temp->t_prev == NULL) && (t_temp->t_next != NULL))
    {
        t_temp->to_user_t->to_user->gt_node = t_temp->t_next;
        t_temp->t_next->t_prev = NULL;
    }
    else if ((t_temp->t_prev == NULL) && (t_temp->t_next == NULL))
        t_temp->to_user_t->to_user->gt_node = NULL;
    return;
}

void delete_t_nodes(struct t *t_node_delete)
{
    struct t *transactionB;
    struct g *g_nodeB;

    if (t_node_delete->t_next != NULL)
        delete_t_nodes(t_node_delete->t_next);
    transactionB = t_node_delete->to_user_t;
    g_nodeB = t_node_delete->to_user;
    if ((transactionB->t_prev != NULL) && (transactionB->t_next != NULL))
    {
        transactionB->t_prev->t_next = transactionB->t_next;
        transactionB->t_next->t_prev = transactionB->t_prev;
    }
    else if ((transactionB->t_prev != NULL) && (transactionB->t_next == NULL))
        transactionB->t_prev->t_next = NULL;
    else if ((transactionB->t_prev == NULL) && (transactionB->t_next != NULL))
        g_nodeB->gt_node = transactionB->t_next;
    else if ((transactionB->t_prev == NULL) && (transactionB->t_next == NULL))
        g_nodeB->gt_node = NULL;
    if (t_node_delete == transactionB)
        free(t_node_delete);
    else
    {
        free(transactionB);
        free(t_node_delete);
    }
    return;
}

void delete_g_node(struct g *g_node_delete)
{
    struct t *t_node_delete = g_node_delete->gt_node;

    if (t_node_delete != NULL)
        delete_t_nodes(t_node_delete);
    if ((g_node_delete->g_next != NULL) && (g_node_delete != g_head))
    {

        g_node_delete->g_prev->g_next = g_node_delete->g_next;
        g_node_delete->g_next->g_prev = g_node_delete->g_prev;
    }
    else if ((g_node_delete->g_next != NULL) && (g_node_delete == g_head))
    {
        g_node_delete->g_next->g_prev = NULL;
        g_head = g_node_delete->g_next;
    }
    else if ((g_node_delete->g_next == NULL) && (g_node_delete != g_head))
        g_node_delete->g_prev->g_next = NULL;
    else
        g_head = NULL;
    free(g_node_delete);
    return;
}

//void edge_insertion(struct g *g_tempA, struct g *g_tempB, struct a *inputA, struct a *inputB)
//{
//    struct t *t_nodeB, *t_nodeA;

//    if ((inputA->dupe == NOT_FOUND) && (inputB->dupe == NOT_FOUND) && (g_head->g_next == NULL))    //First transaction from input file is to
//    {   //Username was not duplicate.                                                              //the same username.
//        g_tempA->gt_node = new_t_node(inputA);
//        g_tempA->gt_node->to_user = g_tempA;
//        g_tempA->gt_node->to_user_t = g_tempA->gt_node;
//    }
//    else if ((inputA->dupe == NOT_FOUND) && (inputB->dupe == NOT_FOUND))
//    {   //Both usernames were not duplicates.                                   //First transaction from input file is between
//        if ((g_tempA != g_head) && (g_tempA != g_tempB))                        //two different usernames.
//        {
//            g_tempA = g_tempA->g_next;  //user_dup_check function returned the previous graph node, so point to the new one.
//            g_tempB = g_tempB->g_next;
//        }
//        else if ((g_tempA == g_head) && (g_tempA != g_tempB) && g_head->g_next->g_next) /*FIXME*/
//        {
//            g_tempA = g_head->g_next;
//            g_tempB = g_tempA->g_next;
//        }
//        if (g_tempA == g_tempB)         //Transaction between the same username.
//        {
//            g_tempA->gt_node = new_t_node(inputA);
//            g_tempA->gt_node->to_user = g_tempA;
//            g_tempA->gt_node->to_user_t = g_tempA->gt_node;
//        }
//        else
//        {
//            g_tempA->gt_node = new_t_node(inputA);
//            g_tempB->gt_node = new_t_node(inputB);
//            g_tempA->gt_node->to_user = g_tempB;
//            g_tempA->gt_node->to_user_t = g_tempB->gt_node;
//            g_tempB->gt_node->to_user = g_tempA;
//            g_tempB->gt_node->to_user_t = g_tempA->gt_node;
//        }
//    }                                                   //All other members not initialized here, are 'memseted' to NULL.
//    else if ((inputA->dupe == NOT_FOUND) && (inputB->dupe == FOUND))
//    {   //One username was duplicate.
//        g_tempA = g_tempA->g_next;  //user_dup_check function returned the previous graph node, so point to the new one.
//        g_tempA->gt_node = new_t_node(inputA);
//        t_nodeB = find_last_transaction(g_tempB);       //Find the last transaction of a graph node.
//        if (t_nodeB == NULL)                            //Graph node created by user may not have a transaction.
//        {                                               //Thus create a transactions head and initialize pointers.
//            g_tempB->gt_node = new_t_node(inputB);
//            g_tempA->gt_node->to_user = g_tempB;
//            g_tempA->gt_node->to_user_t = g_tempB->gt_node;
//            g_tempB->gt_node->to_user = g_tempA;
//            g_tempB->gt_node->to_user_t = g_tempA->gt_node;
//        }
//        else
//        {
//            t_nodeB->t_next = new_t_node(inputB);
//            g_tempA->gt_node->to_user = g_tempB;
//            g_tempA->gt_node->to_user_t = t_nodeB->t_next;
//            t_nodeB->t_next->to_user = g_tempA;
//            t_nodeB->t_next->to_user_t = g_tempA->gt_node;
//            t_nodeB->t_next->t_prev = t_nodeB;
//        }
//    }                                                   //All other members not initialized here, are 'memseted' to NULL.
//    else if ((inputA->dupe == FOUND) && (inputB->dupe == NOT_FOUND))
//    {   //One username was duplicate.
//        g_tempB = g_tempB->g_next;  //user_dup_check function returned the previous graph node, so point to the new one.
//        g_tempB->gt_node = new_t_node(inputB);
//        t_nodeA = find_last_transaction(g_tempA);       //Find the last transaction of a graph node.
//        if (t_nodeA == NULL)                            //Graph node created by user may not have a transaction.
//        {                                               //Thus create a transactions head and initialize pointers.
//            g_tempA->gt_node = new_t_node(inputA);
//            g_tempA->gt_node->to_user = g_tempB;
//            g_tempA->gt_node->to_user_t = g_tempB->gt_node;
//            g_tempB->gt_node->to_user = g_tempA;
//            g_tempB->gt_node->to_user_t = g_tempA->gt_node;
//        }
//        else
//        {
//            t_nodeA->t_next = new_t_node(inputA);
//            t_nodeA->t_next->to_user = g_tempB;
//            t_nodeA->t_next->to_user_t = g_tempB->gt_node;
//            t_nodeA->t_next->t_prev = t_nodeA;
//            g_tempB->gt_node->to_user = g_tempA;
//            g_tempB->gt_node->to_user_t = t_nodeA->t_next;
//        }
//    }                                                   //All other members not initialized here, are 'memseted' to NULL.
//    else
//    {   //Both usernames were duplicates.
//        t_nodeA = find_last_transaction(g_tempA);
//        t_nodeB = find_last_transaction(g_tempB);
//        if ((t_nodeA == t_nodeB) && (t_nodeA != NULL))      //Case of edge insertion from an existing
//        {                                                   //username to itself, with previous transactions existing.
//            t_nodeA->t_next = new_t_node(inputA);
//            t_nodeA->t_next->to_user = g_tempA;
//            t_nodeA->t_next->to_user_t = t_nodeA->t_next;
//            t_nodeA->t_next->t_prev = t_nodeA;
//        }
//        else if ((t_nodeA == NULL) && (t_nodeB == NULL))    //Case of edge insertion between two existing
//        {                                                   //usernames, with no previous transactions.
//            g_tempA->gt_node = new_t_node(inputA);
//            g_tempB->gt_node = new_t_node(inputB);
//            g_tempA->gt_node->to_user = g_tempB;
//            g_tempA->gt_node->to_user_t = g_tempB->gt_node;
//            g_tempB->gt_node->to_user = g_tempA;
//            g_tempB->gt_node->to_user_t = g_tempA->gt_node;
//        }
//        else if ((t_nodeA == NULL) && (t_nodeB != NULL))    //Case of edge insertion between two existing
//        {                                                   //usernames. Only one has previous transactions.
//            g_tempA->gt_node = new_t_node(inputA);
//            t_nodeB->t_next = new_t_node(inputB);
//            g_tempA->gt_node->to_user = g_tempB;
//            g_tempA->gt_node->to_user_t = t_nodeB->t_next;
//            t_nodeB->t_next->to_user = g_tempA;
//            t_nodeB->t_next->to_user_t = g_tempA->gt_node;
//            t_nodeB->t_next->t_prev = t_nodeB;
//        }
//        else if ((t_nodeA != NULL) && (t_nodeB == NULL))    //Case of edge insertion between two existing
//        {                                                   //usernames. Only one has previous transactions.
//            t_nodeA->t_next = new_t_node(inputA);
//            g_tempB->gt_node = new_t_node(inputB);
//            t_nodeA->t_next->to_user = g_tempB;
//            t_nodeA->t_next->to_user_t = g_tempB->gt_node;
//            t_nodeA->t_next->t_prev = t_nodeA;
//            g_tempB->gt_node->to_user = g_tempA;
//            g_tempB->gt_node->to_user_t = t_nodeA->t_next;
//        }
//        else                                                //Case of edge insertion between two existing
//        {                                                   //usernames. Both have previous transactions.
//            t_nodeA->t_next = new_t_node(inputA);
//            t_nodeB->t_next = new_t_node(inputB);
//            t_nodeA->t_next->to_user = g_tempB;
//            t_nodeA->t_next->to_user_t = t_nodeB->t_next;
//            t_nodeA->t_next->t_prev = t_nodeA;
//            t_nodeB->t_next->to_user = g_tempA;
//            t_nodeB->t_next->to_user_t = t_nodeA->t_next;
//            t_nodeB->t_next->t_prev = t_nodeB;
//        }                                                 //All other members not initialized here, are 'memseted' to NULL.
//    }
//    return;
//}

struct g *insert_node(struct a *input, struct g *g_node)
{

    if (input->dupe == NOT_FOUND)
    {
        if (g_head == NULL)
        {
            g_head = new_g_node(input);
            printf("\t- Inserted |%s|\n", input->username);
            return g_head;
        }
        else
        {
            g_node->g_next = new_g_node(input);
            g_node->g_next->g_prev = g_node;
            printf("\t- Inserted |%s|\n", input->username);
            return g_node->g_next;
        }

    }
    else
        printf("\t- Node |%s| Exists;\n", input->username);
    return g_node;
}

void add_to_graph(struct a *inputA, struct a *inputB)
{
    struct g *g_tempA, *g_tempB;

    if ((inputA != inputB) && (g_head != NULL))         //Graph head is not empty and usernameA transacts with usernameB.
    {
        g_tempA = user_dup_check(inputA);               //Check for duplicate username.
        if (inputA->dupe == NOT_FOUND)                  //If dupe is NOT_FOUND, user_dupe_check() function,
        {                                               //has returned the previous graph node.
            g_tempA->g_next = new_g_node(inputA);       //->g_next will be the new graph node.
            g_tempA->g_next->g_prev = g_tempA;          //Set correctly the g_prev of the newly created graph node.
            g_tempA = g_tempA->g_next;                  //Point to the current graph node for correct usage
        }                                               //with edge_insertion() function.
        g_tempB = user_dup_check(inputB);
        if (inputB->dupe == NOT_FOUND)
        {
            g_tempB->g_next = new_g_node(inputB);
            g_tempB->g_next->g_prev = g_tempB;
            g_tempB = g_tempB->g_next;
        }                           //If dupe is FOUND, user_dupe_check() function returns existing graph node for edge insertion().
    }
    else if ((inputA == inputB) && (g_head != NULL))    //Graph head is not empty and username transacts to itself.
    {
        g_tempA = user_dup_check(inputA);
        if (inputA->dupe == NOT_FOUND)
        {
            g_tempA->g_next = new_g_node(inputA);
            g_tempA->g_next->g_prev = g_tempA;
            g_tempA = g_tempA->g_next;
        }
        g_tempB = g_tempA;
    }
    else if ((inputA != inputB) && (g_head == NULL))    //Graph head is empty and usernameA transacts with usernameB.
    {
        g_tempA = g_head = new_g_node(inputA);
        g_tempB = g_head->g_next = new_g_node(inputB);
        g_head->g_next->g_prev = g_head;
    }
    else if ((inputA == inputB) && (g_head == NULL))    //Graph head is empty and username transacts to itself.
    {
        g_tempA = g_head = new_g_node(inputA);
        g_tempB = g_tempA;
    }
    edge_insertion(g_tempA, g_tempB, inputA, inputB);
    return;
}

//void add_pair_to_graph(struct a *inputA, struct a *inputB)
//{
//    struct g *g_tempA, *g_tempB;

//    if (g_head == NULL)                         //If graph is empty. Create the graph head and head->next.
//    {
//        if (inputA != inputB)
//        {
//            g_tempA = g_head = new_g_node(inputA);
//            g_tempB = g_head->g_next = new_g_node(inputB);
//            g_head->g_next->g_prev = g_head;
//        }
//        else                                    //Transaction to the same username.
//        {
//            g_tempA = g_head = new_g_node(inputA);
//            g_tempB = g_tempA;
//        }
//    }
//    else
//    {                                               //If graph is not empty.
//        if (inputA != inputB)
//        {
//            g_tempA = user_dup_check(inputA);       //Check for duplicate username of inputA.
//            if (inputA->dupe == NOT_FOUND)          //If username is not a duplicate,
//            {                                       //Create a new graph node.
//                g_tempA->g_next = new_g_node(inputA);
//                g_tempA->g_next->g_prev = g_tempA;  //Initialize new graph's node member g_prev.
//            }
//            g_tempB = user_dup_check(inputB);       //Check for duplicate username of inputB.
//            if (inputB->dupe == NOT_FOUND)          //If username is not a duplicate,
//            {                                       //Create a new graph node.
//                g_tempB->g_next = new_g_node(inputB);
//                g_tempB->g_next->g_prev = g_tempB;  //Initialize new graph's node member g_prev.
//            }
//        }
//        else                                        //Transaction to the same username.
//        {
//            g_tempA = user_dup_check(inputA);
//            if (inputA->dupe == NOT_FOUND)
//            {
//                g_tempA->g_next = new_g_node(inputA);
//                g_tempA->g_next->g_prev = g_tempA;
//            }
//            g_tempB = g_tempA = g_tempA->g_next;
//        }
//    }
//    edge_insertion(g_tempA, g_tempB, inputA, inputB);
//    return;
//}

void print_receiving(char *username)
{
    int i;
    struct g *g_temp;
    struct t *t_temp;
    struct a *input;

    i = 0;

    input = new_a_node(username, strlen(username), 0, -1);
    g_temp = user_dup_check(input);
    if (input->dupe == NOT_FOUND)
        printf("\t- |%s| does not exist - abort-r;\n", input->username);
    else
    {
        t_temp = g_temp->gt_node;
        while (t_temp != NULL)
        {
            if ((t_temp->direction == 1) || (t_temp->direction == 2))   //Transactions from other usernames, or transactions to self.
            {
                i++;
                if (i == 1)
                    printf("\t- Rec-egdes |%s|->%d->|%s|\n", t_temp->to_user->username, t_temp->weight, g_temp->username);
                else
                    printf("\t            |%s|->%d->|%s|\n", t_temp->to_user->username, t_temp->weight, g_temp->username);
            }
            t_temp = t_temp->t_next;
        }
    }
    if (i == 0)
        printf("\t- No-rec-edges |%s|\n", g_temp->username);
    free(input);
    return;
}

void simple_circles(char *username)
{
    struct g *g_temp;
    struct t *t_temp;
    struct a *input;

    input = new_a_node(username, strlen(username), 0, -1);
    g_temp = user_dup_check(input);
    if (input->dupe == NOT_FOUND)       //Check for existing username.
        printf("\t- |%s| does not exist - abort-c;\n", input->username);
    else
    {
        t_temp = g_temp->gt_node;       //First transaction node to begin searching for circles.
        s_head = new_s_node(g_temp);    //The head->username of the assist list, is the (c)irclefinds command, argument.
        s_head->s_to_g = g_temp;        /*FIXME*/ //Maybe not USED
        circles = 0;
        restate_gNt_nodes();
        find_simple_circles(t_temp);
        printf("circles found = %d\n", circles);
        if (circles == 0)
            printf("\t- No-circle-found |%s|\n", g_temp->username);
    }
    free(input);
}

void find_simple_circles(struct t *t_temp)
{
    int circles_t;
    struct s *s_temp;

    circles_t = 0;

    while (t_temp != NULL)          //Check all transaction nodes of current g_node.
    {
                                    //If a transaction node has direction 0 or 2 and it is not yet visited "t_temp->to_user->state == WHITE".
        if ((t_temp->direction == 0 || t_temp->direction == 2) && (t_temp->to_user->state == WHITE))
        {
            weight_input(t_temp);   //Insert its weight to the stack.
            if (strcmp(s_head->username, t_temp->to_user->username) == 0)
            {                       //If a transaction node, transacts with the username (graph node) I am looking for, print whole stack.
                circles_t++;        
                circles++;         
                print_assist(s_head);
            }
            else                                                //If it transacts with someone else,
            {   
                t_temp->to_user->state = GRAY;                  //that someone else becomes proccessed and cannot be visited for the time being "GRAY".
                s_temp = new_s_node(t_temp->to_user);           //Create a new stack member with the above username.
                add_s_to_list(s_temp, t_temp);                  //Add it to the stack list.
                find_simple_circles(t_temp->to_user->gt_node);  //Recursively call find_simple_circles(), with the transactions list of that username.
                remove_last(s_head);                            //When recurse returns, remove the last member of the stack list.
                t_temp->to_user->state = WHITE;                 //And set the username to "WHITE"(can be visited again).
            }
        }
        if((circles_t == 0) && (t_temp->t_next == NULL))        //If no circles where found while proccessing a username
            t_temp->to_user_t->to_user->state = BLACK;          //Set it to "BLACK"(cannot be visited).
        t_temp = t_temp->t_next;                                //Step through all transactions of a username.
    }
    return;
}

void restate_gNt_nodes(void)
{
    struct g *g_temp;

    g_temp = g_head;

    while(g_temp != NULL)
    {
        if(g_temp->state != WHITE)
            g_temp->state = WHITE;
        g_temp = g_temp->g_next;
    }
    return;
}

void add_s_to_list(struct s *s_to_insert, struct t *t_temp)
{
    struct s *s_temp;

    s_temp = s_head;
    if (s_head == NULL)
    {
        s_head = s_to_insert;
        s_head->s_to_g = t_temp->to_user;
        s_head->s_to_t = t_temp;
    }
    else
    {
        while (s_temp->s_next != NULL)
            s_temp = s_temp->s_next;
        s_to_insert->s_to_g = t_temp->to_user;
        s_to_insert->s_to_t = t_temp;
        s_temp->s_next = s_to_insert;
    }
}

void weight_input(struct t *t_temp)
{
    struct s *s_temp;

    s_temp = s_head;

    while (s_temp->s_next != NULL)      //While not the last assist node.
    {
        s_temp = s_temp->s_next;        //Walk through all assist nodes.
    }
    s_temp->weight = t_temp->weight;    //Insert weight of transaction to the last assist node.
    return;
}

void remove_last(struct s *s_head)
{
    struct s *s_temp, *s_prev;

    s_temp = s_head;
    s_prev = s_head;

    while (s_temp->s_next != NULL)      //While not the last assist node.
    {
        s_prev = s_temp;                //Store previous assist node.
        s_temp = s_temp->s_next;        //Walk through all assist nodes.
    }
    if (s_temp != s_head)
    {
        free(s_temp);                   //Free the last assist node.
        s_prev->s_next = NULL;          //Point previous next to NULL.
    }
    else
    {
        free(s_temp);
    }
}

void print_assist(struct s *s_head)
{
    struct s *s_temp;

    s_temp = s_head;

    printf("\t- Cir-found ");
    while (s_temp != NULL)
    {
        //        if (s_temp->s_to_g->eval == VISITED)
        //        {
        //            printf("\n\n%s\n\n", s_temp->s_to_g->username);
        //            assert(0);
        //        }
        printf("|%s|->%d->", s_temp->username, s_temp->weight);
        s_temp = s_temp->s_next;
    }
    printf("|%s|\n", s_head->username);
}


void print_g_node(struct g *g_node)
{
    struct t *t_temp;

    t_temp = g_node->gt_node;
    printf("\t|%s| with %d transactions to self\n", g_node->username, g_node->t_to_self);
    while (t_temp != NULL)
    {
        if (t_temp->direction == 0)
            printf("\t\t->%d->|%s|\n", t_temp->weight, t_temp->to_user->username);
        else if (t_temp->direction == 1)
            printf("\t\t<-%d<-|%s|\n", t_temp->weight, t_temp->to_user->username);
        else
            printf("\t\t<->%d<->|%s|\n", t_temp->weight, t_temp->to_user->username);
        t_temp = t_temp->t_next;
    }
    return;
}

void print_graph_nodes(void)
{
    struct g *g_temp;

    g_temp = g_head;

    while (g_temp != NULL)
    {
        printf("\t|%s|\n", g_temp->username);
        g_temp = g_temp->g_next;
    }
    return;
}

void print_graph(void)
{
    struct g *g_temp;
    struct t *t_temp;

    g_temp = g_head;

    while (g_temp != NULL)
    {
        t_temp = g_temp->gt_node;
        printf("\t|%s|\n", g_temp->username);
        while (t_temp != NULL)
        {
            if (t_temp->direction == 0)
                printf("\t\t->%d->|%s|\n", t_temp->weight, t_temp->to_user->username);
            else if (t_temp->direction == 1)
                printf("\t\t<-%d<-|%s|\n", t_temp->weight, t_temp->to_user->username);
            else
                printf("\t\t<->%d<->|%s|\n", t_temp->weight, t_temp->to_user->username);
            t_temp = t_temp->t_next;
        }
        g_temp = g_temp->g_next;
    }
    return;
}

void free_assist_list(struct s *s_head)
{
    struct s *s_node;

    s_node = s_head;

    if (s_node->s_next != NULL)                 //Recursively free the assist list.
        free_assist_list(s_node->s_next);
    free(s_node);
    return;

}

void free_transactions_list(struct t *t_node)
{                                               //Recursively free the transactions list of a graph node.
    if (t_node->t_next != NULL)
        free_transactions_list(t_node->t_next);
    free(t_node);
    return;
}

void free_graph(struct g *g_node)
{                                               //Recursively free the list of graph nodes.

    if (g_node->g_next != NULL)
        free_graph(g_node->g_next);
    if (g_node->gt_node != NULL)                //If graphs node, has a transactions list.
        free_transactions_list(g_node->gt_node);//Free transactions list first.
    free(g_node);
    return;
}
