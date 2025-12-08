/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 12:34:07 by mring             #+#    #+#             */
/*   Updated: 2025/12/08 12:38:49 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double	hit_sphere(t_obj *obj)
{
	double	a;
	double	b;
	double	c;
	double	discriminant;

	a = vec_dot(obj->ray_dir, obj->ray_dir);
	b = 2.0 * vec_dot(obj->ray_dir, vec_sub(obj->ray_origin,
				obj->data.sp.center));
	c = vec_dot(vec_sub(obj->ray_origin, obj->data.sp.center),
			vec_sub(obj->ray_origin, obj->data.sp.center))
		- obj->data.sp.diameter * obj->data.sp.diameter / 4;
	discriminant = b * b - 4.0 * a * c;
	if (discriminant < 0)
		return (-1.0);
	obj->t = (-b - sqrt(discriminant)) / (2.0 * a);
	if (obj->t < 0)
		return (-1.0);
	return (obj->t);
}
