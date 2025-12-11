/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 15:11:42 by mring             #+#    #+#             */
/*   Updated: 2025/12/11 09:46:52 by mring            ###   ########.fr       */
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

//
int		validate_scene(t_rt *scene);
void	free_scenes(t_rt *scene);
void	window_loop(t_rt *scene);
void	render_scene(t_rt *scene);
void	resize_hook(int32_t width, int32_t height, void *param);
void	key_hook(mlx_key_data_t keydata, void *param);
void	loop_hook(void *param);

// camera
void	pre_calc_camera(t_rt *scene);
void	calc_camera(t_rt *scene, int i, int j);

// object calculations
void	calc_objs(t_rt *scene);
double	hit_sphere(t_obj *obj);
double	hit_plane(t_obj *obj);
double	hit_cylinder(t_obj *obj);
double	cy_base(t_obj *obj);

// ray calculations (light, shadow)
void	hit_calc(t_obj *obj);
void	light_calc(t_rt *scene);

// pixel calculations
void	calc_pixel(t_rt *scene);
void	draw_pixel(t_rt *scene);

#endif