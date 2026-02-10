#include "minishell.h"

int	ft_env(char **args, char **envp)
{
	int	i;

	if (!envp || args[1])
		return (1);
	if (args[1] && args[1][0] == '-')
	{
		printf("env: invalid option '%s'\n", args[1]);
		return (125);
	}
	if (args[1])
	{
		printf("env: '%s': No such file or directory\n", args[1]);
		return (127);
	}
	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}
