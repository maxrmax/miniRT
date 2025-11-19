/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:22:09 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/19 16:47:43 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int count_array_elements(char **arr)
{
    int count = 0;
    if (!arr)
        return (0);
    while (arr[count])
        count++;
    return (count);
}