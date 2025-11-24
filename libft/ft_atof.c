/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 04:52:59 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/21 17:32:30 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

/// @brief Converts a string to a floating-point number (double)
/// The function skips leading whitespace, handles an optional '+' or '-' sign,
/// and converts both the integer and fractional parts of the number.
/// It stops parsing when a non-numeric character (other than '.') is encountered.
/// @param char *str 
/// @return double
static const char	*skip_whitespace(const char *str)
{
	while (*str == ' ' || *str == '\t' || *str == '\n' ||
		   *str == '\r' || *str == '\f' || *str == '\v')
		str++;
	return (str);
}

static float	parse_integer(const char **str)
{
	float	result;

	result = 0.0;
	while (**str >= '0' && **str <= '9')
	{
		result = result * 10.0 + (**str - '0');
		(*str)++;
	}
	return (result);
}

static float	parse_fraction(const char **str, float result)
{
	float	divisor;

	divisor = 1.0;
	if (**str == '.')
	{
		(*str)++;
		while (**str >= '0' && **str <= '9')
		{
			divisor *= 10.0;
			result += (**str - '0') / divisor;
			(*str)++;
		}
	}
	return (result);
}

float	ft_atof(const char *str)
{
	float	result;
	int		sign;

	sign = 1;
	str = skip_whitespace(str);
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	result = parse_integer(&str);
	result = parse_fraction(&str, result);
	return (result * sign);
}
