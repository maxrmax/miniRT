/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_calc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 13:06:41 by mring             #+#    #+#             */
/*   Updated: 2025/12/11 09:34:44 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
	else if (obj->type == CYLINDER)
		calc_cylinder_normal(obj);
}
