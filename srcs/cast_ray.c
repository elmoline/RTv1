/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evogel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 17:45:00 by evogel            #+#    #+#             */
/*   Updated: 2019/10/15 16:52:31 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_obj	*get_obj_intersect(t_ray *ray, t_env *env, float *t)
{
	int	closest;
	int	i;

	closest = -1;
	i = 0;
	while (i < env->num_obj)
	{
		if (env->objs[i].type == 0 && plane_intersect(ray, &env->objs[i], t))
			closest = i;
		if (env->objs[i].type == 1 && sphere_intersect(ray, &env->objs[i], t))
			closest = i;
		if (env->objs[i].type == 2 && cylinder_intersect(ray, &env->objs[i], t))
			closest = i;
		if (env->objs[i].type == 3 && cone_intersect(ray, &env->objs[i], t))
			closest = i;
		++i;
	}
	if (closest == -1)
		return (NULL);
	return (&env->objs[closest]);
}

t_vec	get_hit_point_normal(t_vec p_hit, t_obj *obj)
{
	t_vec	n_hit;
	t_vec	tmp;

	n_hit = vec(0, 1, 0);
	if (obj->type == 0)
		n_hit = scale(-1, obj->rot);
	else if (obj->type == 1)
		n_hit = normalize(sub_vec(p_hit, obj->pos));
	else if (obj->type == 2)
	{
		n_hit = cross(cross(obj->rot, sub_vec(p_hit, obj->pos)), obj->rot);
		n_hit = normalize(n_hit);
	}
	else if (obj->type == 3)
	{
		tmp = sub_vec(p_hit, obj->pos);
		if (dot(obj->rot, tmp) < 0.0f)
			obj->rot = scale(-1, obj->rot);
		n_hit = scale(dot(obj->rot, tmp) / dot(tmp, tmp), tmp);
		n_hit = normalize(sub_vec(n_hit, obj->rot));
	}
	return (n_hit);
}

float	light_ray(t_env *env, t_light *light, t_vec p_hit, t_obj *obj)
{
	t_vec	n_hit;
	t_ray	light_ray;
	t_obj	*shadow_obj;
	float	t0;
	float	t1;

	t0 = 1000000000000000.0f;
	n_hit = get_hit_point_normal(p_hit, obj);
	light_ray.ori = add_vec(p_hit, scale(0.05f, n_hit));
	light_ray.dir = normalize(sub_vec(light->pos, p_hit));
	shadow_obj = get_obj_intersect(&light_ray, env, &t0);
	t0 = magnitude(scale(t0, light_ray.dir));
	t1 = magnitude(sub_vec(light->pos, p_hit));
	if (shadow_obj == NULL || shadow_obj == obj || t0 > t1)
	{
		t0 = dot(n_hit, light_ray.dir);
		return (t0 < 0.0f ? 0.0f : t0);
	}
	return (0.0f);
}

int		cast_ray(t_env *env, t_ray *ray)
{
	int		j;
	float	t;
	t_col	col;
	t_obj	*curr_obj;
	t_vec	p_hit;

	t = 1000000000000.0f;
	if (!(curr_obj = get_obj_intersect(ray, env, &t)))
		return (0);
	col = color(0, 0, 0);
	p_hit = add_vec(ray->ori, scale(t, ray->dir));
	j = 0;
	while (j < env->num_light)
	{
		t = light_ray(env, &env->lights[j], p_hit, curr_obj);
		t *= 1 - env->ambient;
		col.r += (env->ambient + t * env->lights[j].col.r) * curr_obj->col.r;
		col.g += (env->ambient + t * env->lights[j].col.g) * curr_obj->col.g;
		col.b += (env->ambient + t * env->lights[j].col.b) * curr_obj->col.b;
		++j;
	}
	col.r = (col.r * 255.0f > 255.0f ? 255.0f : col.r * 255.0f);
	col.g = (col.g * 255.0f > 255.0f ? 255.0f : col.g * 255.0f);
	col.b = (col.b * 255.0f > 255.0f ? 255.0f : col.b * 255.0f);
	return (((uint8_t)col.r << 16) | ((uint8_t)col.g << 8) | ((uint8_t)col.b));
}
