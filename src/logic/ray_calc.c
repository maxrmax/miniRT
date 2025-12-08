/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_calc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 13:06:41 by mring             #+#    #+#             */
/*   Updated: 2025/12/08 15:51:04 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	light_calc(t_rt *scene)
{
	t_obj	*shadow_obj;
	double	shadow_t;

	scene->hit_obj->light_dir = vec_sub(scene->light->pos,
			scene->hit_obj->hit_point);
	scene->hit_obj->light_dist = vec_length(scene->hit_obj->light_dir);
	scene->hit_obj->light_dir = vec_normalize(scene->hit_obj->light_dir);
	scene->hit_obj->in_shadow = false;
	shadow_obj = scene->objects;
	while (shadow_obj)
	{
		shadow_obj->ray_origin = vec_add(scene->hit_obj->hit_point,
				vec_mult(scene->hit_obj->normal, 0.001));
		shadow_obj->ray_dir = scene->hit_obj->light_dir;
		shadow_t = -1.0;
		if (shadow_obj->type == SPHERE)
			shadow_t = hit_sphere(shadow_obj);
		else if (shadow_obj->type == PLANE)
			shadow_t = hit_plane(shadow_obj);
		else if (shadow_obj->type == CYLINDER)
			shadow_t = hit_cylinder(shadow_obj);
		if (shadow_t > 0.001 && shadow_t < scene->hit_obj->light_dist)
		{
			scene->hit_obj->in_shadow = true;
			break ;
		}
		shadow_obj = shadow_obj->next;
	}
	scene->hit_obj->diffuse = fmax(0.0, vec_dot(scene->hit_obj->normal,
				scene->hit_obj->light_dir));
	if (scene->hit_obj->in_shadow)
		scene->hit_obj->diffuse = 0.0;
	scene->hit_obj->brightness = scene->ambient->ratio
		+ (scene->light->brightness * scene->hit_obj->diffuse);
	scene->hit_obj->brightness = fmin(scene->hit_obj->brightness, 1.0);
}

static void	calc_cylinder_normal(t_obj *obj)
{
	t_vec3	hit_to_center;
	t_vec3	axis_point;
	double	proj;

	hit_to_center = vec_sub(obj->hit_point, obj->data.cy.center);
	proj = vec_dot(hit_to_center, obj->data.cy.axis);
	if (proj < 0.001)
		obj->normal = vec_negate(obj->data.cy.axis);
	else if (proj > obj->data.cy.height - 0.001)
		obj->normal = obj->data.cy.axis;
	else
	{
		axis_point = vec_add(obj->data.cy.center, vec_mult(obj->data.cy.axis,
					proj));
		obj->normal = vec_normalize(vec_sub(obj->hit_point, axis_point));
	}
}

void	hit_calc(t_obj *obj)
{
	obj->hit_point = vec_add(obj->ray_origin, vec_mult(obj->ray_dir,
				obj->closest_t));
	if (obj->type == SPHERE)
		obj->normal = vec_normalize(vec_sub(obj->hit_point,
					obj->data.sp.center));
	else if (obj->type == PLANE)
		obj->normal = obj->data.pl.normal;
	// cylinders are a bitch
	else if (obj->type == CYLINDER)
		calc_cylinder_normal(obj);
}
