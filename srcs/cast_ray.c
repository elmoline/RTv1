/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evogel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 17:45:00 by evogel            #+#    #+#             */
/*   Updated: 2019/10/14 17:46:24 by evogel           ###   ########.fr       */
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

int		cast_ray(t_env *env, t_ray *ray)
{
	float	t;
	int		j;
	t_col	col;
	t_obj	*curr_obj;

	t = 1000000000000.0f;
	if (!(curr_obj = get_obj_intersect(ray, env, &t)))
		return (0);
	col = color(0, 0, 0);

	t_vec p_hit;
	p_hit = add_vec(ray->ori, scale(t, ray->dir));

	t_vec n_hit;
	n_hit = get_hit_point_normal(p_hit, curr_obj);

	j = 0;
	while (j < env->num_light)
	{
		t_ray light_ray;
		light_ray.ori = add_vec(p_hit, scale(0.05f, n_hit));
		light_ray.dir = normalize(sub_vec(env->lights[j].pos, p_hit));
		
		t = 1000000000000000.0f;
		t_obj *shadow_obj;
		shadow_obj = get_obj_intersect(&light_ray, env, &t);

		float d1, d2;
		d1 = sqrt(dot(sub_vec(env->lights[j].pos, p_hit), sub_vec(env->lights[j].pos, p_hit)));
		d2 = sqrt(dot(scale(t, light_ray.dir), scale(t, light_ray.dir)));

		if (shadow_obj == NULL || shadow_obj == curr_obj || d2 > d1)
		{
			float shade;
			shade = dot(n_hit, light_ray.dir);
			if (shade < 0.0f)
				shade = 0.0f;
			float diff = 1 - env->ambient;
			col.red += (diff * shade * env->lights[j].col.red + env->ambient) * curr_obj->col.red;
			col.green += (diff * shade * env->lights[j].col.green + env->ambient) * curr_obj->col.green;
			col.blue += (diff * shade * env->lights[j].col.blue + env->ambient) * curr_obj->col.blue;
		}
		else
		{
			col.red += env->ambient * curr_obj->col.red;
			col.green += env->ambient * curr_obj->col.green;
			col.blue += env->ambient * curr_obj->col.blue;
		}
		++j;
	}
	col.red *= 255.0f;
	col.red = (col.red > 255.0f ? 255.0f : col.red);
	col.green *= 255.0f;
	col.green = (col.green > 255.0f ? 255.0f : col.green);
	col.blue *= 255.0f;
	col.blue = (col.blue > 255.0f ? 255.0f : col.blue);
	return (((unsigned char)col.red << 16) | ((unsigned char)col.green << 8) | ((unsigned char)col.blue));
}
