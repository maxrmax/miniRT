/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_sub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:28:32 by mring             #+#    #+#             */
/*   Updated: 2025/11/10 17:43:51 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"


// returns the sum of vector a and b
// passed by value, original values are not modified
t_vec3	vec_sub(t_vec3 a, t_vec3 b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return (a);
}

// alternative way to write it with a temporary struct
// compound literal
// return ((t_vec3){a.x + b.x, a.y + b.y, a.z + b.z});