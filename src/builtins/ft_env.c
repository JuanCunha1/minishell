#include "minishell.h"

void	ft_env(t_shell *sh)
{
	int	i;

	if (!sh || !sh->envp)
		return ;
	if (sh->ast->args[1] && sh->ast->args[1][0] == '-')
	{
		printf("env: invalid option '%s'\n", sh->ast->args[1]);
		sh->exit_status = 125;
		return ;
	}
	if (sh->ast->args[1])
	{
		printf("env: '%s': No such file or directory\n", sh->ast->args[1]);
		sh->exit_status = 127;
		return ;
	}
	i = 0;
	while (sh->envp[i])
	{
		printf("%s\n", sh->envp[i]);
		i++;
	}
	sh->exit_status = 0;
}
