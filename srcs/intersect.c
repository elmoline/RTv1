/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/20 14:54:31 by evogel            #+#    #+#             */
/*   Updated: 2019/10/08 16:11:03 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int        cylinder_intersect(t_ray *r, t_obj *s, float *t)
{
	float    A;
	float    B;
	float    C;
	float    discr;

	/* https://mrl.nyu.edu/~dzorin/rend05/lecture2.pdf page 2
http://woo4.me/wootracer/cylinder-intersection/ */

	t_vec temp;
	temp = r->dir;
	r->dir = rotate_full(r->dir, s->rot);

	t_vec p0 = sub_vec(r->ori, s->pos);
	p0 = rotate_full(p0, s->rot);

	A = (r->dir.x * r->dir.x) + (r->dir.z * r->dir.z);
	B = 2 * (r->dir.x * p0.x) + 2 * (r->dir.z * p0.z);
	C = (p0.x * p0.x) + (p0.z * p0.z) - (s->rad * s->rad);

	r->dir = temp;

	discr = B * B - 4 * A * C;
	if (discr >= 0)
	{
		float t0;
		float t1;

		t0 = (-B + sqrtf(discr))/(2 * A);
		t1 = (-B - sqrtf(discr))/(2 * A);

		/* We want the closest one */
		if (t0 > t1)
			t0 = t1;

		/* Verify t1 larger than 0 and less than the original t */
		if ((t0 > 0.001f) && (t0 < *t))
		{
			*t = t0;
			return (1);
		}
	}
	return (0);
}

int		sphere_intersect(t_ray *r, t_obj *s, float *t)
{

	/* We need a vector representing the distance between the ori of 
	 * the ray and the position of the circle.
	 * This is the term (p0 - c) 
	 */
	t_vec dist;
	dist = sub_vec(r->ori, s->pos);

	/* A = d.d, the vector dot product of the direction */
	float A; 
	A = dot(r->dir, r->dir); 

	/* 2d.(p0 - c) */  
	float B;
	B = 2 * dot(r->dir, dist);

	/* (p0 - c).(p0 - c) - r^2 */
	float C;
	C = dot(dist, dist) - (s->rad * s->rad);

	/* Solving the discriminant */
	float discr;
	discr = B * B - 4 * A * C;

	/* If the discriminant is negative, there are no real roots.
	 * Return false in that case as the ray misses the sphere.
	 * Return true in all other cases (can be one or two intersections)
	 * t represents the distance between the start of the ray and
	 * the point on the sphere where it intersects.
	 */
	if (discr < 0)
		return (0);
	else
	{
		float t0;
		float t1;

		t0 = (-B + sqrtf(discr))/2;
		t1 = (-B - sqrtf(discr))/2;

		/* We want the closest one */
		if (t0 > t1)
			t0 = t1;

		/* Verify t1 larger than 0 and less than the original t */
		if ((t0 > 0.001f) && (t0 < *t))
		{
			*t = t0;
			return (1);
		}
	}
	return (0);
}

int        plane_intersect(t_ray *r, t_obj *s, float *t)
{
	int ret = 0;
	float    A;
	float    B;
	float    t0;
	float epsilon;

	epsilon = 0.0001f;
	t_vec p0;
	p0 = sub_vec(s->pos, r->ori);
	A = dot(p0, s->rot);
	B = dot(r->dir, s->rot);
	if (B <= epsilon)
	{
		ret = 0;
	}
	else
	{
		t0 = A / B;
		if (t0 > epsilon && t0 < *t)
		{
			*t = t0;
			ret = 1;
		}
	}
	return (ret);
}

int cone_intersect(t_ray *r, t_obj *s, float *t)
{
    float    epsilon;
    int        ret;
    ret = 0;
    epsilon = 0.00001f;

	float angle;
	angle = 1 - (s->rad / M_PI);

    t_vec p0;
    p0 = sub_vec(r->ori, s->pos);

	t_vec rot = vec(0, -1, 0);
	rot = rotate_full(rot, s->rot);

    /* A = d.d, the vector dot product of the direction */
    float A;
    A = dot(r->dir, rot) * dot(r->dir, rot) - angle * angle;
    
    /* 2d.(p0 - c) */
    float B;
    B = 2 * ((dot(r->dir, rot) * dot(p0, rot)) - dot(r->dir, p0) * angle * angle);
    
    /* (p0 - c).(p0 - c) - r^2 */
    float C;
    C = dot(p0, rot) * dot(p0, rot) - dot(p0, p0) * angle * angle;
    
    /* Solving the discriminant */
    float discr;
    discr = B * B - 4 * A * C;
    
    /* If the discriminant is negative, there are no real roots.
     * Return false in that case as the ray misses the sphere.
     * Return true in all other cases (can be one or two intersections)
     * t represents the distance between the start of the ray and
     * the point on the sphere where it intersects.
     */
    if (discr < 0)
        ret = 0;
    else
    {
        float sqrtdiscr;
        float t0;
        float t1;
        sqrtdiscr = sqrtf(discr);
        t0 = (-B + sqrtdiscr)/(2 * A);
        t1 = (-B - sqrtdiscr)/(2 * A);
        
        /* We want the closest one */
        if (t0 > t1)
            t0 = t1;
        /* Verify t1 larger than 0 and less than the original t */
        if ((t0 > 0.001f) && (t0 < *t))
        {
            *t = t0;
            ret = 1;
        }
        else
            ret = 0;
    }
return (ret);
}
