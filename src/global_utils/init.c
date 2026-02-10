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

t_ast	*init_ast(void)
{
	t_ast	*ast;

	ast = malloc(sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->type = T_STRING;
	ast->left = NULL;
    ast->right = NULL;
    ast->redirs = NULL;
    ast->args = NULL;
	return (ast);
}
