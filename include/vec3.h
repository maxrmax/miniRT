/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:54:10 by mring             #+#    #+#             */
/*   Updated: 2025/11/12 14:36:53 by mring            ###   ########.fr       */
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
t_vec3	vec_new(float x, float y, float z);	// Create new vector

// Basic arithmetic
t_vec3	vec_add(t_vec3 a, t_vec3 b);		// add two vectors
t_vec3	vec_sub(t_vec3 a, t_vec3 b);		// sub two vectors
t_vec3	vec_negate(t_vec3 v);				// (-v.x, -v.y, -v.z)
t_vec3	vec_mult_vec(t_vec3 a, t_vec3 b);	// mult two vectors

// Scalar operations
t_vec3	vec_mult(t_vec3 v, float scalar);	// scale v
t_vec3	vec_div(t_vec3 v, float scalar);	// scale v

// vector operations
float	vec_dot(t_vec3 a, t_vec3 b);		// check alignment
t_vec3	vec_cross(t_vec3 a, t_vec3 b);		// new v, 90d to a and b
float	vec_length(t_vec3 v);				// v length
float	vec_length_squared(t_vec3 v);		// v sq len
t_vec3	vec_normalize(t_vec3 v);			// get v length 1

// debug print
void	vec_print(t_vec3 v);

#endif