/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:54:10 by mring             #+#    #+#             */
/*   Updated: 2025/11/10 17:43:41 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC3_H
# define VEC3_H

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}			t_vec3;

t_vec3	vec_add(t_vec3 a, t_vec3 b);
t_vec3	vec_sub(t_vec3 a, t_vec3 b);
t_vec3	vec_cross(t_vec3 a, t_vec3 b);
float	vec_dot(t_vec3 a, t_vec3 b);
// vec_div
float	vec_length_squared(t_vec3 v);
float	vec_length(t_vec3 v);
t_vec3	vec_normalize(t_vec3 v);
t_vec3	vec_scale(t_vec3 v, double scalar);
// vec invert (vec_negate)
// vec_mult for color multiplications
// vec_new for init

#endif