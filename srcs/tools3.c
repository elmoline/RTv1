/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evogel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 13:59:55 by evogel            #+#    #+#             */
/*   Updated: 2019/10/15 14:19:21 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static t_vec	mul_vec_mat(t_vec vec, t_matrix mat)
{
	t_vec	ret;

	ret.x = mat.row0.x * vec.x + mat.row1.x * vec.y + mat.row2.x * vec.z;
	ret.x += mat.row3.x * 1;
	ret.y = mat.row0.y * vec.x + mat.row1.y * vec.y + mat.row2.y * vec.z;
	ret.y += mat.row3.y * 1;
	ret.z = mat.row0.z * vec.x + mat.row1.z * vec.y + mat.row2.z * vec.z;
	ret.z += mat.row3.z * 1;
	return (ret);
}

t_vec			rotate_x(t_vec pt, double theta)
{
	t_matrix	ret;

	ret.row0 = vec4(1, 0, 0, 0);
	ret.row1 = vec4(0, cos(theta), sin(theta), 0);
	ret.row2 = vec4(0, -sin(theta), cos(theta), 0);
	ret.row3 = vec4(0, 0, 0, 1);
	return (mul_vec_mat(pt, ret));
}

t_vec			rotate_y(t_vec pt, double theta)
{
	t_matrix	ret;

	ret.row0 = vec4(cos(theta), 0, -sin(theta), 0);
	ret.row1 = vec4(0, 1, 0, 0);
	ret.row2 = vec4(sin(theta), 0, cos(theta), 0);
	ret.row3 = vec4(0, 0, 0, 1);
	return (mul_vec_mat(pt, ret));
}

t_vec			rotate_z(t_vec pt, double theta)
{
	t_matrix	ret;

	ret.row0 = vec4(cos(theta), sin(theta), 0, 0);
	ret.row1 = vec4(-sin(theta), cos(theta), 0, 0);
	ret.row2 = vec4(0, 0, 1, 0);
	ret.row3 = vec4(0, 0, 0, 1);
	return (mul_vec_mat(pt, ret));
}

t_vec			rotate_full(t_vec ori, t_vec rot)
{
	t_vec	res;

	res = ori;
	res = rotate_z(res, rot.z);
	res = rotate_x(res, rot.x);
	res = rotate_y(res, rot.y);
	return (res);
}
