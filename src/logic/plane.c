/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 12:33:41 by mring             #+#    #+#             */
/*   Updated: 2025/12/08 12:39:36 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// t = dot(plane_point - ray_origin, plane_normal) / dot(ray_dir, plane_normal)
double	hit_plane(t_obj *obj)
{
	double	denom;

	denom = vec_dot(obj->ray_dir, obj->data.pl.normal);
	if (fabs(denom) < 1e-6)
		return (-1.0);
	obj->t = vec_dot(vec_sub(obj->data.pl.point, obj->ray_origin),
			obj->data.pl.normal) / denom;
	if (obj->t < 0)
		return (-1.0);
	return (obj->t);
}
