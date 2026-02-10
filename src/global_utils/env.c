#include "minishell.h"

char	*get_env_value(const char *var, char **env)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(var);
	if (!env || len == 0)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (NULL);
}
