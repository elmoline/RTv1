/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/19 16:37:45 by evogel            #+#    #+#             */
/*   Updated: 2019/09/05 15:58:12 by evogel           ###   ########.fr       */
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

int		cast_ray(t_env *env, t_ray *ray)
{
	float t = 2000000000.0f; //view limit
	int j = 0;
	t_col col;
	t_obj *curr_obj;

	if (!(curr_obj = get_obj_intersect(ray, env, &t)))
		return (0);
//	else
//		col = curr_obj->col;
	col = color(0, 0, 0);

	t_vec p_hit;
	p_hit = add_vec(ray->ori, scale(t, ray->dir));
	p_hit.z -= 0.2f;

	t_vec n_hit;
	//THIS n_hit ONLY WORKS FOR SPHERES
	//Have to make different n calc for shapes
	if (curr_obj->type == 0)
		n_hit = scale(-1, curr_obj->rot);
	else if (curr_obj->type == 1)
		n_hit = normalize(sub_vec(p_hit, curr_obj->pos));
	else if (curr_obj->type == 2)
		n_hit = normalize(sub_vec(p_hit, vec(curr_obj->pos.x, p_hit.y, curr_obj->pos.z)));
	else if (curr_obj->type == 3)
		n_hit = normalize(vec((p_hit.x - curr_obj->pos.x) / curr_obj->rad, curr_obj->rad, (p_hit.z - curr_obj->pos.z) / curr_obj->rad));
	else
		n_hit = normalize(sub_vec(p_hit, curr_obj->pos));

	while (j < env->num_light)
	{
		t_ray light_ray;
		light_ray.ori = p_hit;
		light_ray.dir = normalize(sub_vec(env->lights[j].pos, p_hit));
		
		t_obj *shadow_obj;
		float tmp = 2000000000000000.0f;
		if ((shadow_obj = get_obj_intersect(&light_ray, env, &tmp)) == NULL
				|| dot(sub_vec(sub_vec(env->lights[j].pos, p_hit), add_vec(p_hit, scale(tmp, light_ray.dir))), vec(1, 1, 1)) < 0)
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
		else if (p_hit.x > 0 && p_hit.y == -100 && p_hit.z > 0)
		{
			ft_printf("p_hit(%.1f, %.1f, %.1f)\n", p_hit.x, p_hit.y, p_hit.z);
			ft_printf("light dir (%.2f, %.2f, %.2f)\n", light_ray.dir.x, light_ray.dir.y, light_ray.dir.z);
			t_vec l_hit;
			l_hit = add_vec(p_hit, scale(tmp, light_ray.dir));
			ft_printf("light hit (%.2f, %.2f, %.2f)\n", l_hit.x, l_hit.y, l_hit.z);
			ft_printf("tmp = %f\n", tmp);
		}
		++j;
	}
	
//	if (col.red + col.green + col.blue == 0)
//		ft_printf("obj_type == %d | col = (%.2f,%.2f,%.2f)\n", curr_obj->type, col.red, col.green, col.blue);
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
			//HERE ADD ROTATION vector to xx yy and -1
			ray.dir = normalize(vec(xx, yy, -1));
			env->mlx.data[x + y * env->win_x] = cast_ray(env, &ray);
			++x;
		}
		++y;
	}
	mlx_put_image_to_window(env->mlx.mlx_ptr, env->mlx.win_ptr, env->mlx.img_ptr, 0, 0);
	return (0);
}
