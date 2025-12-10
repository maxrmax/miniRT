/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:54:36 by mring             #+#    #+#             */
/*   Updated: 2025/12/10 16:01:54 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void resize_hook(int32_t width, int32_t height, void *param)
{
    t_rt *scene = param;

    if (width <= 0 || height <= 0)
        return;

    // Just store the new dimensions, don't render yet
    scene->pending_width = width;
    scene->pending_height = height;
    scene->needs_resize = true;
}
void loop_hook(void *param)
{
    t_rt *scene = param;
    
    // Only do the expensive work when resize is done
    if (scene->needs_resize)
    {
        scene->needs_resize = false;
        
        // Delete old image
        mlx_delete_image(scene->window, scene->img);
        
        // Create new image with new dimensions
        scene->img = mlx_new_image(scene->window, 
                                   scene->pending_width, 
                                   scene->pending_height);
        if (!scene->img)
        {
            mlx_terminate(scene->window);
            exit(1);
        }
        
        // Re-render scene
        pre_calc_camera(scene);
        render_scene(scene);
        mlx_image_to_window(scene->window, scene->img, 0, 0);
    }
}


static void	key_hook(mlx_key_data_t keydata, void *param)
{
	mlx_t	*mlx;

	mlx = param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(mlx);
}

static void	render_pixel(t_rt *scene, int i, int j)
{
	calc_camera(scene, i, j);
	calc_objs(scene);
	calc_pixel(scene);
}

void render_scene(t_rt *scene)
{
    uint32_t i, j;

    for (i = 0; i < scene->img->height; i++)
        for (j = 0; j < scene->img->width; j++)
            render_pixel(scene, i, j);
}


static void	init_window(t_rt *scene)
{
	scene->window = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!scene->window)
		exit(1);
	scene->img = mlx_new_image(scene->window, WIDTH, HEIGHT);
	if (!scene->img)
	{
		mlx_terminate(scene->window);
		exit(1);
	}
}

void window_loop(t_rt *scene)
{
    scene->hit_obj = NULL;
    
    // Initialize resize fields
    scene->needs_resize = false;
    scene->pending_width = WIDTH;
    scene->pending_height = HEIGHT;
    
    init_window(scene);
    pre_calc_camera(scene);
    render_scene(scene);
    mlx_image_to_window(scene->window, scene->img, 0, 0);
    
    mlx_key_hook(scene->window, key_hook, scene->window);
    mlx_resize_hook(scene->window, resize_hook, scene);
    
    // Add the loop hook - this runs every frame
    mlx_loop_hook(scene->window, loop_hook, scene);
    
    mlx_loop(scene->window);
    mlx_delete_image(scene->window, scene->img);
    mlx_terminate(scene->window);
}
