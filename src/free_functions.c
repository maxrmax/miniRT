/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 05:08:14 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/19 23:15:56 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void free_scenes(t_rt *scene)
{
    t_obj *current;
    t_obj *next;

    if (!scene)
        return;
    current = scene->objects;
    if (scene->ambient)
        free(scene->ambient);
    if (scene->camera)
        free(scene->camera);
    if (scene->light)
        free(scene->light);
    while (current)
    {
        next = current->next;
        free(current);
        current = next;
    }
    free(scene);
}

void	ft_free_split(char **arr)
{
	int	i;

	if (!arr)
		return;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}