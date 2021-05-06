#-----------------------------------------------
# Ryanul Haque
# ryhaque
# CSE156/L
# Makefile using Macros
#------------------------------------------------

CC = gcc
TARDIR = ../bin/
CFLAGS	= -g -Wall -Wextra -Wpedantic -Wshadow -lreadline
EXEBIN1 = myserver
EXEBIN2	= myclient
SOURCE1 = $(EXEBIN1).c
SOURCE2 = $(EXEBIN2).c
OBJECT1 = $(EXEBIN1).o
OBJECT2 = $(EXEBIN2).o

all: $(EXEBIN1) $(EXEBIN2); mv $(OBJECT1) $(EXEBIN1) $(OBJECT2) $(EXEBIN2) $(TARDIR)

$(EXEBIN1) : $(OBJECT1)
	$(CC) $(CFLAGS) -o $(EXEBIN1) $(OBJECT1)

$(OBJECT1) : $(SOURCE1)
	$(CC) $(CFLAGS) -c $(SOURCE1)

$(EXEBIN2) : $(OBJECT2)
	$(CC) $(CFLAGS) -o $(EXEBIN2) $(OBJECT2)

$(OBJECT2) : $(SOURCE2)
	$(CC) $(CFLAGS) -c $(SOURCE2)

clean :
	cd $(TARDIR); rm -f $(EXEBIN1) $(OBJECT1) $(OBJECT2) $(EXEBIN2)

