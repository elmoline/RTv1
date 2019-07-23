/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evogel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/08 16:51:38 by evogel            #+#    #+#             */
/*   Updated: 2019/07/23 14:50:44 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec	vec(float x, float y, float z)
{
        t_vec  res;

        res.x = x;
        res.y = y;
        res.z = z;
        return (res);
}

t_col	color(float red, float green, float blue)
{
        t_col  res;

        res.red = red;
        res.green = green;
        res.blue = blue;
        return (res);
}

/* Subtract two vectors and return the resulting vector */
t_vec sub_vec(t_vec v1, t_vec v2)
{
	t_vec res;
	
	res.x = v1.x - v2.x;
	res.y = v1.y - v2.y;
	res.z = v1.z - v2.z;
	return (res);
}

/* Multiply two vectors and return the resulting scalar (dot product) */
float dot(t_vec v1, t_vec v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

/* Calculate Vector x Scalar and return resulting Vector*/ 
t_vec scale(float c, t_vec v)
{
    t_vec res;
	
	res.x = v.x * c;
	res.y = v.y * c;
	res.z = v.z * c;
	return (res);
}

/* Add two vectors and return the resulting vector */
t_vec add_vec(t_vec v1, t_vec v2)
{
    t_vec res;
	
	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	res.z = v1.z + v2.z;
	return (res);
}

/* Normalize a vector */
t_vec normalize(t_vec v)
{
    t_vec	res;
	float	mag;
	
	res = v;
	mag = sqrtf(dot(v, v));
	if (mag > 0)
		res = scale(1 / mag, v);
	return (res);
}

int			deal_key(int key, void *param)
{
	t_env *env;

	env = (t_env *)param;
	if (key == 53)
		exit(1);
	else if (key == 36)
		write_ppm(key, env->mlx.data, env->win_x, env->win_y);
	return (0);
}

int			deal_close(void)
{
	exit(1);
	return (0);
}

int			error2(void)
{
	ft_printf("Error creating window with MLX\n");
	exit(-1);
}
