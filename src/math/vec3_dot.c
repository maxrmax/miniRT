/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_dot.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:44:07 by mring             #+#    #+#             */
/*   Updated: 2025/12/11 09:53:30 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// a is ray_direction
// b is surface_normal
// light intensity: a is light_direction
// sphere:
// a = vec_dot(ray_dir, ray_dir)
// b = 2 * vec_dot(ray_dir, oc) origin of center
// dot product measures how aligned two vectors are
// returns a scalar
// same direction = big positive
// perpendicular = zero (will meet at a single point 90degree)
// opposite direction = negative

// TODO: description
float	vec_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

// usually used in condition checks
// computing intensity/brightness
// part of formulas like ray-sphere intersection
// reflection calculation