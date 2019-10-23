/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evogel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 14:25:35 by evogel            #+#    #+#             */
/*   Updated: 2019/10/22 14:26:33 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_obj	*get_obj_intersect(t_env *env, t_ray *ray)
{
	t_obj	*curr_obj;
	float	t;
	int		closest;
	int		i;

	t = FLT_MAX;
	closest = -1;
	i = 0;
	while (i < env->num_obj)
	{
		curr_obj = &env->objs[i];
		if (curr_obj->type == 0 && plane_intersect(ray, curr_obj, &t))
			closest = i;
		if (curr_obj->type == 1 && sphere_intersect(ray, curr_obj, &t))
			closest = i;
		if (curr_obj->type == 2 && cylinder_intersect(ray, curr_obj, &t))
			closest = i;
		if (curr_obj->type == 3 && cone_intersect(ray, curr_obj, &t))
			closest = i;
		++i;
	}
	if (closest == -1)
		return (NULL);
	return (&env->objs[closest]);
}

t_vec	get_phit_normal(t_vec p_hit, t_obj *obj)
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
