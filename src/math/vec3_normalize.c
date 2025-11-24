/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_normalize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:29:05 by mring             #+#    #+#             */
/*   Updated: 2025/11/20 16:25:49 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// takes any vector and scales it so its length becomes exactly 1.0
// keeping its direction the same
t_vec3	vec_normalize(t_vec3 v)
{
	float	len;

	len = vec_length(v);
	if (len == 0)
		return ((t_vec3){0, 0, 0});
	v.x /= len;
	v.y /= len;
	v.z /= len;
	return (v);
}

// why normalize?
// when you only care about the direction not length
// brightness needs vector length 1
// consistent movement speed
// some math formulas expect it

// normalized(v) = v / |v|
// |v| is the length

// return ((t_vec3){v.x / len, v.y / len, v.z / len});