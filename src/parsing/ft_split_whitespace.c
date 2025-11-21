/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_whitespace.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:28:07 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/21 16:57:49 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

#include <stdlib.h>

static int	is_ws(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static int	count_words(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && is_ws(str[i]))
			i++;
		if (str[i])
			count++;
		while (str[i] && !is_ws(str[i]))
			i++;
	}
	return (count);
}

char	**ft_split_whitespace(char *str)
{
	char	**tokens;
	int		i;
	int		j;
	int		start;

	tokens = malloc(sizeof(char *) * (count_words(str) + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && is_ws(str[i]))
			i++;
		if (str[i])
		{
			start = i;
			while (str[i] && !is_ws(str[i]))
				i++;
			tokens[j++] = ft_substr(str, start, i - start);
		}
	}
	tokens[j] = NULL;
	return (tokens);
}
