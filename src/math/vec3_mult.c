/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_mult.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:39:45 by mring             #+#    #+#             */
/*   Updated: 2025/11/11 13:53:57 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// scale vector v by number k
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