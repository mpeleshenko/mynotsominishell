# mynotsominishell
#

NAME = mynotsominishell

SRC = terminal.c \
	display.c \
	input.c \
	mynotsominishell.c
OBJ = $(SRC:.c=.o)

LIBPATH = ../lib
LIBS = -lmy -ltermcap

RM = rm -f

all: $(OBJ)
	cc -L$(LIBPATH) $(OBJ) $(LIBS) -o $(NAME)

clean: 
	-$(RM) *.o
	-$(RM) *~
	-$(RM) \#*
	-$(RM) *.swp
	-$(RM) *.core

fclean: clean
	-$(RM) $(NAME)

re: fclean all