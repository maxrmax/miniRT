/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 04:23:15 by jpflegha          #+#    #+#             */
/*   Updated: 2025/11/11 05:38:09 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "miniRT.h"

int pars_number(char *input)
{
    int i;

    i = 0;
    while (input[i])
    {
        if(!ft_isdigit(input[i]))
        {
            printf("%s is not a number\n", input);
            return(0);
        }
        i++;
    }
    return(1);
}

int pars_ratio(char *ratio, float *r)
{
    //TODO
    float num;
    if(ft_isfloat(ratio))
        return(0);
    num = ft_atof(ratio);
    if(num < 0 || num > 1)
    {
        printf("%d should be between 0.0 or 1.0", num);
        return(0);
    }
    return(num);
}
int pars_file_of_viepoint(char *fov, t_rt scene)
{
    int num;

    if(pars_number)
        return(0);
    num = ft_atoi(fov);
}

int pars_dir(char *dir, t_rt scene)
{
    
}
int pars_color(char *color, t_color *rgb)
{
    char **split;
    int i;


    i = 0;
    split = ft_split(color, ',');
    if(!split || !rgb)
        return(0);
    while (split[i])
        i++;
    if(i != 3)
    {
        ft_free_split(split);
        printf("There must be exactly 3 values (0–255)\n");
        return(0);
    }
    rgb->r = ft_atof(split[0]);
    rgb->g = ft_atof(split[1]);
    rgb->b = ft_atof(split[1]);
    ft_free_split(split);
    	if (rgb->r < 0 || rgb->r > 255
		|| rgb->g < 0 || rgb->g > 255
		|| rgb->b < 0 || rgb->b > 255)
		return (0);

	return (1);
}
int pars_cordinates(char *cordi, t_rt scene)
{

}