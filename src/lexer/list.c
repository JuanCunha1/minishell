/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rubmedin <rubmedin@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:56:27 by rubmedin          #+#    #+#             */
/*   Updated: 2025/10/07 19:35:35 by rubmedin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void adding_token(t_token **l_tokens, t_token *token)
{
	t_token *buff_token;

	if (!*l_tokens)
	{
		token->next = NULL;
		*l_tokens = token;
		return ;
	}
	buff_token = *l_tokens;
	while (buff_token->next)
		buff_token = buff_token->next;
	buff_token->next = token;
	token->next = NULL;
}

static t_token *create_token(char *str, t_flags *flags)
{
	t_token *buff_tmp;

	buff_tmp = malloc(sizeof(t_token));
	if(!buff_tmp)
		return (NULL);
	buff_tmp->data = ft_strdup(str);
	if (!buff_tmp->data)
	{
		free(buff_tmp);
		return (NULL);
	}
	buff_tmp->type_tok = getype(buff_tmp->data, flags); /*A MEDIO HACER*/
	buff_tmp->next = NULL;
	return (buff_tmp);
}

void	init_list(t_token **tokens, char *str, t_flags *flags)
{
	t_token *buff_tmp;
	int		i;
	int		count;
	char	*cpy_str;
	char	*buff_str;

	i = 0;
	buff_str = ft_strdup(str);
	count = ft_count_word(str, ' ');
	while(i < count)
	{
		cpy_str = ft_substr(buff_str, 0, contador_letras_comis(buff_str, ' '));
		buff_str = &buff_str[contador_letras_comis(buff_str, ' ') + 1];
		buff_tmp = create_token(cpy_str, flags);
		free(cpy_str);
		if(!buff_tmp)
			return ;
		adding_token(tokens, buff_tmp);
		i++;
	}
}

void print_list(t_token **l_tokens)
{
	t_token *buff_token;

	if (!*l_tokens)
		exit(1);
	buff_token = *l_tokens;
	while(buff_token)
	{
		printf("\ndata: %s\n type_tok: %i\n", buff_token->data, buff_token->type_tok);
		buff_token = buff_token->next;
	}
}
