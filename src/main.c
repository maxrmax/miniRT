/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:21:17 by mring             #+#    #+#             */
/*   Updated: 2025/12/22 16:38:48 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
	if (!scene->objects)
	{
		scene->objects = malloc(sizeof(t_obj));
		if (!scene->objects)
		{
			printf("Malloc faild\n");
			free_scenes(scene);
			return (0);
		}
		scene->objects->type = NONE;
		ft_memset(&scene->objects->data, 0, sizeof(scene->objects->data));
		scene->objects->next = NULL;
	}
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
	scene = ft_calloc(1, sizeof(t_rt));
	if (!scene)
		return (printf("Error: malloc failed\n"), 1);
	if (parsing_scene(av[1], scene))
		return (free_scenes(scene), 1);
	if (validate_scene(scene))
		window_loop(scene);
	free_scenes(scene);
	return (0);
}
