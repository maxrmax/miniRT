/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:21:17 by mring             #+#    #+#             */
/*   Updated: 2025/11/11 18:49:29 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	window_loop(void)
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

int	main(int ac, char **av)
{
	t_rt	*scene;

	scene = malloc(sizeof(t_rt));
	if (!scene)
		return (printf("Error: malloc failed\n"), 1);
	ft_memset(scene, 0, sizeof(t_rt));
	if (ac != 2)
	{
		printf("No file or to many arguments \n");
		free(scene);
		return (1);
	}
	if (parsing_scene(av[1], scene))
		{
			printf("Error: Failed to parse scene\n");
			free(scene);
			return (1);
		}
	window_loop();
	free(scene);
	return (0);
}
