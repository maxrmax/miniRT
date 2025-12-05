/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_calc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 12:50:18 by mring             #+#    #+#             */
/*   Updated: 2025/12/05 14:33:40 by mring            ###   ########.fr       */
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
	// might need to adjust for resizable windows
	scene->camera->viewport_width = scene->camera->viewport_height
		* ((double)WIDTH / HEIGHT);
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
	scene->camera->viewport_x = ((double)j / (WIDTH - 1) * 2.0 - 1.0)
		* (scene->camera->viewport_width / 2.0);
	scene->camera->viewport_y = (1.0 - (double)i / (HEIGHT - 1) * 2.0)
		* (scene->camera->viewport_height / 2.0);
	scene->camera->viewport_offset = vec_add(vec_mult(scene->camera->right,
				scene->camera->viewport_x), vec_mult(scene->camera->up,
				scene->camera->viewport_y));
	scene->objects->ray_dir = vec_normalize(vec_add(scene->camera->forward,
				scene->camera->viewport_offset));
	scene->index = (i * WIDTH + j) * 4;
}
