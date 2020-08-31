#ifndef _GRAPH_H_
#define _GRAPH_H_

#define PLACED 1
#define NOT_PLACED 0
#define FOUND 1
#define NOT_FOUND 0
#define FOUND_ER -1
#define WHITE 0
#define GRAY 1
#define BLACK 2

extern struct g *g_head;

typedef enum
{
    EDGE_DELETION,
    EDGE_DELETIONS,
    EDGE_MOD
} edge_operation_t;

struct s
{
    int weight;
    struct g *s_to_g;
    struct t *s_to_t;
    struct s *s_next;
    char username[0];
};

struct a
{
    int dupe;
    int weight;
    int direction;
    int username_length;
    char username[0];
};

struct t
{
    int state;
    int direction;
    int weight;
    struct t *t_next;
    struct t *t_prev;
    struct g *to_user;
    struct t *to_user_t;
};

struct g
{
    int state;
    int t_to_self;
    struct g *g_next;
    struct g *g_prev;
    struct t *gt_node;
    int username_length;
    char username[0];
};

struct test
{
    int a;
    int b;
    char c[0];
};

struct s *new_s_node(struct g *g_node);

struct a *new_a_node(char *username, int username_length, int weight, int direction);

struct t *new_t_node(struct a *input);

struct g *new_g_node(struct a *input);

struct g *user_dup_check(struct a *input);

struct t *find_last_transaction(struct g *g_node);

void modify_edge_S(char *username, char *weight, char *new_weight);

void modify_edge(char *usernameA, char *usernameB, char *weight, char *new_weight);

void find_edges_S(struct a *input);

void rec_find_S(struct t *t_temp, struct a *input);

void find_edges(struct a *inputA, struct a *inputB);

void rec_find(struct t *t_temp, struct a *input);

void find_edge_S(struct a *input, edge_operation_t op);

void find_edge(struct a *inputA, struct a *inputB, edge_operation_t op);

void delete_edge_S(struct t *t_temp);

void delete_edge_side(struct t *t_tempA);

void delete_t_nodes(struct t *t_node_delete);

void delete_g_node(struct g *g_node_delete);

void edge_insertion(struct g *g_tempA, struct g *g_tempB, struct a *inputA, struct a *inputB);

struct g *insert_node(struct a *input, struct g *g_node);

void add_to_graph(struct a *inputA, struct a *inputB);

void print_receiving(char *username);

void complex_circles(char *username, char *k_weight);

void find_complex_circles(struct t *t_temp, struct g *g_temp, struct s *s_head, struct a *input);

int search_c(struct t *t_temp, struct s *s_head);

void simple_circles(char *username);

void find_circles(struct g *g_temp);

void add_user_to_list(struct s *s_temp_in);

void remove_user_from_list(void);

void print_user_list(void);

void eval_g_nodes(void);

void find_simple_circles(struct t *t_temp);

void restate_gNt_nodes(void);

void add_s_to_list(struct s *s_temp, struct t *t_temp);

int search_s(struct t *t_temp, struct s *s_head);

void insert_s(struct s *s_head, struct s *s_insert);

void weight_input(struct t *t_temp);

void remove_last(struct s *s_head);

void print_assist(struct s *s_head);

void print_g_node(struct g *g_node);

void print_graph_nodes(void);

void print_graph(void);

void free_assist_list(struct s *s_head);

void free_transactions_list(struct t *t_node);

void free_graph(struct g *g_node);

#endif /* _GRAPH_H_ */
