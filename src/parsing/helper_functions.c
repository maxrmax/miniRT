/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:22:09 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/24 17:54:24 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/// @brief simple function to check if it is a number
/// @param input
/// @return
int	pars_int(char *input)
{
	int	i;

	i = 0;
	if (!input || !*input)
		return (printf("Input is missing\n"), 0);
	if (input[i] == '-' || input[i] == '+')
		i++;
	while (input[i])
	{
		if (!ft_isdigit(input[i]))
			return (printf("%s is not a valid integer\n", input), 0);
		i++;
	}
	return (1);
}

int	parse_float(char *str, float *f)//maybe not needed
{
	float	num;

	if (!str || !f)
		return (printf("Input is missing\n"), 0);
	if (!ft_isfloat(str))
		return (printf("Invalid float: %s\n", str), 0);
	num = ft_atof(str);
	if (num < 0.0f)
		return (printf("Value must be positive: %.1f\n", num), 0);
	*f = num;
	return (1);
}

int	check_rt_extension(char *filename)
{
	int	len;

	if (!filename)
		return (printf("Error: filename is NULL\n"), 1);
	len = ft_strlen(filename);
	if (len < 4 || ft_strcmp(filename + len - 3, ".rt") != 0)
		return (printf("Error: Wrong file type (expected .rt)\n"), 1);
	return (0);
}
