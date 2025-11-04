/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:21:51 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/04 15:22:16 by jpflegha         ###   ########.fr       */
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

void    check_line(char *line, t_rt scene)
{
    
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
        check_line(line, scene);
        free(line);
    }
    if(check_scenen_arguments(scene))
        return(printf("faild: something is wrong with the file"), 1);

    
}
