/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_calc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:34:36 by mring             #+#    #+#             */
/*   Updated: 2025/12/08 15:44:32 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static double	epsilon(t_rt *scene, t_obj *obj, double closest_t)
{
	if (obj->t > 0 && obj->t < closest_t)
	{
		scene->hit_obj = obj;
		return (obj->t);
	}
	return (closest_t);
}

void	calc_objs(t_rt *scene)
{
	t_obj	*obj;
	double	closest_t;
	t_vec3	ray_origin;

	obj = scene->objects;
	ray_origin = scene->camera->pos;
	closest_t = INFINITY;
	scene->hit_obj = NULL;
	while (obj)
	{
		obj->ray_origin = ray_origin;
		obj->ray_dir = scene->objects->ray_dir;
		obj->t = -1.0;
		if (obj->type == SPHERE)
			obj->t = hit_sphere(obj);
		else if (obj->type == PLANE)
			obj->t = hit_plane(obj);
		else if (obj->type == CYLINDER)
			obj->t = hit_cylinder(obj);
		closest_t = epsilon(scene, obj, closest_t);
		obj = obj->next;
	}
	if (scene->hit_obj)
		scene->hit_obj->closest_t = closest_t;
}
