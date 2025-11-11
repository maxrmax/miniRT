/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:21:51 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/11 19:01:54 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	parse_ambient(char **line, t_rt *scene);
int	parse_camera(char **line, t_rt *scene);
int	parse_light(char **line, t_rt *scene);
int	parse_sphere(char **line, t_rt *scene);

int	check_rt_extension(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 4 || ft_strcmp(filename + len - 3, ".rt") != 0)
		return (printf("Wrong file typ\n"), 1);
	return (0);
}

int	check_line(char **line, t_rt *scene)
{
	if (!line || !line[0] || !scene)
		return (0);
	if (line[0][0] == '\0')
		return (1);
	if (ft_strcmp(line[0], "A") == 0)
		return (parse_ambient(line, scene));
	if (ft_strcmp(line[0], "C") == 0)
		return (parse_camera(line, scene));
	if (ft_strcmp(line[0], "L") == 0)
		return (parse_light(line, scene));
	if (ft_strcmp(line[0], "sp") == 0)
		return (parse_sphere(line, scene));
	if (ft_strcmp(line[0], "pl"))
		return();//TODO
	return (printf("Unknown element: %s\n", line[0]), 0);
}

int	parse_ambient(char **line, t_rt *scene)
{
	if (!scene->ambient)
		scene->ambient = malloc(sizeof(t_ambient));
	if (!scene->ambient)
		return (printf("Error: malloc failed for ambient\n"), 0);
	if (!parse_ratio(line[1], &scene->ambient->ratio)
		|| !parse_color(line[2], &scene->ambient->color))
		return (0);
	return (1);
}

int	parse_camera(char **line, t_rt *scene)
{
	if (!scene->camera)
		scene->camera = malloc(sizeof(t_camera));
	if (!scene->camera)
		return (printf("Error: malloc failed for camera\n"), 0);
	if (!parse_cordinates(line[1], &scene->camera->pos)
		|| !parse_dir(line[2], &scene->camera->dir)
		|| !parse_field_of_view(line[3], scene->camera))
		return (0);
	return (1);
}

int	parse_light(char **line, t_rt *scene)
{
	if (!scene->light)
		scene->light = malloc(sizeof(t_light));
	if (!scene->light)
		return (printf("Error: malloc failed for light\n"), 0);
	if (!parse_cordinates(line[1], &scene->light->pos)
		|| !parse_ratio(line[2], &scene->light->brightness)
		|| !parse_color(line[3], &scene->light->color))
		return (0);
	return (1);
}

int	parse_sphere(char **line, t_rt *scene)
{
	if (!scene->sphere)
		scene->sphere = malloc(sizeof(t_sp));
	if (!scene->sphere)
		return (printf("Error: malloc failed for sphere\n"), 0);
	if (!parse_cordinates(line[1], &scene->sphere->center)
		|| !parse_ratio(line[2], &scene->sphere->diameter)
		|| !parse_color(line[3], &scene->sphere->color))
		return (0);
	return (1);
}


int	parsing_scene(char *av, t_rt *scene)
{
	int		fd;
	char	*line;
	char	**split;

	if (check_rt_extension(av))
		return (1);
	fd = open(av, O_RDONLY);
	if (fd < 0)
		return (perror("Error opening file"), 1);
	while ((line = get_next_line(fd)) != NULL)
	{
		split = ft_split_whitespace(line);
		if (!check_line(split, scene))
			return (free(line), ft_free_split(split), close(fd), 1);
		free(line);
		ft_free_split(split);
	}
	close(fd);
	return (0);
}
