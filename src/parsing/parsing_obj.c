/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_obj.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:49:51 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/21 16:58:26 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	parse_plane(char **line, t_rt *scene)
{
	t_obj_data	plane;

	if (!parse_cordinates(line[1], &plane.pl.point) || !parse_dir(line[2],
			&plane.pl.normal) || !parse_color(line[3], &plane.pl.color))
		return (0);
	add_obj(scene, PLANE, plane);
	return (1);
}

int	parse_cylinder(char **line, t_rt *scene)
{
	t_obj_data	cylinder;

	if (!parse_cordinates(line[1], &cylinder.cy.center) || !parse_dir(line[2],
			&cylinder.cy.axis) || !parse_float(line[3], &cylinder.cy.diameter)
		|| !parse_float(line[4], &cylinder.cy.height) || !parse_color(line[5],
			&cylinder.cy.color))
		return (0);
	add_obj(scene, CYLINDER, cylinder);
	return (1);
}

int	parse_sphere(char **line, t_rt *scene)
{
	t_obj_data	sphere;

	if (!parse_cordinates(line[1], &sphere.sp.center) || !parse_float(line[2],
			&sphere.sp.diameter) || !parse_color(line[3], &sphere.sp.color))
		return (0);
	add_obj(scene, SPHERE, sphere);
	return (1);
}
