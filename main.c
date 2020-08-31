#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "main.h"
#include "graph.h"
#include "doc_utils.h"
#include "cmd_line_utils.h"


int main(int argc, char **argv)
{
    cmd_line_operation_t parser;

    if ((parser = parse_cmd_line_args(argc, argv)) == ERROR)    //Check in line parameters.
        exit(EXIT_FAILURE);
    else if (parser == IO_OK || parser == INPUT_OK)             //In case of present -i flag, parse the inputfile.
        read_inputfile(argv[get_arg_input_val()-1]);
    if (parser == IO_OK || parser == NO_INPUT)
        command_line_user(parser, argv[get_arg_output_val()-1]);  //User enters command line.
}

