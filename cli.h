#ifndef _CLI_H_
#define _CLI_H_

#include "cmd_line_utils.h"

#define MAX_ARG_STR 256
#define VALID 1
#define INVALID 0


void command_line_user(cmd_line_operation_t parser, char *argv);

#endif /* _CLI_H_ */
