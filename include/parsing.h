/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:57:58 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/11 05:09:37 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PARSING_H
# define PARSING_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "miniRT.h"


typedef struct s_vec3
{
	double	e[3];
}			t_vec3;
typedef struct s_color
{
	float	r;
	float	g;
	float	b;
}	t_color;

typedef struct s_ambient
{
    float   ratio;
    t_color color;    
}   t_ambient;

typedef struct s_camera
{
    t_vec3  pos;
    t_vec3  dir;
    float   fov;
}   t_camera;

typedef struct s_light
{
    t_color color;
    t_vec3  pos;
    float   brightness;
}   t_light;

typedef struct s_sp
{
     t_color color;
    t_vec3  center;
    float   diameter;
}   t_sp;

typedef struct s_pl
{
    t_vec3  point;
    t_vec3  normal;
    t_color color;
}   t_pl;

typedef struct s_cy
{
    t_vec3  center;
    t_vec3  axis;
    float   diameter;
    float   hight;
    t_color color;
}   t_cy;

typedef struct s_rt
{
    t_ambient   ambient;
    t_camera    camera;
    t_light     light;
    t_sp        sphere;
    t_pl        palne;
    t_cy        cylinder;
    int         count;
}  t_rt;

int    parsing_scene(char *av, t_rt scene);

void	ft_free_split(char **arr);

# endif
