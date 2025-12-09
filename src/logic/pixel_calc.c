/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_calc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 13:07:24 by mring             #+#    #+#             */
/*   Updated: 2025/12/05 15:48:20 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	draw_pixel(t_rt *scene)
{
	t_color	*color;

	if (scene->hit_obj->type == SPHERE)
		color = &scene->hit_obj->data.sp.color;
	else if (scene->hit_obj->type == PLANE)
		color = &scene->hit_obj->data.pl.color;
	else
		color = &scene->hit_obj->data.cy.color;
	scene->img->pixels[scene->index + 0] = (int)(color->r
			* scene->hit_obj->brightness);
	scene->img->pixels[scene->index + 1] = (int)(color->g
			* scene->hit_obj->brightness);
	scene->img->pixels[scene->index + 2] = (int)(color->b
			* scene->hit_obj->brightness);
}

void	calc_pixel(t_rt *scene)
{
	if (scene->hit_obj)
	{
		hit_calc(scene->hit_obj);
		light_calc(scene);
		draw_pixel(scene);
	}
	else
	{
		scene->img->pixels[scene->index + 0] = 0;
		scene->img->pixels[scene->index + 1] = 0;
		scene->img->pixels[scene->index + 2] = 0;
	}
	scene->img->pixels[scene->index + 3] = 255;
}
