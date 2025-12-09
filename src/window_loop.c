/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:54:36 by mring             #+#    #+#             */
/*   Updated: 2025/12/09 17:57:19 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	key_hook(mlx_key_data_t keydata, void *param)
{
	mlx_t	*mlx;

	mlx = param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(mlx);
}

static void	render_pixel(t_rt *scene, int i, int j)
{
	calc_camera(scene, i, j);
	calc_objs(scene);
	calc_pixel(scene);
}

static void	render_scene(t_rt *scene)
{
	int	i;
	int	j;

	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			render_pixel(scene, i, j);
			j++;
		}
		i++;
	}
}

static void	init_window(t_rt *scene)
{
	scene->window = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!scene->window)
		exit(1);
	scene->img = mlx_new_image(scene->window, WIDTH, HEIGHT);
	if (!scene->img)
	{
		mlx_terminate(scene->window);
		exit(1);
	}
}

void	window_loop(t_rt *scene)
{
	scene->hit_obj = NULL;
	init_window(scene);
	pre_calc_camera(scene);
	render_scene(scene);
	mlx_image_to_window(scene->window, scene->img, 0, 0);
	mlx_key_hook(scene->window, key_hook, scene->window);
	mlx_loop(scene->window);
	mlx_delete_image(scene->window, scene->img);
	mlx_terminate(scene->window);
}
