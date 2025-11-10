/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_cross.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:34:03 by mring             #+#    #+#             */
/*   Updated: 2025/11/10 17:39:15 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// calculates a new vector that is 90 degree to both a and b
// need result or direct return with temp struct
// else would overwrite still necessary results.
t_vec3	vec_cross(t_vec3 a, t_vec3 b)
{
    t_vec3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return (result);
}

// crossproduct is used for camera coordinate system
// surface normals for triangles

// compound literal version
// return ((t_vec3){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y
// 	- a.y * b.x});