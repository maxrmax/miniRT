/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 12:33:03 by mring             #+#    #+#             */
/*   Updated: 2025/12/08 12:34:17 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
