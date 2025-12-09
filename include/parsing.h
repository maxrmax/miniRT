/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:57:58 by jpflegha          #+#    #+#             */
/*   Updated: 2025/12/09 18:03:58 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "miniRT.h"
# include "vec3.h"

# define WIDTH 860
# define HEIGHT 640

typedef enum e_obj_type
{
	SPHERE,
	PLANE,
	CYLINDER
}					t_obj_type;

typedef struct s_color
{
	int				r;
	int				g;
	int				b;
}					t_color;

typedef struct s_ambient
{
	float			ratio;
	t_color			color;
}					t_ambient;

typedef struct s_camera
{
	t_vec3			pos;
	t_vec3			dir;
	int				fov;
	double			fov_rad;
	double			viewport_height;
	double			viewport_width;
	t_vec3			forward;
	t_vec3			world_up;
	t_vec3			right;
	t_vec3			up;

}					t_camera;

typedef struct s_light
{
	t_color			color;
	t_vec3			pos;
	float			brightness;
}					t_light;

typedef struct s_sp
{
	t_color			color;
	t_vec3			center;
	float			diameter;
}					t_sp;

typedef struct s_pl
{
	t_vec3			point;
	t_vec3			normal;
	t_color			color;

}					t_pl;

typedef struct s_cap
{
	t_vec3			radial;
	double			t;
	t_vec3			p;
}					t_cap;

typedef struct s_cy
{
	t_vec3			center;
	t_vec3			axis;
	float			diameter;
	float			height;
	t_color			color;
	double			t1;
	double			t2;
}					t_cy;

typedef union u_obj_data
{
	t_sp			sp;
	t_pl			pl;
	t_cy			cy;
}					t_obj_data;

typedef struct s_obj
{
	t_obj_type		type;
	t_obj_data		data;
	struct s_obj	*next;
	bool			hit;
	t_vec3			ray_dir;
	double			closest_t;
	double			t;
	t_vec3			ray_origin;
	double			diffuse;
	double			brightness;
	bool			in_shadow;
	t_vec3			hit_point;
	t_vec3			normal;
	t_vec3			light_dir;
	double			light_dist;
}					t_obj;

typedef struct s_rt
{
	t_ambient		*ambient;
	t_camera		*camera;
	t_light			*light;
	t_obj			*objects;
	t_obj			*hit_obj;
	uint32_t		index;
	mlx_image_t		*img;
	mlx_t			*window;
}					t_rt;

int					parse_float_with_range(char *str, float *f, float min, float max);
char				**ft_split_whitespace(char *str);
int					parsing_scene(char *av, t_rt *scene);
int					pars_int(char *input);
int					parse_ratio(char *ratio, float *r, int check_range);
int					parse_field_of_view(char *fov_str, t_camera *camera);
int					parse_dir(char *dir, t_vec3 *vec);
int					parse_color(char *color, t_color *rgb);
int					parse_cordinates(char *cordi, t_vec3 *vec);
int					parse_float(char *str, float *f);
void				ft_free_split(char **arr);
void				add_obj(t_rt *scene, t_obj_type type, t_obj_data data);
int					parse_sphere(char **line, t_rt *scene);
int					parse_plane(char **line, t_rt *scene);
int					parse_cylinder(char **line, t_rt *scene);
int					parse_light(char **line, t_rt *scene);
int					parse_camera(char **line, t_rt *scene);
int					parse_ambient(char **line, t_rt *scene);
int					check_rt_extension(char *filename);

#endif
