/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:21:17 by mring             #+#    #+#             */
/*   Updated: 2025/11/25 11:23:01 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	window_loop_test(t_rt *scene)
{
	mlx_t		*window;
	mlx_image_t	*img;
	double		viewport_x;
	double		viewport_y;
	uint32_t	index;
	t_vec3		ray_direction;
	t_vec3		ray_origin;
	t_vec3		ray_dir;
	int			ib;
	t_obj		*obj;
	bool		hit;
	t_obj		*hit_obj;

	hit_obj = NULL;
	ray_origin = scene->camera->pos;
	int i; // convert to u
	int j; // and v for 3d space viewport coords
	window = mlx_init(WIDTH, HEIGHT, "miniRT", true);
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
			viewport_x = (double)j / (WIDTH - 1) * 2.0 - 1.0;
			viewport_y = 1.0 - (double)i / (HEIGHT - 1) * 2.0;
			ray_dir = vec_normalize(vec_new(viewport_x, viewport_y, 1.0));
			ib = (int)(255.999 * ray_dir.z);
			index = (i * WIDTH + j) * 4;
			obj = scene->objects;
			hit = false;
			while (obj)
			{
				if (obj->type == SPHERE)
				{
					if (hit_sphere(ray_origin, ray_dir, obj->data.sp.center,
							obj->data.sp.diameter / 2.0))
					{
						hit = true;
						hit_obj = obj;
						break ;
					}
				}
				obj = obj->next;
			}
			if (hit)
			{
				img->pixels[index + 0] = obj->data.sp.color.r;
				img->pixels[index + 1] = obj->data.sp.color.g;
				img->pixels[index + 2] = obj->data.sp.color.b;
			}
			else
			{
				img->pixels[index + 0] = 0;
				img->pixels[index + 1] = 0;
				img->pixels[index + 2] = 0;
			}
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

#include <stdio.h>

/* --- Helpers ------------------------------------------------------------ */

void	print_vec3(const char *name, t_vec3 v)
{
	printf("%s: (%.1f, %.1f, %.1f)\n", name, v.x, v.y, v.z);
}

void	print_color(const char *name, t_color c)
{
	printf("%s: (%d, %d, %d)\n", name, c.r, c.g, c.b);
}

/* --- Ambient ------------------------------------------------------------ */

void	print_ambient(t_ambient *a)
{
	if (!a)
	{
		printf("Ambient: (null)\n");
		return ;
	}
	printf("Ambient:\n");
	printf("  Ratio: %.1f\n", a->ratio);
	print_color("  Color", a->color);
	printf("\n");
}

/* --- Camera ------------------------------------------------------------- */

void	print_camera(t_camera *cam)
{
	if (!cam)
	{
		printf("Camera: (null)\n");
		return ;
	}
	printf("Camera:\n");
	print_vec3("  Position", cam->pos);
	print_vec3("  Direction", cam->dir);
	printf("  FOV: %d\n\n", cam->fov);
}

/* --- Light -------------------------------------------------------------- */

void	print_light(t_light *l)
{
	if (!l)
	{
		printf("Light: (null)\n");
		return ;
	}
	printf("Light:\n");
	print_color("  Color", l->color);
	print_vec3("  Position", l->pos);
	printf("  Brightness: %f\n\n", l->brightness);
}

/* --- Objects ------------------------------------------------------------ */

void	print_objects(t_obj *obj)
{
	int	i;

	i = 0;
	while (obj)
	{
		printf("Object %d:\n", i);
		if (obj->type == SPHERE)
		{
			printf("  Type: SPHERE\n");
			print_color("  Color", obj->data.sp.color);
			print_vec3("  Center", obj->data.sp.center);
			printf("  Diameter: %f\n", obj->data.sp.diameter);
		}
		else if (obj->type == PLANE)
		{
			printf("  Type: PLANE\n");
			print_vec3("  Point", obj->data.pl.point);
			print_vec3("  Normal", obj->data.pl.normal);
			print_color("  Color", obj->data.pl.color);
		}
		else if (obj->type == CYLINDER)
		{
			printf("  Type: CYLINDER\n");
			print_vec3("  Center", obj->data.cy.center);
			print_vec3("  Axis", obj->data.cy.axis);
			printf("  Diameter: %f\n", obj->data.cy.diameter);
			printf("  Height: %f\n", obj->data.cy.height);
			print_color("  Color", obj->data.cy.color);
		}
		printf("\n");
		obj = obj->next;
		i++;
	}
}

/* --- Full Scene Print --------------------------------------------------- */

void	print_scene(t_rt *scene)
{
	printf("===== SCENE DATA =====\n\n");
	print_ambient(scene->ambient);
	print_camera(scene->camera);
	print_light(scene->light);
	print_objects(scene->objects);
	printf("===== END SCENE =====\n");
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
		free_scenes(scene);
		return (1);
	}
	scene = malloc(sizeof(t_rt));
	if (!scene)
		return (printf("Error: malloc failed\n"), 1);
	if (parsing_scene(av[1], scene))
		return (free_scenes(scene), 1);
	// render(scene->camera);
	print_scene(scene);
	if (validate_scene(scene))
	{
		window_loop_test(scene);
	}
	free_scenes(scene);
	return (0);
}
