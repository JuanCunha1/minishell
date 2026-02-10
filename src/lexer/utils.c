/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rubmedin <rubmedin@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 20:30:29 by rubmedin          #+#    #+#             */
/*   Updated: 2025/10/07 18:46:18 by rubmedin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*asignar_palabra(const char *s, int len)
{
	char	*palabra;
	int		i;
	int		j;
	int		desp;

	desp = 0;
	if ((s[0] == '\'' && s[len - 1] == '\'')
		|| (s[0] == '"' && s[len - 1] == '"'))
	{
		desp = 1;
		len -= 2;
	}
	palabra = malloc((len + 1) * sizeof(char));
	if (palabra == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		palabra[j++] = s[i + desp];
		i++;
	}
	palabra[j] = '\0';
	return (palabra);
}
int	ft_count_word(char const *s, char c)
{
	int	num;
	int	palabra_flag;
	int	comi_simple;
	int	comi_doble;

	num = 0;
	palabra_flag = 0;
	comi_simple = 0;
	comi_doble = 0;
	while (*s != '\0')
	{
		if (*s == '\'' && !comi_doble)
			comi_simple = 1 - comi_simple;
		else if (*s == '\"' && !comi_simple)
			comi_doble = 1 - comi_doble;
		if (*s != c && !palabra_flag)
		{
			palabra_flag = 1;
			num++;
		}
		else if (*s == c && !comi_doble && !comi_simple)
			palabra_flag = 0;
		s++;
	}
	return (num);
}

int	contador_letras_comis(char const *s, char c)
{
	int	letras;
	int	comi_simple;
	int	comi_doble;

	letras = 0;
	comi_simple = 0;
	comi_doble = 0;
	while (s[letras])
	{
		if (s[letras] == '\'' && !comi_doble)
			comi_simple = 1 - comi_simple;
		else if (s[letras] == '\"' && !comi_simple)
			comi_doble = 1 - comi_doble;
		if (s[letras] == c && !comi_doble && !comi_simple)
			break ;
		letras++;
	}
	return (letras);
}

/**** Free de array de 2 dimensiones ****/
void	free2d(char **arr)
{
	int	i;
	
	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		if (arr[i])
			free(arr[i++]);
	}
	if (arr)
		free(arr);
}

/******Copia entorno******/

char	**envp_dup(char **ae)
{
	char	**copy;
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (ae[len])
		len++;
	copy = malloc(sizeof(char *) * (len + 1));
	if (!copy)
		return NULL;
	while (i < len)
	{
		copy[i] = ft_strdup(ae[i]);
		i++;
	}
	copy[len] = NULL;
	return (copy);
}
