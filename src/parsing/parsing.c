/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:21:51 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/19 01:14:05 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	parse_ambient(char **line, t_rt *scene);
int	parse_camera(char **line, t_rt *scene);
int	parse_light(char **line, t_rt *scene);
int	parse_sphere(char **line, t_rt *scene);
int	parse_plane(char **line, t_rt *scene);
int	parse_cylinder(char **line, t_rt *scene);
void	add_obj(t_rt *rt, t_obj_type type, t_obj_data data);

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
	if (ft_strcmp(line[0], "pl") == 0)
		return(parse_plane(line, scene));
	if (ft_strcmp(line[0], "cy") == 0)
		return(parse_cylinder(line, scene));
	return (printf("Unknown element: %s\n", line[0]), 0);
}

int	parse_plane(char **line, t_rt *scene)
{
	t_obj_data plane;

	if (!parse_cordinates(line[1], &plane.pl.point)
		|| !parse_dir(line[2], &plane.pl.normal)
		|| !parse_color(line[3], &plane.pl.color))
		return (0);
	add_obj(scene, PLANE, plane);
	return (1);
}

int	parse_cylinder(char **line, t_rt *scene)
{
	t_obj_data cylinder;

	if (!parse_cordinates(line[1], &cylinder.cy.center)
		|| !parse_dir(line[2], &cylinder.cy.axis)
		|| !parse_float(line[3], &cylinder.cy.diameter)
		|| !parse_float(line[4], &cylinder.cy.height)
		|| !parse_color(line[5], &cylinder.cy.color))
			return (0);
	add_obj(scene, CYLINDER, cylinder);
	return (1);
	}


int	parse_ambient(char **line, t_rt *scene)
{
	if (!scene->ambient)
	{
		scene->ambient = malloc(sizeof(t_ambient));
		if (!scene->ambient)
			return (printf("Error: malloc failed for ambient\n"), 0);
		if (!parse_ratio(line[1], &scene->ambient->ratio, 1)
			|| !parse_color(line[2], &scene->ambient->color))
			return (0);
		return (1);
	}
	else
	{
		printf("Just one Ambient possible\n");
		return(0);
	}
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
	{
		printf("Just one Camera possible\n");
		return(0);
	}
}

int	parse_light(char **line, t_rt *scene)
{
	if (!scene->light)
	{
		scene->light = malloc(sizeof(t_light));
		if (!scene->light)
			return (printf("Error: malloc failed for light\n"), 0);
		if (!parse_cordinates(line[1], &scene->light->pos)
			|| !parse_ratio(line[2], &scene->light->brightness, 0)
			|| !parse_color(line[3], &scene->light->color))
			return (0);
		return (1);
	}
	else
	{
		printf("Just one light possible\n");
		return(0);
	}
}
int	parse_sphere(char **line, t_rt *scene)
{
	t_obj_data sphere;

	if (!parse_cordinates(line[1], &sphere.sp.center)
		|| !parse_ratio(line[2], &sphere.sp.diameter, 0)
		|| !parse_color(line[3], &sphere.sp.color))
		return (0);
	add_obj(scene, SPHERE, sphere);
	return (1);
}

void	add_obj(t_rt *rt, t_obj_type type, t_obj_data data)
{
	t_obj	*new_obj;

	new_obj = malloc(sizeof(t_obj));
	if (!new_obj)
	{
		return ;
	}
	new_obj->type = type;
	new_obj->data = data;
	new_obj->next = rt->objects;
	rt->objects = new_obj;
}

int	parsing_scene(char *av, t_rt *scene)
{
	int		fd;
	char	*line;
	char	**split;
	int		i;

	i = 0;
	if (check_rt_extension(av))
		return (1);
	fd = open(av, O_RDONLY);
	if (fd < 0)
		return (perror("Error opening file"), 1);
	while ((line = get_next_line(fd)) != NULL)
	{
		i++;
		if (line[0] == '\0' || line[0] == '\n' || line[0] == '#') {
			free(line);
			continue;
		}
		split = ft_split_whitespace(line);
		if (!check_line(split, scene))
			return (free(line), ft_free_split(split), close(fd), i);
		free(line);
		ft_free_split(split);
	}
	close(fd);
	return (0);
}
