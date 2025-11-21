/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:21:51 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/21 17:10:36 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static int	check_line(char **line, t_rt *scene)
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
		return (parse_plane(line, scene));
	if (ft_strcmp(line[0], "cy") == 0)
		return (parse_cylinder(line, scene));
	return (1);
}

static int	parse_line(char *line, int line_num, t_rt *scene, int fd)
{
	char	**split;

	if (line[0] == '\n')
	{
		free(line);
		return (1);
	}
	split = ft_split_whitespace(line);
	if (!check_line(split, scene))
	{
		printf("Error on line %d\n", line_num);
		free(line);
		ft_free_split(split);
		close(fd);
		return (0);
	}
	free(line);
	ft_free_split(split);
	return (1);
}

int	parsing_scene(char *av, t_rt *scene)
{
	int		fd;
	char	*line;
	int		line_num;

	line_num = 0;
	if (check_rt_extension(av))
		return (1);
	fd = open(av, O_RDONLY);
	if (fd < 0)
		return (perror("Error opening file"), 1);
	line = get_next_line(fd);
	while (line != NULL)
	{
		line_num++;
		if (!parse_line(line, line_num, scene, fd))
			return (1);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
