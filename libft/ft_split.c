/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:27:45 by jmarques          #+#    #+#             */
/*   Updated: 2025/05/05 15:02:32 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_number_words(char const *s, char c)
{
	size_t	count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	**free_words(size_t i, char **words)
{
	while (i > 0)
	{
		i--;
		free(words[i]);
	}
	free(words);
	return (NULL);
}

static char	**ft_split_helper(char const *s, char c, char **words)
{
	size_t		i;
	const char	*start;

	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			start = s;
			while (*s && *s != c)
				s++;
			words[i] = ft_substr(start, 0, s - start);
			if (!words[i])
				return (free_words(i, words));
			i++;
		}
		else
			s++;
	}
	words[i] = NULL;
	return (words);
}

char	**ft_split(char const *s, char c)
{
	char	**words;
	size_t	count_words;

	if (!s)
		return (NULL);
	count_words = count_number_words(s, c);
	words = (char **)malloc(sizeof(char *) * (count_words + 1));
	if (!words)
		return (NULL);
	if (count_words == 0)
	{
		words[0] = NULL;
		return (words);
	}
	return (ft_split_helper(s, c, words));
}
