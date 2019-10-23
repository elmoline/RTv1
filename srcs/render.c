/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/19 16:37:45 by evogel            #+#    #+#             */
/*   Updated: 2019/10/22 14:51:58 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void	pixel2camera(float *xx, float *yy, int n, t_env *env)
{
	static float angle = -1;
	static float inv_width = -1;
	static float inv_height = -1;
	static float aspectratio = -1;

	if (aspectratio < 0)
	{
		angle = tan(env->cam.fov * 0.5 * M_PI / 180);
		inv_width = 1 / (float)env->win_x;
		inv_height = 1 / (float)env->win_y;
		aspectratio = env->win_x / (float)env->win_y;
	}
	if (xx != NULL)
		*xx = (2 * ((n + 0.5) * inv_width) - 1) * angle * aspectratio;
	else if (yy != NULL)
		*yy = (1 - 2 * ((n + 0.5) * inv_height)) * angle;
}

int			render(t_env *env)
{
	int		x;
	int		y;
	float	xx;
	float	yy;
	t_ray	ray;

	y = 0;
	while (y < env->win_y)
	{
		pixel2camera(NULL, &yy, y, env);
		x = 0;
		while (x < env->win_x)
		{
			pixel2camera(&xx, NULL, x, env);
			ray.ori = env->cam.pos;
			ray.dir = normalize(vec(xx, yy, -1));
			ray.dir = rotate_full(ray.dir, env->cam.rot);
			env->mlx.data[x + y * env->win_x] = cast_ray(env, &ray);
			++x;
		}
		++y;
	}
	mlx_put_image_to_window(env->mlx.mlx_ptr, env->mlx.win_ptr, \
		env->mlx.img_ptr, 0, 0);
	return (0);
}
