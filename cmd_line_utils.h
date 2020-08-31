#ifndef _COMMAND_LINE_UTILS_H_
#define _COMMAND_LINE_UTILS_H_


typedef enum flag
{
    NO_IO,
    NO_INPUT,
    NO_OUTPUT,
    ERROR,
    OUTPUT_OK,
    INPUT_OK,
    IO_OK,

} cmd_line_operation_t;

cmd_line_operation_t parse_cmd_line_args(int argc, char** argv);

int get_arg_input_val(void);

int get_arg_output_val(void);

#endif /* _COMMAND_LINE_UTILS_H_ */
