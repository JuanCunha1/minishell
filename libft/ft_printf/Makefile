# Compiler and flags
CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror

# Archive and remove
AR			=	ar rcs
RM			=	rm -f

# Project output
NAME		=	libftprintf.a

SRCS		=	ft_printf.c		\
				ft_print.c

OBJS		=	${SRCS:.c=.o}

INCLUDE		= 	ft_printf.h

all: $(NAME)

$(NAME): $(OBJS)
			$(AR) $(NAME) $(OBJS)

%.o: %.c  Makefile $(INCLUDE)
			$(CC) $(CFLAGS) -c $< -o $@
clean:
			$(RM) $(OBJS)

fclean: clean
			$(RM) $(NAME)

re: fclean all
