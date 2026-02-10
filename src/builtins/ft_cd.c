/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rubmedin <rubmedin@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:46:14 by rubmedin          #+#    #+#             */
/*   Updated: 2025/12/04 15:46:48 by rubmedin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd(char *old_pwd, t_shell *shell)
{
	int		i;
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		free(old_pwd);
		return ;
	}
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "PWD=", 4) == 0)
		{
			free(shell->envp[i]);
			shell->envp[i] = ft_strjoin("PWD=", new_pwd);
		}
		else if (ft_strncmp(shell->envp[i], "OLDPWD=", 7) == 0)
		{
			free(shell->envp[i]);
			shell->envp[i] = ft_strjoin("OLDPWD=", old_pwd);
		}
		i++;
	}
	free(new_pwd);
}

char	*get_env_value(char **envp, char *key)
{
	int	len;

	len = ft_strlen(key);
	while (*envp)
	{
		if (ft_strncmp(*envp, key, len) == 0 && (*envp)[len] == '=')
			return (*envp + len + 1);
		envp++;
	}
	return (NULL);
}

static char	*get_cd_path(t_ast *ast, t_shell *shell)
{
	char	*home;

	if (!ast->args[1])
	{
		home = get_env_value(shell->envp, "HOME");
		if (!home)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			shell->exit_status = 1;
			return (NULL);
		}
		return (home);
	}
	return (ast->args[1]);
}

void	ft_cd(t_ast *ast, t_shell *shell)
{
	char	*old_pwd;
	char	*next_path;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return ;
	next_path = get_cd_path(ast, shell);
	if (!next_path)
	{
		free(old_pwd);
		return ;
	}
	if (chdir(next_path) == -1)
	{
		perror("cd");
		shell->exit_status = 1;
		free(old_pwd);
		return ;
	}
	update_pwd(old_pwd, shell);
	free(old_pwd);
	shell->exit_status = 0;
}
