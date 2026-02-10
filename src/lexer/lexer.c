/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_lexer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rubmedin <rubmedin@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 15:14:46 by rubmedin          #+#    #+#             */
/*   Updated: 2025/10/07 18:39:17 by rubmedin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **arr, int size)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	if (size == 0)
		while (arr[i])
			free(arr[i++]);
	else
		while (i < size)
		{
			if (arr[i])
				free(arr[i]);
			i++;
		}
	free(arr);
}

int	is_redir(char *str, t_flags *flags)
{
	if (flags->flag_double_quot)
		return (T_STRING);
	else if (str[0] == '<')
		return (T_REDIR_IN);
	else if (str[0] == '>' && str[1] == '>')
		return (T_REDIR_APPEND);
	else if (str[0] == '>')
		return (T_REDIR_OUT);
	return (T_STRING);
}

int	getype(char *str, t_flags *flags)
{
	if (is_simple_quoted(str, flags) && !flags->flag_double_quot)
		return (T_SIMPLE_QUOTED);
	else if (is_double_quoted(str, flags) && !flags->flag_simple_quot)
		return (T_DOUBLE_QUOTED);
	else if (is_pipe(str, flags))
		return (T_PIPE);
	else if (is_heredoc(str, flags))
		return (T_HEREDOC);
	else if (is_or_operator(str, flags))
		return (T_FLOW_OPERATOR);
	return (is_redir(str, flags));
}

/*Lexer se encarga de tokenizar, categorizar y mirar la sintaxys este bien escrita.*/
int	lexer(t_shell *sh)
{
	init_list(&sh->tokens, sh->str, &sh->flags);
	//print_list(l_tokens);
	return (0);
}
