/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:00:51 by jpflegha          #+#    #+#             */
/*   Updated: 2025/12/11 09:05:44 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	resize_hook(int32_t width, int32_t height, void *param)
{
	t_rt	*scene;

	scene = param;
	if (width <= 0 || height <= 0)
		return ;
	scene->pending_width = width;
	scene->pending_height = height;
	scene->needs_resize = true;
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	mlx_t	*mlx;

	mlx = param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(mlx);
}
