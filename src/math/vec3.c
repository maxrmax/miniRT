/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:53:44 by mring             #+#    #+#             */
/*   Updated: 2025/11/10 17:39:41 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"



/*
need a brdf function for light refraction on surfaces and color calculation
Bi-Directional Reflectance Distribution Function
the function calculates the color the ray will have after in bounces off a surface/intersects with any object
which can happen multiple times.
the more rays the more calculation the more calls the slower the program -> here we can optimize with logic


*/