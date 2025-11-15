/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:21:51 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/15 14:20:41 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	parse_ambient(char **line, t_rt *scene);
int	parse_camera(char **line, t_rt *scene);
int	parse_light(char **line, t_rt *scene);
int	parse_sphere(char **line, t_rt *scene);
int	parse_plane(char **line, t_rt *scene);
int	parse_cylinder(char **line, t_rt *scene);

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
		return (scene->count++, parse_camera(line, scene));
	if (ft_strcmp(line[0], "L") == 0)
		return (scene->count++, parse_light(line, scene));
	// objects below this can be multiple
	// need to allocate an array of objects for this
	// t_sp **sphere, t_pl **plane, t_cy **cylinger
	// TODO:
	if (ft_strcmp(line[0], "sp") == 0)
		return (scene->count++, parse_sphere(line, scene));
	if (ft_strcmp(line[0], "pl") == 0)
		return(scene->count++, parse_plane(line, scene));
	if (ft_strcmp(line[0], "cy") == 0)
		return(scene->count++, parse_cylinder(line, scene));
	return (printf("Unknown element: %s\n", line[0]), 0);
}

int	parse_plane(char **line, t_rt *scene)
{
	if (!scene->plane)
			scene->plane = malloc(sizeof(t_pl));
		if (!scene->plane)
			return (printf("Error: malloc failed for plane\n"), 0);
		if (!parse_cordinates(line[1], &scene->plane->point)
			|| !parse_dir(line[2], &scene->plane->normal)
			|| !parse_color(line[3], &scene->plane->color))
			return (0);
		return (1);
}

int	parse_cylinder(char **line, t_rt *scene)
{
	if (!scene->cylinder)
		scene->cylinder = malloc(sizeof(t_cy));
	if (!scene->cylinder)
		return (printf("Error: malloc failed for cylinder\n"), 0);
	if (!parse_cordinates(line[1], &scene->cylinder->center)
		|| !parse_dir(line[2], &scene->cylinder->axis)
		|| !parse_float(line[3], &scene->cylinder->diameter)
		|| !parse_float(line[4], &scene->cylinder->height)
		|| !parse_color(line[5], &scene->cylinder->color))
			return (0);
		return (1);
	}


int	parse_ambient(char **line, t_rt *scene)
{
	if (!scene->ambient)
		scene->ambient = malloc(sizeof(t_ambient));
	if (!scene->ambient)
		return (printf("Error: malloc failed for ambient\n"), 0);
	if (!parse_ratio(line[1], &scene->ambient->ratio, 1)
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
		|| !parse_ratio(line[2], &scene->light->brightness, 0)
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
		|| !parse_ratio(line[2], &scene->sphere->diameter, 0)
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
