/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:25:12 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/21 16:30:50 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	add_obj(t_rt *scene, t_obj_type type, t_obj_data data)
{
	t_obj	*new_obj;

	new_obj = malloc(sizeof(t_obj));
	if (!new_obj)
	{
		printf("Malloc faild\n");
		free_scenes(scene);
		return ;
	}
	new_obj->type = type;
	new_obj->data = data;
	new_obj->next = scene->objects;
	scene->objects = new_obj;
}
