/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:34:36 by mring             #+#    #+#             */
/*   Updated: 2025/11/25 10:59:28 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// bool	hit_plane(void)
// {
// }

// bool hit_cylinder()
// {
// }

bool	hit_sphere(t_vec3 ray_origin, t_vec3 ray_dir, t_vec3 sphere_center,
		double radius)
{
	float	a;
	float	b;
	float	c;

	// calcuate a, b, c
	// calculate discriminant
	// return true if dscriminant >= 0, else false
	a = vec_dot(ray_dir, ray_dir);
	b = 2.0 * vec_dot(ray_dir, vec_sub(ray_origin, sphere_center));
	c = vec_dot(vec_sub(ray_origin, sphere_center), vec_sub(ray_origin,
				sphere_center)) - radius * radius;
	return (b * b - 4.0 * a * c >= 0);
}

// void	render(t_camera *camera)
// {
// }

/*
If camera is at position (0, 0, 0) looking down +Z axis,
with FOV 90degree;
- What are the coordinates of the viewport's center point?
- What are the coordinates of the viewport's right edge?
- what are the coordinates of the top edge?
Center:					0, 0, 1
Right Edge Center:		1, 0, 1,
Left Edge Center:	-1, 0, 1
top edge center:		0, ar, 1
bottom edge center:		0, -ar, 1
*/