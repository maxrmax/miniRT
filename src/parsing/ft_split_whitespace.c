/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_whitespaces.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:28:07 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/06 13:36:11 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

char **ft_split_whitespace(char *str)
{
    char **tokens;
    int i = 0;
    int j = 0;
    int count = 0;
    
    // Count tokens
    while (str[i])
    {
        // Skip whitespace
        while (str[i] && (str[i] == ' ' || str[i] == '\t' || 
               str[i] == '\n' || str[i] == '\r'))
            i++;
        if (str[i])
            count++;
        // Skip non-whitespace
        while (str[i] && str[i] != ' ' && str[i] != '\t' && 
               str[i] != '\n' && str[i] != '\r')
            i++;
    }
    
    tokens = malloc(sizeof(char *) * (count + 1));
    if (!tokens)
        return (NULL);
    
    // Extract tokens
    i = 0;
    while (str[i])
    {
        while (str[i] && (str[i] == ' ' || str[i] == '\t' || 
               str[i] == '\n' || str[i] == '\r'))
            i++;
        if (str[i])
        {
            int start = i;
            while (str[i] && str[i] != ' ' && str[i] != '\t' && 
                   str[i] != '\n' && str[i] != '\r')
                i++;
            tokens[j] = ft_substr(str, start, i - start);
            j++;
        }
    }
    tokens[j] = NULL;
    return (tokens);
}
