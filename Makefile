# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address -fsanitize=leak
READLINE = -lreadline -lncurses

MAKEFLAGS += --no-print-directory
# Program name
NAME = minishell

# Directories
SRCS_DIR = src
OBJS_DIR = objs
INCLUDES_DIR = include
DIRS := $(shell find $(SRCS_DIR) -type d | sed "s/$(SRCS_DIR)/$(OBJS_DIR)/")

# Includes
INCLUDES_FLAG = -I$(INCLUDES_DIR) -Ilibft/

# Libft
LIBFT = libft/libft.a
LIBS = -Llibft -lft $(READLINE)

# ---------------------------------------------------------
#                 SOURCE FILE DETECTION
# ---------------------------------------------------------

# Main only in src/
MAIN = $(SRCS_DIR)/main.c

# Parser sources
PARSER_SRCS = \
		$(SRCS_DIR)/parser/parser.c \
		$(SRCS_DIR)/parser/ast.c \
		$(SRCS_DIR)/parser/pipe.c \
		$(SRCS_DIR)/parser/redirection.c \


# Lexer sources
LEXER_SRCS = \
		$(SRCS_DIR)/lexer/lexer.c \
		$(SRCS_DIR)/lexer/simple_quot.c \
		$(SRCS_DIR)/lexer/list.c \
		$(SRCS_DIR)/lexer/utils.c

# execute sources
EXECUTE_SRCS = \
		$(SRCS_DIR)/exec/executor.c \
		$(SRCS_DIR)/exec/get_path.c \
		$(SRCS_DIR)/exec/exec_redirection.c \
		$(SRCS_DIR)/exec/exec_pipe.c \
		$(SRCS_DIR)/exec/utils.c

SIGNALS_SRCS = \
		$(SRCS_DIR)/signal/signals.c

# Utility functions
GLOBAL_UTILS = \
		$(SRCS_DIR)/global_utils/free_utils.c \
		$(SRCS_DIR)/global_utils/init.c

# Builtin sources
BUILTIN_SRCS = \
		$(SRCS_DIR)/builtins/ft_pwd.c \
		$(SRCS_DIR)/builtins/ft_echo.c \
		$(SRCS_DIR)/builtins/ft_cd.c \
		$(SRCS_DIR)/builtins/ft_exit.c \
		$(SRCS_DIR)/builtins/ft_export.c \
		$(SRCS_DIR)/builtins/ft_export_utils.c \
		$(SRCS_DIR)/builtins/builtin.c \
		$(SRCS_DIR)/builtins/ft_unset.c \
		$(SRCS_DIR)/builtins/ft_env.c

# Combine all sources
SRCS =	$(MAIN) \
		$(PARSER_SRCS) \
		$(LEXER_SRCS) \
		$(BUILTIN_SRCS) \
		$(EXECUTE_SRCS) \
		$(SIGNALS_SRCS) \
		$(GLOBAL_UTILS)

# Object files
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)


# Colors
GREEN  = \033[0;32m
YELLOW = \033[0;33m
RED    = \033[0;31m
RESET  = \033[0m


# ---------------------------------------------------------
#                   COMPILATION RULES
# ---------------------------------------------------------

all: $(OBJS_DIR) $(LIBFT) $(NAME)

$(OBJS_DIR):
	@mkdir -p $(DIRS)

$(NAME): $(OBJS)
	@echo "$(GREEN) - Building $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)
	@echo "$(YELLOW) - Compilation finished!$(RESET)"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@$(CC) $(CFLAGS) $(INCLUDES_FLAG) -c $< -o $@


# ---------------------------------------------------------
#                        LIBFT
# ---------------------------------------------------------
$(LIBFT):
	@$(MAKE) -C libft

# ---------------------------------------------------------
#                      CLEAN RULES
# ---------------------------------------------------------
clean:
	@rm -rf $(OBJS_DIR)
	@$(MAKE) -C libft clean
	@echo "$(RED) - Objects cleaned$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C libft fclean
	@echo "$(RED) - Full clean done$(RESET)"

re: fclean all

nvim: fclean

.PHONY: all clean fclean re nvim
