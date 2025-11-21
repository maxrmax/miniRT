/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:21:17 by mring             #+#    #+#             */
/*   Updated: 2025/11/21 16:20:58 by jpflegha         ###   ########.fr       */
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
	/*
	now with scene prepped:
	the camera looks into a certain direction.
	lets say 1280x720 (16:9).
	for each pixel we send out a single ray.
	if that ray hits an object:
	is it blocked by another object or has it a clear path to the light source?
	yes (clear path): get the color and brightness
	no (blocked path): shadowed... get paths to different objects until light source?
	
	// shadow checking =//= ray bouncing
	
	conceptually:
	void window_loop(t_rt *scene)
{
    mlx_t *window = mlx_init(WIDTH, HEIGHT, "miniRT", false);
    mlx_image_t *img = mlx_new_image(window, WIDTH, HEIGHT);
    
    // Hardcoded for now (later use scene->camera)
    t_vec3 camera_pos = vec_new(0, 0, 0);
    
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            t_ray ray = get_camera_ray(i, j, camera_pos);
            t_color color = ray_color(ray, scene);
            write_color(img, i, j, color);
        }
    }
    
    mlx_image_to_window(window, img, 0, 0);
    mlx_loop(window);
    // cleanup...
}

// ----
Step-by-Step Example:
Your scenario: Camera → Plane → Sphere1 → Sphere2 → Sphere3 → Sphere4 → Light

Call Stack (depth = 5):
1. ray_color(camera_ray, depth=5)
   ↓ Hits plane
   
2. ray_color(reflected_off_plane, depth=4)
   ↓ Hits Sphere1
   
3. ray_color(reflected_off_sphere1, depth=3)
   ↓ Hits Sphere2
   
4. ray_color(reflected_off_sphere2, depth=2)
   ↓ Hits Sphere3
   
5. ray_color(reflected_off_sphere3, depth=1)
   ↓ Hits Sphere4
   
6. ray_color(reflected_off_sphere4, depth=0)
   ↓ depth limit reached, return BLACK
   (or check direct light)

Unwinding (backward color calculation):

   6. Sphere4: color = object_color * direct_light + BLACK (no more bounces)
   ↓ return to 5

5. Sphere3: color = object_color * (direct_light + reflectivity * color_from_sphere4)
   ↓ return to 4

4. Sphere2: color = object_color * (direct_light + reflectivity * color_from_sphere3)
   ↓ return to 3

3. Sphere1: color = object_color * (direct_light + reflectivity * color_from_sphere2)
   ↓ return to 2

2. Plane: color = object_color * (direct_light + reflectivity * color_from_sphere1)
   ↓ return to 1

1. Final pixel color!

The math at each bounce:
// Local illumination (direct light from light sources)
direct = 0;		// check type(s) -> if (type) vec_new(x, y, z);
		// then apply matrix: rotation, color, etc.
for each light:
    if (shadow_ray to light is clear):
        direct += light_contribution(normal, light_direction);

// Reflected illumination (from other objects via recursion)
reflected = ray_color(bounce_ray, depth - 1);

// Combine
final_color = material_color * (direct + material.reflectivity * reflected);
	*/

	/*
	for each pixel (i, j):
    // 1. Generate primary ray
    ray = camera_ray_for_pixel(i, j)
    
    // 2. Find closest object hit
    hit = find_closest_intersection(ray, scene->objects)
    
    if (no hit):
        color = background_color  // Sky
    else:
        // 3. Calculate surface properties
        hit_point = ray_at(ray, hit.distance)
        normal = calculate_normal(hit_point, hit.object)
        
        // 4. Check each light source
        for each light in scene->lights:
            // Cast shadow ray
            shadow_ray = ray_from_to(hit_point, light.position)
            
            if (shadow_ray hits another object):
                // In shadow from this light
                continue
            else:
                // Calculate lighting contribution
                brightness = dot(normal, light_direction)
                color += object_color * light_color * brightness
        
        // 5. Add ambient light (base illumination)
        color += ambient_color * object_color
    
    write_pixel(img, i, j, color)
scene
	TODO:
	Phase 1: Get something on screen
	Parse scene (you have this)
	Generate camera rays (next step!)
	Ray-sphere intersection (detect hits)
	Color based on hit/miss (red sphere, blue sky)
	Phase 2: Add lighting
	Calculate surface normals
	Simple shading (dot product with light direction)
	Shadow rays (check if light is blocked)
	Phase 3: Advanced (later)
	Multiple objects
	Reflections (ray bouncing)
	Anti-aliasing
	Planes, cylinders
	*/

	/*
	next step:
	void window_loop(t_rt *scene)
{
    mlx_t *window = mlx_init(WIDTH, HEIGHT, "miniRT", false);
    mlx_image_t *img = mlx_new_image(window, WIDTH, HEIGHT);
    
    // Hardcoded for now (later use scene->camera)
    t_vec3 camera_pos = vec_new(0, 0, 0);
    
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            t_ray ray = get_camera_ray(i, j, camera_pos);
            t_color color = ray_color(ray, scene);
            write_color(img, i, j, color);
        }
    }
    
    mlx_image_to_window(window, img, 0, 0);
    mlx_loop(window);
    // cleanup...
}
	*/
	(void)scene;
	printf("window_loop (not looping lol)\n");
	return ;
}
#include <stdio.h>

/* --- Helpers ------------------------------------------------------------ */

void print_vec3(const char *name, t_vec3 v)
{
    printf("%s: (%.1f, %.1f, %.1f)\n", name, v.x, v.y, v.z);
}

void print_color(const char *name, t_color c)
{
    printf("%s: (%d, %d, %d)\n", name, c.r, c.g, c.b);
}

/* --- Ambient ------------------------------------------------------------ */

void print_ambient(t_ambient *a)
{
    if (!a)
    {
        printf("Ambient: (null)\n");
        return;
    }

    printf("Ambient:\n");
    printf("  Ratio: %.1f\n", a->ratio);
    print_color("  Color", a->color);
    printf("\n");
}

/* --- Camera ------------------------------------------------------------- */

void print_camera(t_camera *cam)
{
    if (!cam)
    {
        printf("Camera: (null)\n");
        return;
    }

    printf("Camera:\n");
    print_vec3("  Position", cam->pos);
    print_vec3("  Direction", cam->dir);
    printf("  FOV: %d\n\n", cam->fov);
}

/* --- Light -------------------------------------------------------------- */

void print_light(t_light *l)
{
    if (!l)
    {
        printf("Light: (null)\n");
        return;
    }

    printf("Light:\n");
    print_color("  Color", l->color);
    print_vec3("  Position", l->pos);
    printf("  Brightness: %f\n\n", l->brightness);
}

/* --- Objects ------------------------------------------------------------ */

void print_objects(t_obj *obj)
{
    int i = 0;

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

void print_scene(t_rt *scene)
{
    printf("===== SCENE DATA =====\n\n");

    print_ambient(scene->ambient);
    print_camera(scene->camera);
    print_light(scene->light);
    print_objects(scene->objects);

    printf("===== END SCENE =====\n");
}



int validate_scene(t_rt *scene)
{
    int i;

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

	scene = malloc(sizeof(t_rt));
	if (!scene)
		return (printf("Error: malloc failed\n"), 1);
	
	if (ac != 2)
	{
		printf("No file or to many arguments \n");
		free_scenes(scene);
		return (1);
	}
	if (parsing_scene(av[1], scene))
			return (free_scenes(scene), 1);
	print_scene(scene);
	if(validate_scene(scene))
	{
		window_loop_test();
	}
	free_scenes(scene);
	return (0);
}
