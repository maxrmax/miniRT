/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:34:36 by mring             #+#    #+#             */
/*   Updated: 2025/12/03 13:36:59 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double	hit_sphere(t_vec3 ray_origin, t_vec3 ray_dir, t_obj *obj)
{
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t;

	// calcuate a, b, c
	// calculate discriminant
	// return true if dscriminant >= 0, else false
	a = vec_dot(ray_dir, ray_dir);
	b = 2.0 * vec_dot(ray_dir, vec_sub(ray_origin, obj->data.sp.center));
	c = vec_dot(vec_sub(ray_origin, obj->data.sp.center), vec_sub(ray_origin,
				obj->data.sp.center)) - obj->data.sp.diameter
		* obj->data.sp.diameter / 4;
	discriminant = b * b - 4.0 * a * c;
	if (discriminant < 0)
		return (-1.0);
	t = (-b - sqrt(discriminant)) / (2.0 * a);
	if (t < 0)
		return (-1.0);
	return (t);
}

// t = dot(plane_point - ray_origin, plane_normal) / dot(ray_dir, plane_normal)
double	hit_plane(t_vec3 ray_origin, t_vec3 ray_dir, t_obj *obj)
{
	double	denom;
	double	t;

	denom = vec_dot(ray_dir, obj->data.pl.normal);
	if (fabs(denom) < 1e-6) // parallel
		return (-1.0);
	t = vec_dot(vec_sub(obj->data.pl.point, ray_origin), obj->data.pl.normal)
		/ denom;
	if (t < 0)
		return (-1.0);
	return (t);
}

double	hit_cylinder(t_vec3 ray_origin, t_vec3 ray_dir, t_obj *obj)
{
	t_vec3	oc;
	t_vec3	ray_perp;
	t_vec3	oc_perp;
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t1;
	double	t2;
	double	t;
	t_vec3	p1;
	double	h1;
	t_vec3	p2;
	double	h2;
	double	cap_denom;
	double	t_bottom;
	t_vec3	p_bottom;
	t_vec3	to_center;
	t_vec3	top_center;
	double	t_top;
	t_vec3	p_top;
	t_vec3	to_top_center;
	t_vec3	radial;
	t_vec3	radial_top;

	// Vector from ray origin to cylinder base
	oc = vec_sub(ray_origin, obj->data.cy.center);
	// Project ray direction and oc onto plane perpendicular to cylinder axis
	ray_perp = vec_sub(ray_dir, vec_mult(obj->data.cy.axis, vec_dot(ray_dir,
					obj->data.cy.axis)));
	oc_perp = vec_sub(oc, vec_mult(obj->data.cy.axis, vec_dot(oc,
					obj->data.cy.axis)));
	a = vec_dot(ray_perp, ray_perp);
	b = 2.0 * vec_dot(ray_perp, oc_perp);
	c = vec_dot(oc_perp, oc_perp) - obj->data.cy.diameter
		* obj->data.cy.diameter / 4;
	discriminant = b * b - 4.0 * a * c;
	if (discriminant < 0)
		return (-1.0);
	t1 = (-b - sqrt(discriminant)) / (2.0 * a);
	t2 = (-b + sqrt(discriminant)) / (2.0 * a);
	// Check both intersection points against height bounds
	t = -1.0;
	p1 = vec_add(ray_origin, vec_mult(ray_dir, t1));
	h1 = vec_dot(vec_sub(p1, obj->data.cy.center), obj->data.cy.axis);
	if (t1 > 0 && h1 >= 0 && h1 <= obj->data.cy.height)
		t = t1;
	else
	{
		p2 = vec_add(ray_origin, vec_mult(ray_dir, t2));
		h2 = vec_dot(vec_sub(p2, obj->data.cy.center), obj->data.cy.axis);
		if (t2 > 0 && h2 >= 0 && h2 <= obj->data.cy.height)
			t = t2;
	}
	cap_denom = vec_dot(ray_dir, obj->data.cy.axis);
	if (fabs(cap_denom) > 1e-6)
	{
		// Bottom cap
		t_bottom = vec_dot(vec_sub(obj->data.cy.center, ray_origin),
				obj->data.cy.axis) / cap_denom;
		if (t_bottom > 0)
		{
			p_bottom = vec_add(ray_origin, vec_mult(ray_dir, t_bottom));
			to_center = vec_sub(p_bottom, obj->data.cy.center);
			radial = vec_sub(to_center, vec_mult(obj->data.cy.axis,
						vec_dot(to_center, obj->data.cy.axis)));
			if (vec_dot(radial, radial) <= obj->data.cy.diameter
				* obj->data.cy.diameter / 4)
			{
				if (t < 0 || t_bottom < t)
					t = t_bottom;
			}
		}
		// Top cap
		top_center = vec_add(obj->data.cy.center, vec_mult(obj->data.cy.axis,
					obj->data.cy.height));
		t_top = vec_dot(vec_sub(top_center, ray_origin), obj->data.cy.axis)
			/ cap_denom;
		if (t_top > 0)
		{
			p_top = vec_add(ray_origin, vec_mult(ray_dir, t_top));
			to_top_center = vec_sub(p_top, top_center);
			radial_top = vec_sub(to_top_center, vec_mult(obj->data.cy.axis,
						vec_dot(to_top_center, obj->data.cy.axis)));
			if (vec_dot(radial_top, radial_top) <= obj->data.cy.diameter
				* obj->data.cy.diameter / 4)
			{
				if (t < 0 || t_top < t)
					t = t_top;
			}
		}
	}
	return (t);
}

// refactor start
void	pre_calc_camera(t_camera *camera)
{
	// false error, math.h is included
	// fov calculation for our scene
	camera->fov_rad = camera->fov * M_PI / 180.0;
	// viewport is our screen inside the 3D space
	camera->viewport_height = 2.0 * tan(camera->fov_rad / 2.0);
	// might need to adjust for resizable windows
	camera->viewport_width = camera->viewport_height * ((double)WIDTH / HEIGHT);
	camera->forward = vec_normalize(camera->dir);
	// x (left right), y (up down), z (forward backward)
	camera->world_up = vec_new(0, 1, 0); // assuming +Y is up
	camera->right = vec_normalize(vec_cross(camera->forward, camera->world_up));
	// already normalized
	camera->up = vec_cross(camera->right, camera->forward);
}

void	calc_camera(t_camera *camera)
{
	camera->viewport_x = ((double)j / (WIDTH - 1) * 2.0 - 1.0)
		* (camera->viewport_width / 2.0);
	camera->viewport_y = (1.0 - (double)i / (HEIGHT - 1) * 2.0)
		* (camera->viewport_height / 2.0);
	camera->viewport_offset = vec_add(vec_mult(camera->right,
				camera->viewport_x), vec_mult(camera->up, camera->viewport_y));
	camera->ray_dir = vec_normalize(vec_add(camera->forward,
				camera->viewport_offset));
	index = (i * WIDTH + j) * 4;
}

void	calc_pixel(t_rt *scene)
{
	if (scene->objects->hit)
	{
		hit_calc();
		light_calc();
		draw_pixel(scene);
	}
	else
	{
		img->pixels[index + 0] = 0;
		img->pixels[index + 1] = 0;
		img->pixels[index + 2] = 0;
	}
	img->pixels[index + 3] = 255;
}

void	obj_calc(t_obj *objects)
{
	t_obj	*obj;
	double	t;

	obj = objects;
	obj->hit = false;
	while (obj)
	{
		t = -1.0;
		// TODO: adjust arguments, pass entire object instead
		// reduces clutter, keeps logic inside sub-functions
		if (obj->type == SPHERE)
			obj->t = hit_sphere(obj->ray_origin, obj->ray_dir, obj);
		else if (obj->type == PLANE)
			obj->t = hit_plane(obj->ray_origin, obj->ray_dir, obj);
		else if (obj->type == CYLINDER)
			obj->t = hit_cylinder(obj->ray_origin, obj->ray_dir, obj);
		// if t is above 0 and smaller than closest_t (iirc t was time in math)
		if (obj->t > 0 && obj->t < obj->closest_t)
		{
			obj->closest_t = obj->t;
			scene->hit_obj = obj; // may can move hit_obj into t_obj
			obj->hit = true;
		}
		obj = obj->next;
	}
}

void	hit_calc(void)
{
	hit_point = vec_add(ray_origin, vec_mult(ray_dir, closest_t));
	// Calculating surface normal based on object type
	// obj->type == SPHERE && obj->hit == true ?
	if (scene->hit_obj->type == SPHERE)
		normal = vec_normalize(vec_sub(hit_point,
					scene->hit_obj->data.sp.center));
	else if (scene->hit_obj->type == PLANE)
		normal = scene->hit_obj->data.pl.normal;
	// cylinders are a bitch
	else if (scene->hit_obj->type == CYLINDER)
	{
		hit_to_center = vec_sub(hit_point, scene->hit_obj->data.cy.center);
		proj = vec_dot(hit_to_center, scene->hit_obj->data.cy.axis);
		// Bottom cap: proj near 0
		if (proj < 0.001)
			normal = vec_negate(scene->hit_obj->data.cy.axis);
		// Top cap: proj near height
		else if (proj > scene->hit_obj->data.cy.height - 0.001)
			normal = scene->hit_obj->data.cy.axis;
		// Body
		else
		{
			axis_point = vec_add(scene->hit_obj->data.cy.center,
					vec_mult(scene->hit_obj->data.cy.axis, proj));
			normal = vec_normalize(vec_sub(hit_point, axis_point));
		}
	}
}

void	light_calc(void)
{
	light_dir = vec_sub(scene->light->pos, hit_point);
	light_dist = vec_length(light_dir);
	light_dir = vec_normalize(light_dir);
	scene->shadow_obj = scene->objects;
	in_shadow = false;
	while (scene->shadow_obj)
	{
		shadow_t = -1.0;
		if (scene->shadow_obj->type == SPHERE)
			shadow_t = hit_sphere(hit_point, light_dir, scene->shadow_obj);
		else if (scene->shadow_obj->type == PLANE)
			shadow_t = hit_plane(hit_point, light_dir, scene->shadow_obj);
		else if (scene->shadow_obj->type == CYLINDER)
			shadow_t = hit_cylinder(hit_point, light_dir, scene->shadow_obj);
		// 0.001 to avoid self-intersection
		if (shadow_t > 0.001 && shadow_t < light_dist)
		{
			in_shadow = true;
			break ;
		}
		scene->shadow_obj = scene->shadow_obj->next;
	}
	diffuse = fmax(0.0, vec_dot(normal, light_dir));
	if (in_shadow)
		diffuse = 0.0;
	brightness = scene->ambient->ratio + (scene->light->brightness * diffuse);
	brightness = fmin(brightness, 1.0);
}

void	draw_pixel(t_rt *scene)
{
	if (scene->hit_obj->type == SPHERE)
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
		else if (scene->hit_obj->type == PLANE)
		{
			img->pixels[index + 0] = (int)(hit_obj->data.pl.color.r
					* brightness);
			img->pixels[index + 1] = (int)(hit_obj->data.pl.color.g
					* brightness);
			img->pixels[index + 2] = (int)(hit_obj->data.pl.color.b
					* brightness);
		}
		else if (scene->hit_obj->type == CYLINDER)
		{
			img->pixels[index + 0] = (int)(hit_obj->data.cy.color.r
					* brightness);
			img->pixels[index + 1] = (int)(hit_obj->data.cy.color.g
					* brightness);
			img->pixels[index + 2] = (int)(hit_obj->data.cy.color.b
					* brightness);
		}
}
