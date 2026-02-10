#include "minishell.h"

t_shell	init_shell(char **envp)
{
	t_shell	sh;

	sh.exit_status = 0;
	sh.in_pipeline = 0;
	sh.tokens = NULL;
	sh.envp = envp_dup(envp);
	return (sh);
}

t_pipe	init_pipe(t_ast *node, t_shell *sh)
{
	t_pipe	pip;

	pip.node = node;
	pip.sh = sh;
	pip.input_fd = STDIN_FILENO;
	pip.count = 0;
	pip.capacity = INITIAL_PID_CAPACITY;
	pip.pipe_fd[0] = -1;
	pip.pipe_fd[1] = -1;
	pip.pids = ft_calloc(1, sizeof(pid_t) * pip.capacity);
	if (!pip.pids)
		fatal("malloc", 1);
	return (pip);
}

t_ast	*init_ast(void)
{
	t_ast	*ast;

	ast = ft_calloc(1, sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->type = T_STRING;
	ast->left = NULL;
    ast->right = NULL;
    ast->redirs = NULL;
    ast->args = NULL;
	return (ast);
}
