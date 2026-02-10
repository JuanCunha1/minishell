#include "minishell.h"

static int	env_length(char **env)
{
	int	i;

	if (!env)
		return (0);
	i = 0;
	while (env[i])
		i++;
	return (i);
}

static void	append_env_var(t_shell *shell, char *arg)
{
	int		len;
	char	**tmp;

	len = env_length(shell->envp);
	tmp = (char **)realloc(shell->envp, sizeof(char *) * (len + 2));
	if (!tmp)
	{
		perror("realloc failed");
		return ;
	}
	shell->envp = tmp;
	shell->envp[len] = ft_strdup(arg);
	if (!shell->envp[len])
	{
		perror("strdup failed");
		shell->envp[len] = NULL;
		return ;
	}
	shell->envp[len + 1] = NULL;
}

static int	get_env_index(char **envp, char *name)
{
	int		i;
	int		len;

	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, len)
			&& envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static int	is_valid_env_name(char *s)
{
	int		i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	handle_export_arg(t_shell *shell, char *name, int has_equal, char *arg)
{
	int	idx;

	if (!is_valid_env_name(name))
	{
		ft_putstr_fd("export: not a valid identifier", 2);
		ft_putendl_fd(name, 2);
		return ;
	}
	idx = get_env_index(shell->envp, name);
	if (!has_equal)
		return ;
	if (idx != -1)
	{
		free(shell->envp[idx]);
		if (has_equal)
			shell->envp[idx] = ft_strdup(arg);
		else
			shell->envp[idx] = ft_strdup(shell->envp[idx]);
		if (!shell->envp[idx])
			perror("strdup failed");
		return ;
	}
	else
		append_env_var(shell, arg);
}
