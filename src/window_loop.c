/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:54:36 by mring             #+#    #+#             */
/*   Updated: 2025/12/11 09:06:22 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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

static void	render_pixel(t_rt *scene, int i, int j)
{
	calc_camera(scene, i, j);
	calc_objs(scene);
	calc_pixel(scene);
}

void	render_scene(t_rt *scene)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	j = 0;
	while (i < scene->img->height)
	{
		while (j < scene->img->width)
		{
			render_pixel(scene, i, j);
			j++;
		}
		j = 0;
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
	scene->needs_resize = false;
	scene->pending_width = WIDTH;
	scene->pending_height = HEIGHT;
	init_window(scene);
	pre_calc_camera(scene);
	render_scene(scene);
	mlx_image_to_window(scene->window, scene->img, 0, 0);
	mlx_key_hook(scene->window, key_hook, scene->window);
	mlx_resize_hook(scene->window, resize_hook, scene);
	mlx_loop_hook(scene->window, loop_hook, scene);
	mlx_loop(scene->window);
	mlx_delete_image(scene->window, scene->img);
	mlx_terminate(scene->window);
}
