/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 14:38:14 by evogel            #+#    #+#             */
/*   Updated: 2019/10/15 20:05:57 by wael-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec		get_axe(t_vec rot)
{
	t_vec axe;
	
	rot.x = deg2rad(rot.x);
	rot.y = deg2rad(rot.y);
	rot.z = deg2rad(rot.z);
	axe = vec(0, -1, 0);
	axe = rotate_full(axe, rot);
	return (normalize(axe));
}

void			init_scene(t_env *env)
{
	env->win_x = WIN_X;
	env->win_y = WIN_Y;

	env->cam.pos = vec(-400, 1000, 2500);
	env->cam.rot = vec(deg2rad(-20), deg2rad(-20), deg2rad(0));
	env->cam.fov = 30;

	env->ambient = 0.2f;
	env->num_light = 2;
	if (!(env->lights = (t_light *)ft_memalloc(env->num_light * sizeof(t_light))))
		exit(-1);

	env->lights[0].pos = vec(400, 700, 1000);
	env->lights[0].col = color(0.5, 0.5, 0.5);
	env->lights[1].pos = vec(-600, 300, 100);
	env->lights[1].col = color(0, 0.8, 0.8);

	env->num_obj = 5;
	if (!(env->objs = (t_obj *)ft_memalloc(env->num_obj * sizeof(t_obj))))
		exit(-1);

	env->objs[0].type = 0;
	env->objs[0].pos = vec(0, 0, 0);
	env->objs[0].rad = 0;
	env->objs[0].rot = get_axe(vec(deg2rad(0), deg2rad(0), deg2rad(0)));
	env->objs[0].col = color(1, 0.2, 0.2);
	env->objs[0].reflect = 0;

	env->objs[1].type = 0;
	env->objs[1].pos = vec(0, 0, -400);
	env->objs[1].rad = 0;
	env->objs[1].rot = get_axe(vec(deg2rad(90), deg2rad(0), deg2rad(0)));
	env->objs[1].col = color(0, 0.6, 1);
	env->objs[1].reflect = 0;

	env->objs[2].type = 3;
	env->objs[2].pos = vec(0, 400, 150);
	env->objs[2].rad = deg2rad(20);
	env->objs[2].rot = get_axe(vec(deg2rad(0), deg2rad(0), deg2rad(20)));
	env->objs[2].col = color(1, 1, 0);
	env->objs[2].reflect = 0;

	env->objs[3].type = 2;
	env->objs[3].pos = vec(400, 300, 400);
	env->objs[3].rad = 60;
	env->objs[3].rot = get_axe(vec(deg2rad(90), deg2rad(0), deg2rad(0)));
	env->objs[3].col = color(1, 0, 1);
	env->objs[3].reflect = 0;

	env->objs[4].type = 1;
	env->objs[4].pos = vec(500, 400, 0);
	env->objs[4].rad = 120;
	env->objs[4].col = color(1, 0.5, 0);
	env->objs[4].reflect = 0;
}

int				window_init(t_mlx *mlx, int win_x, int win_y)
{
	int	tmp;

	mlx->mlx_ptr = mlx_init();
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr, win_x, win_y, "RTv1");
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, win_x, win_y);
	mlx->data = (int *)mlx_get_data_addr(mlx->img_ptr, &tmp, &tmp, &tmp);
	if (!mlx->mlx_ptr || !mlx->win_ptr || !mlx->img_ptr || !mlx->data)
		return (-1);
	return (0);
}

int				main(int ac, char **av)
{
	t_env	env;

	if (ac != 2)
		return (ft_printf("usage:\n"));
	//PARSING GOES HERE
	//following is temp
	// (void)av;
	// init_scene(&env);
	parsing(av, &env);
	//create error1() for parsing errors
	//end temp
	if (window_init(&env.mlx, env.win_x, env.win_y) == -1)
		error2();
	render(&env);
	mlx_hook(env.mlx.win_ptr, 17, (1L << 17), deal_close, NULL);
	mlx_hook(env.mlx.win_ptr, 2, 0, deal_key, &env);
	mlx_loop(env.mlx.mlx_ptr);
	return (0);
}
