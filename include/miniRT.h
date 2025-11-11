/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 15:11:42 by mring             #+#    #+#             */
/*   Updated: 2025/11/11 14:07:36 by mring            ###   ########.fr       */
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

char **ft_split_whitespace(char *str);


#endif