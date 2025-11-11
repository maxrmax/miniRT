/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_length.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:22:17 by mring             #+#    #+#             */
/*   Updated: 2025/11/10 17:28:51 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// length is the square root of the dot product with itself
// vec_dot(v, v)
// the return is its magnitude
float vec_length_squared(t_vec3 v)
{
    return (v.x * v.x + v.y * v.y + v.z * v.z);
}

float	vec_length(t_vec3 v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

// for comparing and checking thresholds: squared
// for actual value usage: length


// when to use squared:
// comparing distances
// finding closest object
// ray-sphere intersection math
// checking if vector is "close to zero"

// when we want to use length:
// normalizing vectors
// displaying/debugging actual distance
// calculating attenuation (light falloff)
// when formula explicitly needs length

// sqrt is expensive, avoid when possible!

