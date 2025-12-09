/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:21:17 by mring             #+#    #+#             */
/*   Updated: 2025/12/09 14:41:23 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	key_hook(mlx_key_data_t keydata, void *param)
{
	mlx_t	*mlx;

	mlx = param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(mlx);
}

void	window_loop(t_rt *scene)
{
	int	i;
	int	j;

	scene->hit_obj = NULL;
	// calculate render size by window size
	scene->window = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!scene->window)
		exit(1);
	scene->img = mlx_new_image(scene->window, WIDTH, HEIGHT);
	if (!scene->img)
	{
		mlx_terminate(scene->window);
		exit(1);
	}
	pre_calc_camera(scene);
	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			calc_camera(scene, i, j);
			// -> done, no dependencies elsewhere
			calc_objs(scene);
			// -> hit_sphere/plane/cylinder
			calc_pixel(scene);
			// -> if (hit) hit_calc; light_calc; draw_pixel;
			// else pixel black
			j++;
		}
		i++;
	}
	// key_hook(/*idk yet*/);
	mlx_image_to_window(scene->window, scene->img, 0, 0);
	mlx_loop(scene->window);
	mlx_delete_image(scene->window, scene->img);
	mlx_terminate(scene->window);
}

int	validate_scene(t_rt *scene)
{
	int	i;

	i = 0;
	if (!scene->ambient)
		i = printf("Error: Missing ambient\n");
	if (!scene->camera)
		i = printf("Error: Missing camera\n");
	if (!scene->light)
		i = printf("Error: Missing light\n");
	if (i > 0)
		return (0);
	return (1);
}

int	main(int ac, char **av)
{
	t_rt	*scene;

	if (ac != 2)
	{
		printf("No file or to many arguments \n");
		return (1);
	}
	scene = ft_calloc(1, sizeof(t_rt));
	if (!scene)
		return (printf("Error: malloc failed\n"), 1);
	if (parsing_scene(av[1], scene))
		return (free_scenes(scene), 1);
	print_scene(scene); // debug
	if (validate_scene(scene))
		window_loop(scene);
	free_scenes(scene);
	return (0);
}
