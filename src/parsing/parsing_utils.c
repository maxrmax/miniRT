/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 04:23:15 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/24 14:33:15 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/// @brief function to pars the ratio (0.0 - 1.0)
/// @param ratio
/// @param r
/// @return
int	parse_ratio(char *ratio, float *r, int check_range)
{
	float	num;

	if (!ratio || !r)
		return (printf("Input is missing\n"), 0);
	if (!ft_isfloat(ratio))
		return (printf("Invalid float: %s\n", ratio), 0);
	num = ft_atof(ratio);
	if (check_range && (num < 0.0f || num > 1.0f))
		return (printf("%f %s should be between 0.0 and 1.0\n", num, ratio), 0);
	*r = num;
	return (1);
}

/// @brief function to pars the filed of view (0-180 degree)
/// @param fov_str
/// @param camera
/// @return
int	parse_field_of_view(char *fov_str, t_camera *camera)
{
	int	fov;

	if (!fov_str || !camera)
		return (printf("Input is missing\n"), 0);
	if (!pars_int(fov_str))
		return (0);
	fov = ft_atoi(fov_str);
	if (fov < 0 || fov > 180)
	{
		printf("%d is invalid: field of view should be between 0 and 180\n",
			fov);
		return (0);
	}
	camera->fov = fov;
	return (1);
}

int	parse_dir(char *dir, t_vec3 *vec)
{
	if (!parse_cordinates(dir, vec))
		return (0);
	if (vec->x < -1.0f || vec->x > 1.0f || vec->y < -1.0f || vec->y > 1.0f
		|| vec->z < -1.0f || vec->z > 1.0f)
		return (printf("Direction must be in range [-1,1]\n"), 0);
	if (vec->x == 0.0f && vec->y == 0.0f && vec->z == 0.0f)
		return (printf("Direction vector cannot be zero\n"), 0);
	*vec = vec_normalize(*vec);
	return (1);
}

/// @brief function to pars a color i RGB 0-255,0-255,0-255
/// @param color
/// @param rgb
/// @return
int	parse_color(char *color, t_color *rgb)
{
	char	**split;
	int		count;

	if (!color || !rgb)
		return (printf("Input is missing\n"), 0);
	split = ft_split(color, ',');
	if (!split)
		return (0);
	count = 0;
	while (split[count])
		count++;
	if (count != 3)
	{
		ft_free_split(split);
		return (printf("Color must have exactly 3 values (0–255)\n"), 0);
	}
	rgb->r = ft_atoi(split[0]);
	rgb->g = ft_atoi(split[1]);
	rgb->b = ft_atoi(split[2]);
	ft_free_split(split);
	if (rgb->r < 0 || rgb->r > 255 || rgb->g < 0 || rgb->g > 255 || rgb->b < 0
		|| rgb->b > 255)
		return (printf("Color values must be between 0 and 255\n"), 0);
	return (1);
}

int	parse_cordinates(char *cordi, t_vec3 *vec)
{
	char	**split;
	int		count;

	if (!cordi || !vec)
		return (printf("Input is missing\n"), 0);
	split = ft_split(cordi, ',');
	if (!split)
		return (0);
	count = 0;
	while (split[count])
		count++;
	if (count != 3)
	{
		ft_free_split(split);
		return (printf("Coordinates must have exactly 3 values\n"), 0);
	}
	if (!ft_isfloat(split[0]) || !ft_isfloat(split[1]) || !ft_isfloat(split[2]))
	{
		return (ft_free_split(split), printf("Invalid coordinate format\n"), 0);
	}
	vec->x = ft_atof(split[0]);
	vec->y = ft_atof(split[1]);
	vec->z = ft_atof(split[2]);
	ft_free_split(split);
	return (1);
}
