/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_calc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 12:50:18 by mring             #+#    #+#             */
/*   Updated: 2025/12/11 09:34:37 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	pre_calc_camera(t_rt *scene)
{
	scene->camera->fov_rad = scene->camera->fov * M_PI / 180.0;
	scene->camera->viewport_height = 2.0 * tan(scene->camera->fov_rad / 2.0);
	scene->camera->viewport_width = scene->camera->viewport_height
		* ((double)scene->img->width / scene->img->height);
	scene->camera->forward = vec_normalize(scene->camera->dir);
	scene->camera->world_up = vec_new(0, 1, 0);
	scene->camera->right = vec_normalize(vec_cross(scene->camera->forward,
				scene->camera->world_up));
	scene->camera->up = vec_cross(scene->camera->right, scene->camera->forward);
}

void	calc_camera(t_rt *scene, int i, int j)
{
	double	viewport_x;
	double	viewport_y;
	t_vec3	viewport_offset;

	viewport_x = ((double)j / (scene->img->width - 1) * 2.0 - 1.0)
		* (scene->camera->viewport_width / 2.0);
	viewport_y = (1.0 - (double)i / (scene->img->height - 1) * 2.0)
		* (scene->camera->viewport_height / 2.0);
	viewport_offset = vec_add(vec_mult(scene->camera->right, viewport_x),
			vec_mult(scene->camera->up, viewport_y));
	scene->objects->ray_dir = vec_normalize(vec_add(scene->camera->forward,
				viewport_offset));
	scene->index = (i * scene->img->width + j) * 4;
}
