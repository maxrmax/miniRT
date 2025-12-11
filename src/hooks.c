/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:00:51 by jpflegha          #+#    #+#             */
/*   Updated: 2025/12/11 09:32:39 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	resize_hook(int32_t width, int32_t height, void *param)
{
	t_rt	*scene;

	scene = param;
	if (width <= 0 || height <= 0)
		return ;
	scene->pending_width = width;
	scene->pending_height = height;
	scene->needs_resize = true;
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	mlx_t	*mlx;

	mlx = param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(mlx);
}

void	loop_hook(void *param)
{
	t_rt	*scene;

	scene = param;
	if (scene->needs_resize)
	{
		scene->needs_resize = false;
		mlx_delete_image(scene->window, scene->img);
		scene->img = mlx_new_image(scene->window, scene->pending_width,
				scene->pending_height);
		if (!scene->img)
		{
			mlx_terminate(scene->window);
			exit(1);
		}
		pre_calc_camera(scene);
		render_scene(scene);
		mlx_image_to_window(scene->window, scene->img, 0, 0);
	}
}
