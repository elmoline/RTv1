/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 14:38:14 by evogel            #+#    #+#             */
/*   Updated: 2019/10/10 16:41:55 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	get_plane_n(t_obj *p, t_vec rot)
{
	t_vec n;

	n = vec(0, -1, 0);
	n = rotate_full(n, rot);
	p->rot = normalize(n);
}

void	init_scene(t_env *env)
{
	env->win_x = WIN_X;
	env->win_y = WIN_Y;

	env->cam.pos = vec(100, 600, 4000);
	env->cam.rot = vec(deg2rad(0), deg2rad(0), deg2rad(0));
	env->cam.fov = 30;

	env->ambient = 0.2f;
	env->num_light = 1;
	if (!(env->lights = (t_light *)malloc(env->num_light * sizeof(t_light))))
		exit(-1);

	env->lights[0].pos = vec(400, 700, 1000); 
	env->lights[0].col = color(1, 1, 1); 

	env->num_obj = 4;
	if (!(env->objs = (t_obj *)malloc(env->num_obj * sizeof(t_obj))))
		exit(-1);

	env->objs[0].type = 0; 
	env->objs[0].pos = vec(0, 0, 0);
	env->objs[0].rad = 0;
	get_plane_n(&env->objs[0], vec(deg2rad(0), deg2rad(0), deg2rad(0)));
	env->objs[0].col = color(1, 0.2, 0.2); 
	env->objs[0].reflect = 0;

	env->objs[1].type = 0; 
	env->objs[1].pos = vec(0, 0, -400);
	env->objs[1].rad = 0;
	get_plane_n(&env->objs[1], vec(deg2rad(90), deg2rad(0), deg2rad(0)));
	env->objs[1].col = color(0, 0.6, 1); 
	env->objs[1].reflect = 0;
	
	env->objs[2].type = 3; 
	env->objs[2].pos = vec(-200, 400, 150);
	env->objs[2].rad = deg2rad(10);
	get_plane_n(&env->objs[2], vec(deg2rad(0), deg2rad(0), deg2rad(20)));
	env->objs[2].col = color(1, 1, 0); 
	env->objs[2].reflect = 0;
	
	env->objs[3].type = 2; 
	env->objs[3].pos = vec(-100, 0, 400);
	env->objs[3].rad = 60;
	get_plane_n(&env->objs[3], vec(deg2rad(50), deg2rad(80), deg2rad(0)));
	env->objs[3].col = color(1, 0, 1); 
	env->objs[3].reflect = 0;
/*
	env->objs[4].type = 4; 
	env->objs[4].pos = vec(500, 80, 400);
	env->objs[4].rad = 20;
	env->objs[4].col = color(1, 1, 1); 
	env->objs[4].reflect = 0;
	
	env->objs[5].type = 0; 
	env->objs[5].pos = vec(-800, 0, 0);
	env->objs[5].rad = 0;
	get_plane_n(&env->objs[5], vec(deg2rad(0), deg2rad(0), deg2rad(-90)));
	env->objs[5].col = color(0.5, 1, 0.5); 
	env->objs[5].reflect = 0;
	
	env->objs[6].type = 0; 
	env->objs[6].pos = vec(800, 0, 0);
	env->objs[6].rad = 0;
	get_plane_n(&env->objs[6], vec(deg2rad(0), deg2rad(0), deg2rad(90)));
	env->objs[6].col = color(0.3, 0.3, 1); 
	env->objs[6].reflect = 0;

	env->objs[7].type = 4; 
	env->objs[7].pos = vec(0, 800, 0);
	env->objs[7].rad = 0;
	get_plane_n(&env->objs[7], vec(deg2rad(0), deg2rad(0), deg2rad(180)));
	env->objs[7].col = color(1, 1, 1); 
	env->objs[7].reflect = 0;
*/
}

int		window_init(t_mlx *mlx, int win_x, int win_y)
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

int		main(int ac, char **av)
{
	t_env	env;

	if (ac != 2)
		return (ft_printf("usage:\n"));
	//PARSING GOES HERE
	//following is temp
	(void)av;
	init_scene(&env);
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
