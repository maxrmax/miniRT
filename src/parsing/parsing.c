/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:21:51 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/11 04:22:13 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	check_rt_extension(char *filename)
{
	int	len = ft_strlen(filename);
	if (len < 4 || ft_strcmp(filename + len - 3, ".rt") != 0)
		return (printf("Wrong file typ\n"), 1);
	return (0);
}

int    check_line(char *token, t_rt scene)
{
    if (strcmp(token[0], "A") == 0)
        parse_ambient(token, scene);
    else if (strcmp(token[0], "C") == 0)
        parse_camera(token, scene);
    else if (strcmp(token[0], "L") == 0)
        parse_light(token, scene);
    else if (strcmp(token[0], "sp") == 0)
        parse_sphere(token, scene);
    else if (strcmp(token[0], "pl") == 0)
        parse_plane(token, scene);
    else if (strcmp(token[0], "cy") == 0)
        parse_cylinder(token, scene);
    else
        printf("Unknown identifier: %s\n", token[0]);
}

int    parsing_scene(char *av, t_rt scene)
{
    int     fd;
    char    *line;
    
    if (check_rt_extension(av))
		return (1);
    fd = open(av, O_RDONLY);
    if (fd < 0)
	{
		perror("Error opening file");
		return (1);
	}
    while ((line = get_next_line(fd)) != NULL)
    {

        if (check_line(line, scene))
        {
            free(line);
            close(fd);
            return (0);
        }
        free(line);
    }
    // if(check_scenen_arguments(scene))
    //     return(printf("faild: something is wrong with the file"), 1);
    return(0);
    
}
