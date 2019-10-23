/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/08 16:51:38 by evogel            #+#    #+#             */
/*   Updated: 2019/10/22 14:31:27 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec	vec(float x, float y, float z)
{
	t_vec	res;

	res.x = x;
	res.y = y;
	res.z = z;
	return (res);
}

t_vec4	vec4(float x, float y, float z, float w)
{
	t_vec4	res;

	res.x = x;
	res.y = y;
	res.z = z;
	res.w = w;
	return (res);
}

float	magnitude(t_vec v)
{
	return (sqrt(dot(v, v)));
}

t_vec	normalize(t_vec v)
{
	t_vec	res;
	float	mag;

	res = v;
	mag = magnitude(v);
	if (mag > 0)
		res = scale(1 / mag, v);
	return (res);
}

float	deg2rad(int d)
{
	return (d * M_PI / 180);
}
