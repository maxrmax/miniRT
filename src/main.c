/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:21:17 by mring             #+#    #+#             */
/*   Updated: 2025/12/02 09:09:30 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	window_loop(t_rt *scene)
{
	mlx_t		*window;
	mlx_image_t	*img;
	double		viewport_x;
	double		viewport_y;
	uint32_t	index;
	t_vec3		ray_origin;
	t_vec3		ray_dir;
	t_obj		*obj;
	bool		hit;
	t_obj		*hit_obj;
	t_vec3		forward;
	t_vec3		right;
	t_vec3		world_up;
	t_vec3		up;
	t_vec3		viewport_offset;
	double		fov_rad;
	double		viewport_height;
	double		viewport_width;
	double		t;
	double		closest_t;
	t_vec3		hit_point;
	t_vec3		normal;
	t_vec3		light_dir;
	double		light_dist;
	t_obj		*shadow_obj;
	bool		in_shadow;
	double		shadow_t;
	double		diffuse;
	double		brightness;
	t_vec3		hit_to_center;
	double		proj;
	t_vec3		axis_point;
	int			i;
	int			j;

	// what can i say?
	// i like refactoring
	hit_obj = NULL;
	ray_origin = scene->camera->pos;
	window = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!window)
		exit(1);
	img = mlx_new_image(window, WIDTH, HEIGHT);
	if (!img)
	{
		mlx_terminate(window);
		exit(1);
	}
	// false error, math.h is included
	// fov calculation for our scene
	fov_rad = scene->camera->fov * M_PI / 180.0;
	// viewport is our screen inside the 3D space
	viewport_height = 2.0 * tan(fov_rad / 2.0);
	// might need to adjust for resizable windows
	viewport_width = viewport_height * ((double)WIDTH / HEIGHT);
	forward = vec_normalize(scene->camera->dir);
	// x (left right), y (up down), z (forward backward)
	world_up = vec_new(0, 1, 0); // assuming +Y is up
	right = vec_normalize(vec_cross(forward, world_up));
	up = vec_cross(right, forward); // already normalized
	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			closest_t = INFINITY;
			viewport_x = ((double)j / (WIDTH - 1) * 2.0 - 1.0) * (viewport_width
					/ 2.0);
			viewport_y = (1.0 - (double)i / (HEIGHT - 1) * 2.0)
				* (viewport_height / 2.0);
			viewport_offset = vec_add(vec_mult(right, viewport_x), vec_mult(up,
						viewport_y));
			ray_dir = vec_normalize(vec_add(forward, viewport_offset));
			index = (i * WIDTH + j) * 4;
			obj = scene->objects;
			hit = false;
			while (obj)
			{
				t = -1.0;
				// TODO: adjust arguments, pass entire object instead
				// reduces clutter, keeps logic inside sub-functions
				if (obj->type == SPHERE)
					t = hit_sphere(ray_origin, ray_dir, obj->data.sp.center,
							obj->data.sp.diameter / 2.0);
				else if (obj->type == PLANE)
					t = hit_plane(ray_origin, ray_dir, obj->data.pl.point,
							obj->data.pl.normal);
				else if (obj->type == CYLINDER)
					t = hit_cylinder(ray_origin, ray_dir, obj->data.cy.center,
							obj->data.cy.axis, obj->data.cy.diameter / 2.0,
							obj->data.cy.height);
				// if t is above 0 and smaller than closest_t (iirc t was time in math)
				if (t > 0 && t < closest_t)
				{
					closest_t = t;
					hit_obj = obj;
					hit = true;
				}
				obj = obj->next;
			}
			// ayy, we hit something, time to do more math!
			if (hit)
			{
				// Calculating actual hit point
				hit_point = vec_add(ray_origin, vec_mult(ray_dir, closest_t));
				// Calculating surface normal based on object type
				if (hit_obj->type == SPHERE)
					normal = vec_normalize(vec_sub(hit_point,
								hit_obj->data.sp.center));
				else if (hit_obj->type == PLANE)
					normal = hit_obj->data.pl.normal;
				// cylinders are a bitch
				else if (hit_obj->type == CYLINDER)
				{
					hit_to_center = vec_sub(hit_point, hit_obj->data.cy.center);
					proj = vec_dot(hit_to_center, hit_obj->data.cy.axis);
					// Bottom cap: proj near 0
					if (proj < 0.001)
						normal = vec_negate(hit_obj->data.cy.axis);
					// Top cap: proj near height
					else if (proj > hit_obj->data.cy.height - 0.001)
						normal = hit_obj->data.cy.axis;
					// Body
					else
					{
						axis_point = vec_add(hit_obj->data.cy.center,
								vec_mult(hit_obj->data.cy.axis, proj));
						normal = vec_normalize(vec_sub(hit_point, axis_point));
					}
				}
				// there shall be light
				// Light direction and distance
				light_dir = vec_sub(scene->light->pos, hit_point);
				light_dist = vec_length(light_dir);
				light_dir = vec_normalize(light_dir);
				// and there shall be shadows
				// Shadow ray: check if path to light is blocked
				shadow_obj = scene->objects;
				in_shadow = false;
				while (shadow_obj)
				{
					shadow_t = -1.0;
					if (shadow_obj->type == SPHERE)
						shadow_t = hit_sphere(hit_point, light_dir,
								shadow_obj->data.sp.center,
								shadow_obj->data.sp.diameter / 2.0);
					else if (shadow_obj->type == PLANE)
						shadow_t = hit_plane(hit_point, light_dir,
								shadow_obj->data.pl.point,
								shadow_obj->data.pl.normal);
					else if (shadow_obj->type == CYLINDER)
						shadow_t = hit_cylinder(hit_point, light_dir,
								shadow_obj->data.cy.center,
								shadow_obj->data.cy.axis,
								shadow_obj->data.cy.diameter / 2.0,
								shadow_obj->data.cy.height);
					// 0.001 to avoid self-intersection
					if (shadow_t > 0.001 && shadow_t < light_dist)
					{
						in_shadow = true;
						break ;
					}
					shadow_obj = shadow_obj->next;
				}
				// calculating lighting
				diffuse = fmax(0.0, vec_dot(normal, light_dir));
				if (in_shadow)
					diffuse = 0.0;
				brightness = scene->ambient->ratio + (scene->light->brightness
						* diffuse);
				brightness = fmin(brightness, 1.0);
				// and finally: there shall be colors to enrichen your vision
				if (hit_obj->type == SPHERE)
				{
					img->pixels[index + 0] = (int)(hit_obj->data.sp.color.r
							* brightness);
					img->pixels[index + 1] = (int)(hit_obj->data.sp.color.g
							* brightness);
					img->pixels[index + 2] = (int)(hit_obj->data.sp.color.b
							* brightness);
				}
				// repeat for PLANE and CYLINDER types
				else if (hit_obj->type == PLANE)
				{
					img->pixels[index + 0] = (int)(hit_obj->data.pl.color.r
							* brightness);
					img->pixels[index + 1] = (int)(hit_obj->data.pl.color.g
							* brightness);
					img->pixels[index + 2] = (int)(hit_obj->data.pl.color.b
							* brightness);
				}
				else if (hit_obj->type == CYLINDER)
				{
					img->pixels[index + 0] = (int)(hit_obj->data.cy.color.r
							* brightness);
					img->pixels[index + 1] = (int)(hit_obj->data.cy.color.g
							* brightness);
					img->pixels[index + 2] = (int)(hit_obj->data.cy.color.b
							* brightness);
				}
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
