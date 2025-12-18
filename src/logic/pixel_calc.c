/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_calc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 13:07:24 by mring             #+#    #+#             */
/*   Updated: 2025/12/18 17:05:09 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	draw_pixel(t_rt *scene)
{
	t_color	*color;
	double	ambient_contribution;
	double	light_contribution;

	if (scene->hit_obj->type == SPHERE)
		color = &scene->hit_obj->data.sp.color;
	else if (scene->hit_obj->type == PLANE)
		color = &scene->hit_obj->data.pl.color;
	else
		color = &scene->hit_obj->data.cy.color;
	ambient_contribution = scene->ambient->ratio;
	light_contribution = scene->light->brightness * scene->hit_obj->diffuse;
	scene->img->pixels[scene->index + 0] = fmin(255, (int)(color->r
				* ambient_contribution * scene->ambient->color.r / 255.0
				+ color->r * light_contribution * scene->light->color.r
				/ 255.0));
	scene->img->pixels[scene->index + 1] = fmin(255, (int)(color->g
				* ambient_contribution * scene->ambient->color.g / 255.0
				+ color->g * light_contribution * scene->light->color.g
				/ 255.0));
	scene->img->pixels[scene->index + 2] = fmin(255, (int)(color->b
				* ambient_contribution * scene->ambient->color.b / 255.0
				+ color->b * light_contribution * scene->light->color.b
				/ 255.0));
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
