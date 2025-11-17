NAME			= minishell
CC				= cc
CFLAGS			= -Wall -Wextra -Werror
MKDIR			= mkdir -p
RM				= rm -rf
LINKER  	    = -lreadline

# Includes
INCLUDES_DIR 	= include
INCLUDES_FLAG 	= -I$(INCLUDES_DIR)
INCLUDES		= $(wildcard $(INCLUDES_DIR)/*.h)

# Sources
SRCS_DIR		= src/
SRC_FILES		= main.c \
				  lexer.c

# Libraries
LIBFT = libft/libft.a
LIBS = -Llibft -lft

# Objects
OBJS_DIR		= objs/
OBJ_FILES		= $(SRC_FILES:.c=.o)
OBJS			= $(addprefix $(OBJS_DIR), $(OBJ_FILES))


all : $(OBJS_DIR) $(NAME)

$(OBJS_DIR) :
	@$(MKDIR) $(OBJS_DIR)

$(NAME) : $(OBJS) Makefile $(LIBFT)
	@echo $(GREEN) " - Compiling $(NAME)..." $(RESET)
	@$(CC) $(CFLAGS) $(OBJS) $(LINKER) -o $(NAME) $(LIBS)
	@echo $(YELLOW) " - Compiling FINISHED" $(RESET)

$(OBJS_DIR)%.o : $(SRCS_DIR)%.c $(INCLUDES)
	@$(CC) $(CFLAGS) $(INCLUDES_FLAG) -c $< -o $@	

# Library Rules
$(LIBFT):
	@make -C libft

clean :
	@$(RM) $(OBJS_DIR)
	@make clean -C libft
	@echo $(RED) " - Cleaned!" $(RESET)

fclean : clean
	@$(RM) $(NAME) $(LIBFT)
	@echo $(RED) " - Full Cleaned!" $(RESET)

re: fclean all

.PHONY: all clean fclean re libft