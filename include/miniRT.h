/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 15:11:42 by mring             #+#    #+#             */
/*   Updated: 2025/12/01 13:32:09 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "libft.h"
# include "parsing.h"
# include "vec3.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define WIDTH 860
# define HEIGHT 640

char	**ft_split_whitespace(char *str);
int		count_array_elements(char **arr);
void	free_scenes(t_rt *scene);
int		validate_scene(t_rt *scene);

double	hit_sphere(t_vec3 ray_origin, t_vec3 ray_dir, t_vec3 sphere_center,
			double radius);
double	hit_plane(t_vec3 ray_origin, t_vec3 ray_dir, t_vec3 plane_point,
			t_vec3 plane_normal);
double	hit_cylinder(t_vec3 ray_origin, t_vec3 ray_dir, t_vec3 cy_center,
			t_vec3 cy_axis, double radius, double height);

#endif