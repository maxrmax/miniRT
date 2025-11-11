/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 15:11:42 by mring             #+#    #+#             */
/*   Updated: 2025/11/11 05:47:54 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "libft.h"
# include "parsing.h"
# include "vec3.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

# define WIDTH 860
# define HEIGHT 640

typedef struct s_vec3
{
	double	e[3];
}			t_vec3;

char **ft_split_whitespace(char *str);


#endif