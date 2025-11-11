/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isfloat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 05:29:01 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/11 05:29:39 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_isfloat(char *str)
{
    int i;
    int dot_count;

    if (!str || !*str)
        return (0);

    i = 0;
    dot_count = 0;
    if (str[i] == '+' || str[i] == '-')
        i++;
    if (!str[i])
        return (0);

    while (str[i])
    {
        if (str[i] == '.')
        {
            dot_count++;
            if (dot_count > 1)
                return (0);
        }
        else if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }

    return (1);
}
