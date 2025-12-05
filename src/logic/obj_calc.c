/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_calc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:34:36 by mring             #+#    #+#             */
/*   Updated: 2025/12/05 15:42:45 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double	hit_sphere(t_obj *obj)
{
	// calcuate a, b, c
	// calculate discriminant
	// return true if discriminant >= 0, else false
	obj->data.sp.a = vec_dot(obj->ray_dir, obj->ray_dir);
	obj->data.sp.b = 2.0 * vec_dot(obj->ray_dir, vec_sub(obj->ray_origin,
				obj->data.sp.center));
	obj->data.sp.c = vec_dot(vec_sub(obj->ray_origin, obj->data.sp.center),
			vec_sub(obj->ray_origin, obj->data.sp.center))
		- obj->data.sp.diameter * obj->data.sp.diameter / 4;
	obj->discriminant = obj->data.sp.b * obj->data.sp.b - 4.0 * obj->data.sp.a
		* obj->data.sp.c;
	if (obj->discriminant < 0)
		return (-1.0);
	obj->t = (-obj->data.sp.b - sqrt(obj->discriminant)) / (2.0
			* obj->data.sp.a);
	if (obj->t < 0)
		return (-1.0);
	return (obj->t);
}

// t = dot(plane_point - ray_origin, plane_normal) / dot(ray_dir, plane_normal)
double	hit_plane(t_obj *obj)
{
	obj->data.pl.denom = vec_dot(obj->ray_dir, obj->data.pl.normal);
	if (fabs(obj->data.pl.denom) < 1e-6) // parallel
		return (-1.0);
	obj->t = vec_dot(vec_sub(obj->data.pl.point, obj->ray_origin),
			obj->data.pl.normal) / obj->data.pl.denom;
	if (obj->t < 0)
		return (-1.0);
	return (obj->t);
}

double	hit_cylinder(t_obj *obj)
{
	// Vector from ray origin to cylinder base
	obj->data.cy.objcent = vec_sub(obj->ray_origin, obj->data.cy.center);
	// Project ray direction and oc onto plane perpendicular to cylinder axis
	obj->data.cy.ray_perp = vec_sub(obj->ray_dir, vec_mult(obj->data.cy.axis,
				vec_dot(obj->ray_dir, obj->data.cy.axis)));
	obj->data.cy.oc_perp = vec_sub(obj->data.cy.objcent,
			vec_mult(obj->data.cy.axis, vec_dot(obj->data.cy.objcent,
					obj->data.cy.axis)));
	obj->data.cy.a = vec_dot(obj->data.cy.ray_perp, obj->data.cy.ray_perp);
	obj->data.cy.b = 2.0 * vec_dot(obj->data.cy.ray_perp, obj->data.cy.oc_perp);
	obj->data.cy.c = vec_dot(obj->data.cy.oc_perp, obj->data.cy.oc_perp)
		- obj->data.cy.diameter * obj->data.cy.diameter / 4;
	obj->discriminant = obj->data.cy.b * obj->data.cy.b - 4.0 * obj->data.cy.a
		* obj->data.cy.c;
	if (obj->discriminant < 0)
		return (-1.0);
	obj->data.cy.t1 = (-obj->data.cy.b - sqrt(obj->discriminant)) / (2.0
			* obj->data.cy.a);
	obj->data.cy.t2 = (-obj->data.cy.b + sqrt(obj->discriminant)) / (2.0
			* obj->data.cy.a);
	// Check both intersection points against height bounds
	obj->t = -1.0;
	obj->data.cy.p1 = vec_add(obj->ray_origin, vec_mult(obj->ray_dir,
				obj->data.cy.t1));
	obj->data.cy.h1 = vec_dot(vec_sub(obj->data.cy.p1, obj->data.cy.center),
			obj->data.cy.axis);
	if (obj->data.cy.t1 > 0 && obj->data.cy.h1 >= 0
		&& obj->data.cy.h1 <= obj->data.cy.height)
		obj->t = obj->data.cy.t1;
	else
	{
		obj->data.cy.p2 = vec_add(obj->ray_origin, vec_mult(obj->ray_dir,
					obj->data.cy.t2));
		obj->data.cy.h2 = vec_dot(vec_sub(obj->data.cy.p2, obj->data.cy.center),
				obj->data.cy.axis);
		if (obj->data.cy.t2 > 0 && obj->data.cy.h2 >= 0
			&& obj->data.cy.h2 <= obj->data.cy.height)
			obj->t = obj->data.cy.t2;
	}
	obj->data.cy.cap_denom = vec_dot(obj->ray_dir, obj->data.cy.axis);
	if (fabs(obj->data.cy.cap_denom) > 1e-6)
	{
		// Bottom cap
		obj->data.cy.t_bottom = vec_dot(vec_sub(obj->data.cy.center,
					obj->ray_origin), obj->data.cy.axis)
			/ obj->data.cy.cap_denom;
		if (obj->data.cy.t_bottom > 0)
		{
			obj->data.cy.p_bottom = vec_add(obj->ray_origin,
					vec_mult(obj->ray_dir, obj->data.cy.t_bottom));
			obj->data.cy.to_center = vec_sub(obj->data.cy.p_bottom,
					obj->data.cy.center);
			obj->data.cy.radial = vec_sub(obj->data.cy.to_center,
					vec_mult(obj->data.cy.axis, vec_dot(obj->data.cy.to_center,
							obj->data.cy.axis)));
			if (vec_dot(obj->data.cy.radial,
					obj->data.cy.radial) <= obj->data.cy.diameter
				* obj->data.cy.diameter / 4)
			{
				if (obj->t < 0 || obj->data.cy.t_bottom < obj->t)
					obj->t = obj->data.cy.t_bottom;
			}
		}
		// Top cap
		obj->data.cy.top_center = vec_add(obj->data.cy.center,
				vec_mult(obj->data.cy.axis, obj->data.cy.height));
		obj->data.cy.t_top = vec_dot(vec_sub(obj->data.cy.top_center,
					obj->ray_origin), obj->data.cy.axis)
			/ obj->data.cy.cap_denom;
		if (obj->data.cy.t_top > 0)
		{
			obj->data.cy.p_top = vec_add(obj->ray_origin, vec_mult(obj->ray_dir,
						obj->data.cy.t_top));
			obj->data.cy.to_top_center = vec_sub(obj->data.cy.p_top,
					obj->data.cy.top_center);
			obj->data.cy.radial_top = vec_sub(obj->data.cy.to_top_center,
					vec_mult(obj->data.cy.axis,
						vec_dot(obj->data.cy.to_top_center,
							obj->data.cy.axis)));
			if (vec_dot(obj->data.cy.radial_top,
					obj->data.cy.radial_top) <= obj->data.cy.diameter
				* obj->data.cy.diameter / 4)
			{
				if (obj->t < 0 || obj->data.cy.t_top < obj->t)
					obj->t = obj->data.cy.t_top;
			}
		}
	}
	return (obj->t);
}

// refactor start
// TODO:
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
		// if t is above 0 and smaller than closest_t (iirc t was time in math)
		// TODO: subfunction EPSILON
		if (obj->t > 0 && obj->t < closest_t)
		{
			closest_t = obj->t;
			scene->hit_obj = obj; // may can move hit_obj into t_obj
		}
		obj = obj->next;
	}
	if (scene->hit_obj)
		scene->hit_obj->closest_t = closest_t;
}
