/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:42:11 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/03 18:42:32 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_read_loop(int fd, char *save, char *line, int *bytes_read)
{
	char	*temp;

	*bytes_read = 1;
	while (!ft_strchr(line, '\n') && *bytes_read != 0)
	{
		*bytes_read = read(fd, save, BUFFER_SIZE);
		if (*bytes_read == -1)
			return (free(line), NULL);
		save[*bytes_read] = '\0';
		temp = ft_strjoin(line, save);
		if (temp == NULL)
			return (free(line), NULL);
		free(line);
		line = temp;
	}
	return (line);
}

char	*ft_get_line(char *str)
{
	char	*line;
	size_t	i;

	i = 0;
	while (str[i] != '\n' && str[i] != '\0')
		i++;
	if (str[i] == '\n')
		line = ft_substr(str, 0, i + 1);
	else
		line = ft_strdup(str);
	return (line);
}

int	ft_reset_save(char *save)
{
	int	i;

	i = 0;
	while (save[i] != '\n' && save[i] != '\0')
		i++;
	if (save[i] == '\n')
	{
		ft_memcpy(save, ft_strchr(save, '\n') + 1, ft_strlen(ft_strchr(save,
					'\n')));
		return (1);
	}
	return (0);
}
char	*get_next_line(int fd)
{
	static char	save[BUFFER_SIZE + 1];
	char		*line;
	char		*temp;
	int			bytes_read;

	if (BUFFER_SIZE <= 0 || fd < 0 || read(fd, save, 0) == -1)
		return (ft_memcpy(save, "\0", 1), NULL);
	line = ft_strdup(save);
	if (line == NULL)
		return (NULL);
	line = ft_read_loop(fd, save, line, &bytes_read);
	if (line == NULL)
		return (NULL);
	if (ft_strlen(line) == 0 && bytes_read == 0)
		return (free(line), NULL);
	temp = ft_get_line(line);
	return (ft_reset_save(save), free(line), temp);
}

