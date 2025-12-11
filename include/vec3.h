/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:54:10 by mring             #+#    #+#             */
/*   Updated: 2025/12/11 09:58:49 by mring            ###   ########.fr       */
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

// Constructor
t_vec3		vec_new(float x, float y, float z);

// Basic arithmetic
t_vec3		vec_add(t_vec3 a, t_vec3 b);
t_vec3		vec_sub(t_vec3 a, t_vec3 b);
t_vec3		vec_negate(t_vec3 v);
t_vec3		vec_mult_vec(t_vec3 a, t_vec3 b);

// Scalar operations
t_vec3		vec_mult(t_vec3 v, float scalar);
t_vec3		vec_div(t_vec3 v, float scalar);

// vector operations
float		vec_dot(t_vec3 a, t_vec3 b);
t_vec3		vec_cross(t_vec3 a, t_vec3 b);
float		vec_length(t_vec3 v);
t_vec3		vec_normalize(t_vec3 v);

#endif