/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_simple_quot.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rubmedin <rubmedin@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 18:53:37 by rubmedin          #+#    #+#             */
/*   Updated: 2025/10/09 19:02:28 by rubmedin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


void	init_flags(t_flags *flags)
{
	flags->flag_simple_quot = 0;
	flags->flag_double_quot = 0;
	flags->flag_heredoc = 0;
}

int	is_heredoc(char *str, t_flags *flags)
{
	int	iter;

	iter = 0;
	while(str[iter])
	{
		if (str[iter] == '<' && str[iter + 1] && str[iter + 1] == '<' && 
			flags->flag_heredoc == 0)
		{
			flags->flag_heredoc = 1;
			return (1);
		}
		else if(str[iter] == '<' && str[iter + 1] && str[iter + 1] == '<' && 
			flags->flag_heredoc == 1)
		{
			flags->flag_heredoc = 0;
			return (1);
		}
		else if  (flags->flag_heredoc == 1)
			return (1);
		iter++;
	}
	return (0);
}

int	is_simple_quoted(char *str, t_flags *flags)
{
	int	iter;

	iter = 0;
	while(str[iter])
	{
		if (str[iter] == '\'' && flags->flag_simple_quot == 0)
		{
			flags->flag_simple_quot = 1;
			return (1);
		}
		else if (str[iter] == '\'' && flags->flag_simple_quot == 1)
		{
			flags->flag_simple_quot = 0;
			return (1);
		}
		else if  (flags->flag_simple_quot == 1)
			return (1);
		iter++;
	}
	return (0);
}

int	is_double_quoted(char *str, t_flags *flags)
{
	int	iter;

	iter = 0;
	while(str[iter])
	{
		if (str[iter] == '\"' && flags->flag_double_quot == 0)
		{
			flags->flag_double_quot = 1;
			return (1);
		}
		else if (str[iter] == '\"' && flags->flag_double_quot == 1)
		{
			flags->flag_double_quot = 0;
			return (1);
		}
		else if (flags->flag_double_quot == 1)
			return (2);
		iter++;
	}
	return (0);
}

int	is_pipe(char *str, t_flags *flags)
{
	if((!flags->flag_simple_quot || !flags->flag_double_quot) && 
		!ft_strncmp(str, "|", ft_strlen(str)))
			return (1);
	return (0);
}

int	is_or_operator(char *str, t_flags *flags)
{
	if((!flags->flag_simple_quot || !flags->flag_double_quot) && 
		!ft_strncmp(str, "||", ft_strlen(str)))
			return (1);
	return (0);
}