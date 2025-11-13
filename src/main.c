/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:21:17 by mring             #+#    #+#             */
/*   Updated: 2025/11/13 20:42:23 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	window_loop_test(void)
{
	mlx_t		*window;
	mlx_image_t	*img;
	int			i;
	int			j;
	double		r;
	double		g;
	int			ir;
	int			ig;
	uint32_t	index;

	window = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!window)
		exit(1);
	img = mlx_new_image(window, WIDTH, HEIGHT);
	if (!img)
	{
		mlx_terminate(window);
		exit(1);
	}
	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			r = (double)j / (WIDTH - 1);
			g = (double)i / (HEIGHT - 1);
			ir = (int)(255.999 * r);
			ig = (int)(255.999 * g);
			index = (i * WIDTH + j) * 4;
			img->pixels[index + 0] = ir;
			img->pixels[index + 1] = ig;
			img->pixels[index + 2] = 0;
			img->pixels[index + 3] = 255;
			j++;
		}
		i++;
	}
	mlx_image_to_window(window, img, 0, 0);
	mlx_loop(window);
	mlx_delete_image(window, img);
	mlx_terminate(window);
}

void	window_loop(t_rt *scene)
{
	// with vector and matrix set the prop into position.
	/*
	detect object type
	draw_scene(scene); // figure out the types we have.
	if 
	if (scene->)
	*/
	vec_new(x, y, z);
}

int	main(int ac, char **av)
{
	t_rt	*scene;

	
	if (ac != 2)
	return (printf("No file or to many arguments \n"), 1);
	scene = malloc(sizeof(t_rt));
	if (!scene)
		return (printf("Error: malloc failed\n"), 1);
	// warum? brauchen doch nicht memory zu nullen
	// ft_memset(scene, 0, sizeof(t_rt));
	if (parsing_scene(av[1], scene))
	{
		printf("Error: Failed to parse scene\n");
		return (free_scenes(scene), 1);
	}
	if (!validate_scene(scene))
	{
		printf("Scene validation failed. Cannot render.\n");
		return (free_scenes(scene), 1);
	}
	else
		printf("Scene is valid! Ready to render.\n");
	window_loop(scene);
	free_scenes(scene);
	return (0);
}
