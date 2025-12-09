/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_calc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:25:55 by mring             #+#    #+#             */
/*   Updated: 2025/12/09 17:34:21 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	calc_brightness(t_rt *scene)
{
	scene->hit_obj->diffuse = fmax(0.0, vec_dot(scene->hit_obj->normal,
				scene->hit_obj->light_dir));
	if (scene->hit_obj->in_shadow)
		scene->hit_obj->diffuse = 0.0;
	scene->hit_obj->brightness = scene->ambient->ratio
		+ (scene->light->brightness * scene->hit_obj->diffuse);
	scene->hit_obj->brightness = fmin(scene->hit_obj->brightness, 1.0);
}

static double	get_shadow_t(t_obj *shadow_obj)
{
	if (shadow_obj->type == SPHERE)
		return (hit_sphere(shadow_obj));
	else if (shadow_obj->type == PLANE)
		return (hit_plane(shadow_obj));
	else if (shadow_obj->type == CYLINDER)
		return (hit_cylinder(shadow_obj));
	return (-1.0);
}

static bool	check_shadow(t_rt *scene)
{
	t_obj	*shadow_obj;
	double	shadow_t;
	t_vec3	shadow_origin;

	shadow_origin = vec_add(scene->hit_obj->hit_point,
			vec_mult(scene->hit_obj->normal, 0.001));
	shadow_obj = scene->objects;
	while (shadow_obj)
	{
		shadow_obj->ray_origin = shadow_origin;
		shadow_obj->ray_dir = scene->hit_obj->light_dir;
		shadow_t = get_shadow_t(shadow_obj);
		if (shadow_t > 0.001 && shadow_t < scene->hit_obj->light_dist)
			return (true);
		shadow_obj = shadow_obj->next;
	}
	return (false);
}

static void	setup_light_dir(t_obj *hit_obj, t_vec3 light_pos)
{
	hit_obj->light_dir = vec_sub(light_pos, hit_obj->hit_point);
	hit_obj->light_dist = vec_length(hit_obj->light_dir);
	hit_obj->light_dir = vec_normalize(hit_obj->light_dir);
}

void	light_calc(t_rt *scene)
{
	setup_light_dir(scene->hit_obj, scene->light->pos);
	scene->hit_obj->in_shadow = check_shadow(scene);
	calc_brightness(scene);
}
