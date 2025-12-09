/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_calc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:14:38 by mring             #+#    #+#             */
/*   Updated: 2025/12/09 17:24:31 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	check_cap(t_obj *obj, t_cap *cap, t_vec3 cap_center,
		double cap_denom)
{
	t_vec3	to_center;

	cap->t = vec_dot(vec_sub(cap_center, obj->ray_origin), obj->data.cy.axis)
		/ cap_denom;
	// if t exist, basically?
	if (cap->t > 0)
	{
		// p = vec_add (ray_origin + vec_mult(ray_dir * t)
		cap->p = vec_add(obj->ray_origin, vec_mult(obj->ray_dir, cap->t));
		// to_center = vec_sub(p - center)
		to_center = vec_sub(cap->p, cap_center);
		// radial = vec_sub(to_center 0 - vec_mult(axis
		// * vec_dot(to_center ** axis)))
		cap->radial = vec_sub(to_center, vec_mult(obj->data.cy.axis,
					vec_dot(to_center, obj->data.cy.axis)));
		// if vec_dot(radial ** radial) <= circle area
		if (vec_dot(cap->radial, cap->radial) <= obj->data.cy.diameter
			* obj->data.cy.diameter / 4)
		{
			// if t < 0 or cap->t < t
			if (obj->t < 0 || cap->t < obj->t)
				// t = cap->t
				obj->t = cap->t;
		}
	}
}

void	cap_calc(t_obj *obj)
{
	t_vec3	top_center;
	t_cap	bottom;
	t_cap	top;
	double	cap_denom;

	// set cap denom to vec_dot of ray_dir and cy.axis
	cap_denom = vec_dot(obj->ray_dir, obj->data.cy.axis);
	// check absolute value of cap_denom bigger than 0.000001
	if (fabs(cap_denom) > 1e-6)
	{
		check_cap(obj, &bottom, obj->data.cy.center, cap_denom);
		top_center = vec_add(obj->data.cy.center, vec_mult(obj->data.cy.axis,
					obj->data.cy.height));
		check_cap(obj, &top, top_center, cap_denom);
	}
}

void	cy_height(t_obj *obj)
{
	double	h1;
	double	h2;

	// Check both intersection points against height bounds
	obj->t = -1.0;
	h1 = vec_dot(vec_sub(vec_add(obj->ray_origin, vec_mult(obj->ray_dir,
						obj->data.cy.t1)), obj->data.cy.center),
			obj->data.cy.axis);
	if (obj->data.cy.t1 > 0 && h1 >= 0 && h1 <= obj->data.cy.height)
		obj->t = obj->data.cy.t1;
	else
	{
		h2 = vec_dot(vec_sub(vec_add(obj->ray_origin, vec_mult(obj->ray_dir,
							obj->data.cy.t2)), obj->data.cy.center),
				obj->data.cy.axis);
		if (obj->data.cy.t2 > 0 && h2 >= 0 && h2 <= obj->data.cy.height)
			obj->t = obj->data.cy.t2;
	}
}

double	hit_cylinder(t_obj *obj)
{
	if (cy_base(obj) < 0)
		return (-1.0);
	cy_height(obj);
	cap_calc(obj);
	return (obj->t);
}
