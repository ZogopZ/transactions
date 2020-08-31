#############################################################################
# Makefile for: Operating_Systems: Project 1 			            #
#############################################################################


CC            = gcc
CFLAGS        = -g -Wall -W
DEL_FILE      = rm -f

OBJECTS       = main.o \
		cli.o \
		cmd_line_utils.o\
		doc_utils.o \
		graph.o \
		tools.o

default: mygraph

mygraph: main.o cli.o cmd_line_utils.o doc_utils.o graph.o tools.o
	$(CC) $(CFLAGS) -o mygraph main.o cli.o cmd_line_utils.o doc_utils.o graph.o tools.o

main.o: main.c main.h \
		cli.h \
		graph.h \
		doc_utils.h \
		cmd_line_utils.h 		
	$(CC) -c $(CFLAGS) -o main.o main.c

cli.o: cli.c cli.h \
		tools.h \
		graph.h 
	$(CC) -c $(CFLAGS) -o cli.o cli.c

doc_utils.o: doc_utils.c doc_utils.h \
		tools.h \
		graph.h 
	$(CC) -c $(CFLAGS) -o doc_utils.o doc_utils.c

cmd_line_utils.o: cmd_line_utils.c cmd_line_utils.h
	$(CC) -c $(CFLAGS) -o cmd_line_utils.o cmd_line_utils.c

graph.o: graph.c graph.h \
		tools.h 
	$(CC) -c $(CFLAGS) -o graph.o graph.c

tools.o: tools.c tools.h \
		graph.h 
	$(CC) -c $(CFLAGS) -o tools.o tools.c


clean:
	-$(DEL_FILE)  mygraph
	-$(DEL_FILE)  $(OBJECTS)
