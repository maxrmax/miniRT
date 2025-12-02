/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:34:36 by mring             #+#    #+#             */
/*   Updated: 2025/12/02 09:09:50 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double	hit_sphere(t_vec3 ray_origin, t_vec3 ray_dir, t_vec3 sphere_center,
		double radius)
{
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t;

	// calcuate a, b, c
	// calculate discriminant
	// return true if dscriminant >= 0, else false
	a = vec_dot(ray_dir, ray_dir);
	b = 2.0 * vec_dot(ray_dir, vec_sub(ray_origin, sphere_center));
	c = vec_dot(vec_sub(ray_origin, sphere_center), vec_sub(ray_origin,
				sphere_center)) - radius * radius;
	discriminant = b * b - 4.0 * a * c;
	if (discriminant < 0)
		return (-1.0);
	t = (-b - sqrt(discriminant)) / (2.0 * a);
	if (t < 0)
		return (-1.0);
	return (t);
}

// t = dot(plane_point - ray_origin, plane_normal) / dot(ray_dir, plane_normal)
double	hit_plane(t_vec3 ray_origin, t_vec3 ray_dir, t_vec3 plane_point,
		t_vec3 plane_normal)
{
	double	denom;
	double	t;

	denom = vec_dot(ray_dir, plane_normal);
	if (fabs(denom) < 1e-6) // parallel
		return (-1.0);
	t = vec_dot(vec_sub(plane_point, ray_origin), plane_normal) / denom;
	if (t < 0)
		return (-1.0);
	return (t);
}

double	hit_cylinder(t_vec3 ray_origin, t_vec3 ray_dir, t_vec3 cy_center,
		t_vec3 cy_axis, double radius, double height)
{
	t_vec3	oc;
	t_vec3	ray_perp;
	t_vec3	oc_perp;
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t1;
	double	t2;
	double	t;
	t_vec3	p1;
	double	h1;
	t_vec3	p2;
	double	h2;
	double	cap_denom;
	double	t_bottom;
	t_vec3	p_bottom;
	t_vec3	to_center;
	t_vec3	top_center;
	double	t_top;
	t_vec3	p_top;
	t_vec3	to_top_center;
	t_vec3	radial;
	t_vec3	radial_top;

	// Vector from ray origin to cylinder base
	oc = vec_sub(ray_origin, cy_center);
	// Project ray direction and oc onto plane perpendicular to cylinder axis
	ray_perp = vec_sub(ray_dir, vec_mult(cy_axis, vec_dot(ray_dir, cy_axis)));
	oc_perp = vec_sub(oc, vec_mult(cy_axis, vec_dot(oc, cy_axis)));
	// Quadratic equation coefficients
	a = vec_dot(ray_perp, ray_perp);
	b = 2.0 * vec_dot(ray_perp, oc_perp);
	c = vec_dot(oc_perp, oc_perp) - radius * radius;
	discriminant = b * b - 4.0 * a * c;
	if (discriminant < 0)
		return (-1.0);
	t1 = (-b - sqrt(discriminant)) / (2.0 * a);
	t2 = (-b + sqrt(discriminant)) / (2.0 * a);
	// Check both intersection points against height bounds
	t = -1.0;
	p1 = vec_add(ray_origin, vec_mult(ray_dir, t1));
	h1 = vec_dot(vec_sub(p1, cy_center), cy_axis);
	if (t1 > 0 && h1 >= 0 && h1 <= height)
		t = t1;
	else
	{
		p2 = vec_add(ray_origin, vec_mult(ray_dir, t2));
		h2 = vec_dot(vec_sub(p2, cy_center), cy_axis);
		if (t2 > 0 && h2 >= 0 && h2 <= height)
			t = t2;
	}
	cap_denom = vec_dot(ray_dir, cy_axis);
	if (fabs(cap_denom) > 1e-6)
	{
		// Bottom cap
		t_bottom = vec_dot(vec_sub(cy_center, ray_origin), cy_axis) / cap_denom;
		if (t_bottom > 0)
		{
			p_bottom = vec_add(ray_origin, vec_mult(ray_dir, t_bottom));
			to_center = vec_sub(p_bottom, cy_center);
			radial = vec_sub(to_center, vec_mult(cy_axis, vec_dot(to_center,
							cy_axis)));
			if (vec_dot(radial, radial) <= radius * radius)
			{
				if (t < 0 || t_bottom < t)
					t = t_bottom;
			}
		}
		// Top cap
		top_center = vec_add(cy_center, vec_mult(cy_axis, height));
		t_top = vec_dot(vec_sub(top_center, ray_origin), cy_axis) / cap_denom;
		if (t_top > 0)
		{
			p_top = vec_add(ray_origin, vec_mult(ray_dir, t_top));
			to_top_center = vec_sub(p_top, top_center);
			radial_top = vec_sub(to_top_center, vec_mult(cy_axis,
						vec_dot(to_top_center, cy_axis)));
			if (vec_dot(radial_top, radial_top) <= radius * radius)
			{
				if (t < 0 || t_top < t)
					t = t_top;
			}
		}
	}
	return (t);
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