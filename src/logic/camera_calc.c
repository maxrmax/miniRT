/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_calc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 12:50:18 by mring             #+#    #+#             */
/*   Updated: 2025/12/10 15:51:45 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// TODO: check ray_origin refactor
void	pre_calc_camera(t_rt *scene)
{
	// M_PI false error, math.h is included
	// fov calculation for our scene
	scene->camera->fov_rad = scene->camera->fov * M_PI / 180.0;
	// viewport is our screen inside the 3D space
	scene->camera->viewport_height = 2.0 * tan(scene->camera->fov_rad / 2.0);
	// Use actual image dimensions instead of constants
	scene->camera->viewport_width = scene->camera->viewport_height
		* ((double)scene->img->width / scene->img->height);
	scene->camera->forward = vec_normalize(scene->camera->dir);
	// x (left right), y (up down), z (forward backward)
	scene->camera->world_up = vec_new(0, 1, 0); // assuming +Y is up
	scene->camera->right = vec_normalize(vec_cross(scene->camera->forward,
				scene->camera->world_up));
	// already normalized
	scene->camera->up = vec_cross(scene->camera->right, scene->camera->forward);
}

// TODO: adjust index
// TODO: window resizing adjustment
void	calc_camera(t_rt *scene, int i, int j)
{
	double	viewport_x;
	double	viewport_y;
	t_vec3	viewport_offset;

	// Use actual image dimensions
	viewport_x = ((double)j / (scene->img->width - 1) * 2.0 - 1.0)
		* (scene->camera->viewport_width / 2.0);
	viewport_y = (1.0 - (double)i / (scene->img->height - 1) * 2.0)
		* (scene->camera->viewport_height / 2.0);
	viewport_offset = vec_add(vec_mult(scene->camera->right, viewport_x),
			vec_mult(scene->camera->up, viewport_y));
	scene->objects->ray_dir = vec_normalize(vec_add(scene->camera->forward,
				viewport_offset));
	// Use actual image width for index calculation
	scene->index = (i * scene->img->width + j) * 4;
}
