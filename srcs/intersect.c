/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/20 14:54:31 by evogel            #+#    #+#             */
/*   Updated: 2019/10/14 16:11:43 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int	equation_solver(float a, float b, float c, float *t)
{
	float	t0;
	float	t1;
	float	discr;

	discr = sqrtf(b * b - 4 * a * c);
	if (discr < 0.0f)
		return (0);
	else
	{
		t0 = (-b + discr) / (2 * a);
		t1 = (-b - discr) / (2 * a);
		if (t0 > t1)
			t0 = t1;
		if ((t0 > 0.0f) && (t0 < *t))
		{
			*t = t0;
			return (1);
		}
	}
	return (0);
}

int			cylinder_intersect(t_ray *r, t_obj *s, float *t)
{
	t_vec	cross1;
	t_vec	cross2;
	float	a;
	float	b;
	float	c;

	cross1 = cross(sub_vec(r->ori, s->pos), s->rot);
	cross2 = cross(r->dir, s->rot);
	a = dot(cross2, cross2);
	b = 2 * dot(cross2, cross1);
	c = dot(cross1, cross1) - (s->rad * s->rad * dot(s->rot, s->rot));
	return (equation_solver(a, b, c, t));
}

int			sphere_intersect(t_ray *r, t_obj *s, float *t)
{
	t_vec	dist;
	float	a;
	float	b;
	float	c;

	dist = sub_vec(r->ori, s->pos);
	a = dot(r->dir, r->dir);
	b = 2 * dot(r->dir, dist);
	c = dot(dist, dist) - (s->rad * s->rad);
	return (equation_solver(a, b, c, t));
}

int			plane_intersect(t_ray *r, t_obj *s, float *t)
{
	float	a;
	float	b;
	float	t0;

	a = dot(sub_vec(s->pos, r->ori), s->rot);
	b = dot(r->dir, s->rot);
	if (b < 0.0f)
		return (0);
	else
	{
		t0 = a / b;
		if (t0 > 0.0f && t0 < *t)
		{
			*t = t0;
			return (1);
		}
	}
	return (0);
}

int			cone_intersect(t_ray *r, t_obj *s, float *t)
{
	float	angle;
	float	a;
	float	b;
	float	c;
	t_vec	p0;

	angle = 1 - (s->rad / M_PI);
	p0 = sub_vec(r->ori, s->pos);
	a = dot(r->dir, s->rot) * dot(r->dir, s->rot) - angle * angle;
	b = 2 * ((dot(r->dir, s->rot) * dot(p0, s->rot)) - dot(r->dir, p0) \
		* angle * angle);
	c = dot(p0, s->rot) * dot(p0, s->rot) - dot(p0, p0) * angle * angle;
	return (equation_solver(a, b, c, t));
}
