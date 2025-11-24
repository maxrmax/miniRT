/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_scene.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 17:03:30 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/24 17:52:20 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	parse_ambient(char **line, t_rt *scene)
{
	if (!scene->ambient)
	{
		if (!line[1] || !line[2])
		{
			printf("Something went wrong!!!\n");
			return (0);
		}
		scene->ambient = malloc(sizeof(t_ambient));
		if (!scene->ambient)
			return (printf("Error: malloc failed for ambient\n"), 0);
		if (!parse_float_with_range(line[1], &scene->ambient->ratio, 0.0, 1.0)
			|| !parse_color(line[2], &scene->ambient->color))
			return (0);
		return (1);
	}
	else
		return (1);
}

int	parse_camera(char **line, t_rt *scene)
{
	if (!scene->camera)
	{
		scene->camera = malloc(sizeof(t_camera));
		if (!scene->camera)
			return (printf("Error: malloc failed for camera\n"), 0);
		if (!parse_cordinates(line[1], &scene->camera->pos)
			|| !parse_dir(line[2], &scene->camera->dir)
			|| !parse_field_of_view(line[3], scene->camera))
			return (0);
		return (1);
	}
	else
		return (1);
}

int	parse_light(char **line, t_rt *scene)
{
	if (!scene->light)
	{
		scene->light = malloc(sizeof(t_light));
		if (!scene->light)
			return (printf("Error: malloc failed for light\n"), 0);
		if (!parse_cordinates(line[1], &scene->light->pos)
			|| !parse_float_with_range(line[2], &scene->light->brightness, 0.0,
				1.0) || !parse_color(line[3], &scene->light->color))
			return (0);
		return (1);
	}
	else
		return (1);
}
