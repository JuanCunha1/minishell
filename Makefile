# **************************************************************************** #
#                                   CONFIG                                     #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP
READLINE = -lreadline -lncurses

MAKEFLAGS += --no-print-directory

SRCS_DIR = src
OBJS_DIR = objs
INCLUDES = -Iinclude -Ilibft

LIBFT = libft/libft.a
LIBS = -Llibft -lft $(READLINE)


# **************************************************************************** #
#                                   SOURCES                                    #
# **************************************************************************** #

MAIN = src/main.c

PARSER_SRCS = \
	src/parser/parser.c \
	src/parser/pipe.c \
	src/parser/redirection.c

LEXER_SRCS = \
	src/lexer/lexer.c \
	src/lexer/tokenizer.c \
	src/lexer/quotes.c \
	src/lexer/states.c \
	src/lexer/expand_var.c

EXECUTE_SRCS = \
	src/exec/executor.c \
	src/exec/get_path.c \
	src/exec/exec_redirection.c \
	src/exec/exec_pipe.c \
	src/exec/exec_heredoc.c \
	src/exec/utils.c

SIGNALS_SRCS = \
	src/signal/signals.c \
	src/signal/heredoc_signal.c

SYNTAX_SRCS = \
	src/syntax/syntax_error.c \
	src/syntax/syntax_main.c

GLOBAL_UTILS = \
	src/global_utils/free_utils.c \
	src/global_utils/free_utils2.c \
	src/global_utils/init.c \
	src/global_utils/env.c \
	src/global_utils/banner.c

BUILTIN_SRCS = \
	src/builtins/ft_pwd.c \
	src/builtins/ft_echo.c \
	src/builtins/ft_cd.c \
	src/builtins/ft_exit.c \
	src/builtins/ft_export.c \
	src/builtins/ft_export_utils.c \
	src/builtins/builtin.c \
	src/builtins/ft_unset.c \
	src/builtins/ft_env.c

SRCS = \
	$(MAIN) \
	$(PARSER_SRCS) \
	$(LEXER_SRCS) \
	$(EXECUTE_SRCS) \
	$(SIGNALS_SRCS) \
	$(SYNTAX_SRCS) \
	$(GLOBAL_UTILS) \
	$(BUILTIN_SRCS)

OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

DEPS = $(OBJS:.o=.d)

# **************************************************************************** #
#                                   COLORS                                     #
# **************************************************************************** #

GREEN  = \033[0;32m
YELLOW = \033[0;33m
RED    = \033[0;31m
RESET  = \033[0m

# **************************************************************************** #
#                                   RULES                                      #
# **************************************************************************** #

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) Makefile
	@echo "$(GREEN) - Building $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)
	@echo "$(YELLOW) - Done!$(RESET)"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C libft

clean:
	@rm -rf $(OBJS_DIR)
	@$(MAKE) -C libft clean
	@echo "$(RED) - Objects removed$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C libft fclean
	@echo "$(RED) - Full clean done$(RESET)"

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
