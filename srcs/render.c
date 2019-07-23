/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evogel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/19 16:37:45 by evogel            #+#    #+#             */
/*   Updated: 2019/07/23 14:50:41 by evogel           ###   ########.fr       */
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
		if (env->objs[i].type == 1 && sphere_intersect(ray, &env->objs[i], t))
			closest = i;
		if (env->objs[i].type == 2 && cylinder_intersect(ray, &env->objs[i], t))
			closest = i;
		//add check for other types of objects
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
	t_col color;
	t_obj *curr_obj;

	if (!(curr_obj = get_obj_intersect(ray, env, &t)))
		return (0);
	color.red = 0;
	color.green = 0;
	color.blue = 0;

	t_vec p_hit;
	p_hit = add_vec(ray->ori, scale(t, ray->dir));

	t_vec n_hit;
	n_hit = normalize(sub_vec(p_hit, curr_obj->pos));

	/* Shadow pseudocode 
	 *
	 * shade = dot_product( light_vector, normal_vector )
	 * if ( shade < 0 )
	 * shade = 0
	 * point_color = object_color * ( ambient_coefficient + 
	 * diffuse_coefficient * shade )
	 */

	while (j < env->num_light)
	{
		t_ray light_ray;
		light_ray.ori = p_hit;
		light_ray.dir = normalize(sub_vec(env->lights[j].pos, p_hit));
		
		if (!get_obj_intersect(&light_ray, env, &t))
		{
			float shade;
			shade = dot(light_ray.dir, n_hit);
			if (shade < 0.0f)
				shade = 0.0f;
			float diff = 1 - env->ambient;
			color.red += (diff * shade * env->lights[j].col.red + env->ambient) * curr_obj->col.red;
			color.green += (diff * shade * env->lights[j].col.green + env->ambient) * curr_obj->col.green;
			color.blue += (diff * shade * env->lights[j].col.blue + env->ambient) * curr_obj->col.blue;
		}
		++j;
	}
	return (((unsigned char)min(color.red * 255.0f, 255.0f) << 16) | ((unsigned char)min(color.green * 255.0f, 255.0f) << 8) | ((unsigned char)min(color.blue * 255.0f, 255.0f)));
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
