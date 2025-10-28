/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:21:17 by mring             #+#    #+#             */
/*   Updated: 2025/10/28 13:42:23 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	main(void)
{
	int		width;
	int		height;
	int		i;
	int		j;
	double	r;
	double	g;
	double	b;
	int		ir;
	int		ig;
	int		ib;

	width = 256;
	height = 256;
	printf("P3\n%d %d\n255\n", width, height);
	i = 0;
	while (i < height)
	{
		fprintf(stderr, "\rScanlines remaining: %d\n", height - i);
		j = 0;
		while (j < width)
		{
			r = (double)j / (height - 1);
			g = (double)i / (width - 1);
			b = 0.0;
			ir = (int)255.999 * r;
			ig = (int)255.999 * g;
			ib = (int)255.999 * b;
			printf("%d %d %d\n", ir, ig, ib);
			j++;
		}
		i++;
	}
}
