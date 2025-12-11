/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_mult.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:39:45 by mring             #+#    #+#             */
/*   Updated: 2025/12/11 09:50:02 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// scale vector v by scalar
t_vec3	vec_mult(t_vec3 v, float scalar)
{
	v.x *= scalar;
	v.y *= scalar;
	v.z *= scalar;
	return (v);
}

// alternative way to write it with a temporary struct
// compound literal
// return ((t_vec3){v.x * scalar, v.y * scalar, v.z * scalar});