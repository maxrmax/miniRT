/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_prints.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 08:56:24 by mring             #+#    #+#             */
/*   Updated: 2025/12/02 09:00:27 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/* --- Helpers ------------------------------------------------------------ */

static void	print_vec3(const char *name, t_vec3 v)
{
	printf("%s: (%.1f, %.1f, %.1f)\n", name, v.x, v.y, v.z);
}

static void	print_color(const char *name, t_color c)
{
	printf("%s: (%d, %d, %d)\n", name, c.r, c.g, c.b);
}

/* --- Ambient ------------------------------------------------------------ */

static void	print_ambient(t_ambient *a)
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

static void	print_camera(t_camera *cam)
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

static void	print_light(t_light *l)
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

static void	print_objects(t_obj *obj)
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
