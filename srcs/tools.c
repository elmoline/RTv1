/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/08 16:51:38 by evogel            #+#    #+#             */
/*   Updated: 2019/10/10 16:01:19 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

float	deg2rad(int d)
{
	return (d * M_PI / 180);
}

t_vec	vec(float x, float y, float z)
{
        t_vec  res;

        res.x = x;
        res.y = y;
        res.z = z;
        return (res);
}

t_vec4	vec4(float x, float y, float z, float w)
{
        t_vec4  res;

        res.x = x;
        res.y = y;
        res.z = z;
        res.w = w;
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

t_vec cross(t_vec v1, t_vec v2)
{
    t_vec res;
	
	res.x = v1.y * v2.z - v1.z * v2.y;
	res.y = v1.z * v2.x - v1.x * v2.z;
	res.z = v1.x * v2.y - v1.y * v2.x;
	return (res);
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

float magnitude(t_vec v)
{
	return (sqrt(dot(v, v)));
}

/* Normalize a vector */
t_vec normalize(t_vec v)
{
    t_vec	res;
	float	mag;
	
	res = v;
	mag = magnitude(v);
	if (mag > 0)
		res = scale(1 / mag, v);
	return (res);
}

t_vec		mul_vec_mat(t_vec vec, t_matrix mat)
{
	t_vec	ret;

	ret.x = mat.row0.x * vec.x + mat.row1.x * vec.y + mat.row2.x * vec.z + mat.row3.x * 1;
	ret.y = mat.row0.y * vec.x + mat.row1.y * vec.y + mat.row2.y * vec.z + mat.row3.y * 1;
	ret.z = mat.row0.z * vec.x + mat.row1.z * vec.y + mat.row2.z * vec.z + mat.row3.z * 1;
	return (ret);
}

t_vec	rotate_x(t_vec pt, double theta)
{
	t_matrix ret;

	ret.row0 = vec4(1, 0, 0, 0);
	ret.row1 = vec4(0, cos(theta), sin(theta), 0);
	ret.row2 = vec4(0, -sin(theta), cos(theta), 0);
	ret.row3 = vec4(0, 0, 0, 1);
	return (mul_vec_mat(pt, ret));
}

t_vec	rotate_y(t_vec pt, double theta)
{
	t_matrix ret;

	ret.row0 = vec4(cos(theta), 0, -sin(theta), 0);
	ret.row1 = vec4(0, 1, 0, 0);
	ret.row2 = vec4(sin(theta), 0, cos(theta), 0);
	ret.row3 = vec4(0, 0, 0, 1);
	return (mul_vec_mat(pt, ret));
}

t_vec	rotate_z(t_vec pt, double theta)
{
	t_matrix ret;

	ret.row0 = vec4(cos(theta), sin(theta), 0, 0);
	ret.row1 = vec4(-sin(theta), cos(theta), 0, 0);
	ret.row2 = vec4(0, 0, 1, 0);
	ret.row3 = vec4(0, 0, 0, 1);
	return (mul_vec_mat(pt, ret));
}

t_vec	rotate_full(t_vec ori, t_vec rot)
{
	t_vec res;

	res = ori;
	res = rotate_z(res, rot.z);
	res = rotate_x(res, rot.x);
	res = rotate_y(res, rot.y);
	return (res);
}


int			deal_key(int key, void *param)
{
	t_env *env;

	env = (t_env *)param;
	if (key == 53)
		exit(1);
	else if (key == 36)
		write_ppm(key, env);
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
