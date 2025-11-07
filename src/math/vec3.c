/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:53:44 by mring             #+#    #+#             */
/*   Updated: 2025/11/06 17:28:07 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec3	vec_add(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x + b.x, a.y + b.y, a.z + b.z});
}

// scale vectors
t_vec3	vec_scale(t_vec3 v, double k)
{
	return ((t_vec3){v.x * k, v.y * k, v.z * k});
}

// tells you how alligned two vectors are
float	vec_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

// returns a vector perpendicular to both a and b
t_vec3	vec_cross(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y
		- a.y * b.x});
}

float	vec_length(t_vec3 v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_vec3	vec_normalize(t_vec3 v)
{
	float	len;

	len = vec_length(v);
	return ((t_vec3){v.x / len, v.y / len, v.z / len});
}

/*
need a brdf function for light refraction on surfaces and color calculation
Bi-Directional Reflectance Distribution Function
the function calculates the color the ray will have after in bounces off a surface/intersects with any object
which can happen multiple times.
the more rays the more calculation the more calls the slower the program -> here we can optimize with logic


*/