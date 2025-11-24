/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 15:45:28 by mring             #+#    #+#             */
/*   Updated: 2025/11/21 21:41:45 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	draw_elements(t_rt *scenes)
{
	// ambient has no pos, its deciding color and ratio
	// think about its implementation
	if (scenes->camera)
		vec_new(scenes->camera->pos.x, scenes->camera->pos.y,
			scenes->camera->pos.z);
	if (scenes->light)
		vec_new(scenes->light->pos.x, scenes->light->pos.y,
			scenes->light->pos.z);
	// am i missing anything?
	// matrixes?
	// TODO: research
}

void	object_drawing(t_rt *scenes)
{
	int	i;

	draw_elements(scenes);
	i = 0;
	// draw objects
	while (i < scenes)
	{
		// sphere has vec3 center, float diameter,
		// how to place it?
		// if (scenes->sphere)              //[i])
		// draw_sphere(scenes->sphere); //[i]);
		// plane has a vec3 point, a normal, and color
		// if (scenes->plane[i])
		// draw_plane(scenes->plane[i]);
		// cylinder has center, axis (matrix?), diameter, height, color
		// if (scenes->cylinder[i])
		// draw_cylinder(scenes->cylinder[i]);
		i++;
	}
	// same here, anything missing?
	// TODO: learn how to apply matrix.
	// TODO: research
}

int	draw_scene(t_rt *scenes)
{
	printf("draw_scene\n");
	return (-1);
	object_drawing(scenes);
	// draw every object in the list
	// will later be an array needed to be iterated.
	// can be multiple cy, pl, sp
	// then apply matrix: rotation, color, etc.
	return (0);
}
