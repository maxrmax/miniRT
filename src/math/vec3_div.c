/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_div.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 13:10:19 by mring             #+#    #+#             */
/*   Updated: 2025/12/11 09:53:48 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// divide vector by scalar
t_vec3	vec_div(t_vec3 v, float scalar)
{
	v.x /= scalar;
	v.y /= scalar;
	v.z /= scalar;
	return (v);
}
