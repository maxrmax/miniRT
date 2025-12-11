/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 12:33:03 by mring             #+#    #+#             */
/*   Updated: 2025/12/11 09:34:51 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	calc_intersections(t_vec3 ray_perp, t_vec3 oc_perp,
		double discriminant, t_obj *obj)
{
	double	a;
	double	b;

	a = vec_dot(ray_perp, ray_perp);
	b = 2.0 * vec_dot(ray_perp, oc_perp);
	obj->data.cy.t1 = (-b - sqrt(discriminant)) / (2.0 * a);
	obj->data.cy.t2 = (-b + sqrt(discriminant)) / (2.0 * a);
}

static double	calc_disc(t_vec3 ray_perp, t_vec3 oc_perp, double radius)
{
	double	a;
	double	b;
	double	c;

	a = vec_dot(ray_perp, ray_perp);
	b = 2.0 * vec_dot(ray_perp, oc_perp);
	c = vec_dot(oc_perp, oc_perp) - radius * radius;
	return (b * b - 4.0 * a * c);
}

static t_vec3	calc_oc_perp(t_obj *obj, t_vec3 objcent)
{
	t_vec3	projection;
	double	dot;

	dot = vec_dot(objcent, obj->data.cy.axis);
	projection = vec_mult(obj->data.cy.axis, dot);
	return (vec_sub(objcent, projection));
}

static t_vec3	calc_ray_perp(t_obj *obj)
{
	t_vec3	projection;
	double	dot;

	dot = vec_dot(obj->ray_dir, obj->data.cy.axis);
	projection = vec_mult(obj->data.cy.axis, dot);
	return (vec_sub(obj->ray_dir, projection));
}

double	cy_base(t_obj *obj)
{
	double	discriminant;
	t_vec3	objcent;
	t_vec3	ray_perp;
	t_vec3	oc_perp;

	objcent = vec_sub(obj->ray_origin, obj->data.cy.center);
	ray_perp = calc_ray_perp(obj);
	oc_perp = calc_oc_perp(obj, objcent);
	discriminant = calc_disc(ray_perp, oc_perp, obj->data.cy.diameter / 2.0);
	if (discriminant < 0)
		return (-1.0);
	calc_intersections(ray_perp, oc_perp, discriminant, obj);
	return (0);
}
