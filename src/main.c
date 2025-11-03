/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:21:17 by mring             #+#    #+#             */
/*   Updated: 2025/11/03 21:07:54 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static inline t_vec3	vec3_zero(void)
{
	return (vec3_init(0.0, 0.0, 0.0));
}

static inline double	vec3_length_squared(const t_vec3 *v)
{
	return (v->e[0] * v->e[0] + v->e[1] * v->e[1] + v->e[2] * v->e[2]);
}

static inline double	vec3_length(const t_vec3 *v)
{
	return (sqrt(vec3_length_squared(v)));
}

static inline t_vec3	vec3_add(const t_vec3 *u, const t_vec3 *v)
{
	// return (vec3_init(u->e[0] + v->e[0], u->e[1] + v->e[1], u->e[2]
	// + v->e[2]));
	return ((t_vec3){.e = {u->e[0], u->e[1], u->e[2]}});
}

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
		lock mlx_terminate(window);
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

int	main(void)
{
	window_loop();
}
