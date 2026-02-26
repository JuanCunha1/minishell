/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 12:07:45 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/19 12:47:47 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*shorten_home(char *pwd, char *home)
{
	char	*tmp;

	if (!pwd || !home)
		return (ft_strdup(pwd));
	if (!ft_strncmp(pwd, home, ft_strlen(home)))
	{
		tmp = ft_strjoin("~", pwd + ft_strlen(home));
		if (!tmp)
			return (NULL);
		return (tmp);
	}
	return (ft_strdup(pwd));
}

char	*write_pwd(char **envp)
{
	char	*pwd;
	char	*home;
	char	*short_pwd;
	char	*tmp;
	char	*prompt;

	pwd = get_env_value("PWD", envp);
	home = get_env_value("HOME", envp);
	if (!pwd)
		pwd = "";
	short_pwd = shorten_home(pwd, home);
	if (!short_pwd)
		return (NULL);
	tmp = ft_strjoin("\001\033[1;32m\002minishell:\001\033[0m\002:"\
"\001\033[1;34m\002", short_pwd);
	free(short_pwd);
	if (!tmp)
		return (NULL);
	prompt = ft_strjoin(tmp, "\001\033[0m\002$ ");
	free(tmp);
	return (prompt);
}
