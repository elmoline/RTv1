/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 17:45:00 by evogel            #+#    #+#             */
/*   Updated: 2019/11/04 13:09:33 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

float	shadows(t_env *env, t_light *light, t_obj *obj)
{
	t_ray	light_ray;
	t_obj	*shadow_obj;
	float	d1;
	float	d2;
	float	res;

	res = 0.0f;
	d1 = 0.0f;
	d2 = 0.0f;
	light_ray.ori = add_vec(obj->hit.ori, scale(0.02f, obj->hit.dir));
	light_ray.dir = normalize(sub_vec(light->pos, obj->hit.ori));
	if ((shadow_obj = get_obj_intersect(env, &light_ray)) != NULL)
	{
		d1 = magnitude(sub_vec(shadow_obj->hit.ori, obj->hit.ori));
		d2 = magnitude(sub_vec(light->pos, obj->hit.ori));
	}
	if (shadow_obj == NULL || shadow_obj == obj || d1 > d2)
		res = dot(obj->hit.dir, light_ray.dir);
	return (res < 0.0f ? 0.0f : res);
}

t_vec	reflect(t_vec i, t_vec n)
{
	float	reflect;
	t_vec	tmp;

	reflect = 2.0f * dot(n, i);
	tmp = scale(reflect, n);
	return (sub_vec(tmp, i));
}

float	shine(t_light *light, t_obj *obj, t_ray *ray)
{
	t_vec l_dir;
	float res;

	if (obj->type == 0)
		return (0.0f);
	l_dir = normalize(sub_vec(obj->hit.ori, light->pos));
	l_dir = reflect(l_dir, obj->hit.dir);
	res = pow(fmax(dot(l_dir, normalize(ray->dir)), 0.0), 40);
	return (res < 0.0f ? 0.0f : res);
}

int		cast_ray(t_env *env, t_ray *ray)
{
	int		j;
	t_col	col;
	t_obj	*curr_obj;
	float	res;

	if (!(curr_obj = get_obj_intersect(env, ray)))
		return (0);
	col = color(0, 0, 0);
	add_color(&col, &curr_obj->col, env->ambient);
	j = 0;
	while (j < env->num_light)
	{
		res = shadows(env, &env->lights[j], curr_obj);
		res *= 1 - env->ambient;
		add_color(&col, &curr_obj->col, res);
		res = shine(&env->lights[j], curr_obj, ray);
		add_color(&col, &env->lights[j].col, res);
		++j;
	}
	col.r = fmin(col.r * 255.0f, 255.0f);
	col.g = fmin(col.g * 255.0f, 255.0f);
	col.b = fmin(col.b * 255.0f, 255.0f);
	return (((uint8_t)col.r << 16) | ((uint8_t)col.g << 8) | ((uint8_t)col.b));
}
