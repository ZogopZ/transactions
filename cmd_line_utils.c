#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_line_utils.h"

#define LINUX_MACHINE

#ifdef LINUX_MACHINE
    #define FIRST_ARG   0
    #define SECOND_ARG  1
    #define THIRD_ARG   2
    #define FOURTH_ARG  3
    #define FIFTH_ARG   4
#else
    #define FIRST_ARG   1
    #define SECOND_ARG  2
    #define THIRD_ARG   3
    #define FOURTH_ARG  4
#endif


int arg_input_val;
int arg_output_val;

int get_arg_input_val(void)
{
    return (arg_input_val);     //Returns the argc value of input file.
}

int get_arg_output_val(void)
{
    return (arg_output_val);    //Returns the argc value of output file.
}

cmd_line_operation_t parse_cmd_line_args(int argc, char** argv)
{
    // Given arguments >>>./mygraph
    if (argc == 1 && (strcmp(argv[FIRST_ARG], "./mygraph") == 0))
    {
        puts(">>> No input/output file.");
        return NO_IO;
    }
    // Given arguments >>>./mygraph -i inputfile
    else if (argc == 3 && (strcmp(argv[FIRST_ARG], "./mygraph") == 0)
             && (strcmp(argv[SECOND_ARG], "-i") == 0))
    {
        arg_input_val = 3;
        arg_output_val = NO_OUTPUT;
        puts(">>> No output file.");
        return INPUT_OK;
    }
    // Given arguments >>>./mygraph -o outfile
    else if (argc == 3 && (strcmp(argv[FIRST_ARG], "./mygraph") == 0)
             && (strcmp(argv[SECOND_ARG], "-o") == 0))
    {
        arg_input_val = NO_INPUT;
        arg_output_val = 3;
        puts(">>> No input file.");
        return NO_INPUT;
    }
    // Given arguments >>>./mygraph -i inputfile -o outfile
    else if (argc == 5 && (strcmp(argv[FIRST_ARG], "./mygraph") == 0)
             && (strcmp(argv[SECOND_ARG], "-i") == 0) && (strcmp(argv[FOURTH_ARG], "-o") == 0 ))
    {
        arg_input_val = 3;
        arg_output_val = 5;
        return IO_OK;
    }
    // Given arguments >>>./mygraph -o outfile -i inputfile
    else if (argc == 5 && (strcmp(argv[FIRST_ARG], "./mygraph") == 0)
             && (strcmp(argv[SECOND_ARG], "-o") == 0) && (strcmp(argv[FOURTH_ARG], "-i") == 0 ))
    {
        arg_input_val = 5;
        arg_output_val = 3;
        return IO_OK;
    }
    else
    {
        printf("Wrong Arguments\n");
        return ERROR;
    }
}


