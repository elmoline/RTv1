/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/19 16:37:45 by evogel            #+#    #+#             */
/*   Updated: 2019/10/10 16:43:39 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_obj	*get_obj_intersect(t_ray *ray, t_env *env, float *t)
{
	int i;
	int closest;

	i = 0;
	closest = -1;
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
	t_vec n_hit;

	n_hit = vec(0, 0, 0);
	if (obj->type == 0)
		n_hit = scale(-1, obj->rot);
	else if (obj->type == 1)
		n_hit = normalize(sub_vec(p_hit, obj->pos));
	else if (obj->type == 2)
		n_hit = normalize(cross(cross(obj->rot, sub_vec(p_hit, obj->pos)), obj->rot));
	else if (obj->type == 3)
	{
		t_vec cp = sub_vec(p_hit, obj->pos);
		if (dot(obj->rot, cp) < 0.0f)
			obj->rot = scale(-1, obj->rot);
		n_hit = normalize(sub_vec(scale(dot(obj->rot, cp) / dot(cp, cp), cp), obj->rot));
	}
	return (n_hit);
}

int		cast_ray(t_env *env, t_ray *ray)
{
	float t = 1000000000000000.0f;
	int j = 0;
	t_col col;
	t_obj *curr_obj;

	if (!(curr_obj = get_obj_intersect(ray, env, &t)))
		return (0);
	col = color(0, 0, 0);

	t_vec p_hit;
	p_hit = add_vec(ray->ori, scale(t, ray->dir));

	t_vec n_hit;
	n_hit = get_hit_point_normal(p_hit, curr_obj);

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
	
	return (((unsigned char)min(col.red * 255.0f, 255.0f) << 16) | ((unsigned char)min(col.green * 255.0f, 255.0f) << 8) | ((unsigned char)min(col.blue * 255.0f, 255.0f)));
}

/* Pseudocode for handling reflection/refraction
 *
 * Color trace_ray( Ray original_ray )
 * {
 * 		Color point_color, reflect_color, refract_color
 * 		Object obj 
 *
 * 		obj = get_first_intersection( original_ray )
 * 		point_color = get_point_color( obj )
 *
 * 		if ( object is reflective )
 * 		reflect_color = trace_ray( get_reflected_ray( original_ray, obj ) )
 * 		if ( object is refractive )
 * 		refract_color = trace_ray( get_refracted_ray( original_ray, obj ) )
 *
 * 		return ( combine_colors( point_color, reflect_color, refract_color ))
 * }
 */

int		render(t_env *env)
{
	int x;
	int y;
	float xx;
	float yy;
	t_ray ray;

	float angle = tan(env->cam.fov * 0.5 * M_PI / 180);
	float inv_width = 1 / (float)env->win_x;
	float inv_height = 1 / (float)env->win_y;
	float aspectratio = env->win_x / (float)env->win_y;

	y = 0;
	while (y < env->win_y)
	{
		yy = (1 - 2 * ((y + 0.5) * inv_height)) * angle;
		x = 0;
		while (x < env->win_x)
		{
			xx = (2 * ((x + 0.5) * inv_width) - 1) * angle * aspectratio;
			ray.ori = env->cam.pos;
			ray.dir = normalize(vec(xx, yy, -1));
			ray.dir = rotate_full(ray.dir, env->cam.rot);
			env->mlx.data[x + y * env->win_x] = cast_ray(env, &ray);
			++x;
		}
		++y;
	}
	mlx_put_image_to_window(env->mlx.mlx_ptr, env->mlx.win_ptr, env->mlx.img_ptr, 0, 0);
	return (0);
}
