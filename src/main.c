/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:21:17 by mring             #+#    #+#             */
/*   Updated: 2025/12/03 13:36:55 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	window_loop(t_rt *scene)
{
	mlx_t		*window;
	mlx_image_t	*img;
	uint32_t	index;
	t_obj		*obj;
	t_vec3		hit_point;
	t_vec3		normal;
	t_vec3		light_dir;
	double		light_dist;
	bool		in_shadow;
	double		shadow_t;
	double		diffuse;
	double		brightness;
	t_vec3		hit_to_center;
	double		proj;
	t_vec3		axis_point;
	int			i;
	int			j;

	scene->hit_obj = NULL;
	ray_origin = scene->camera->pos;
	// calculate render size by window size
	window = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!window)
		exit(1);
	img = mlx_new_image(window, WIDTH, HEIGHT);
	if (!img)
	{
		mlx_terminate(window);
		exit(1);
	}
	pre_calc_camera(scene->camera);
	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			calc_camera(scene->camera);
			obj_calc(scene->objects);
			calc_pixel(scene);
			j++;
		}
		i++;
	}
	mlx_image_to_window(window, img, 0, 0);
	mlx_loop(window);
	mlx_delete_image(window, img);
	mlx_terminate(window);
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
	scene = malloc(sizeof(t_rt));
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
