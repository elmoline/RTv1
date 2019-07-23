/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evogel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 14:38:14 by evogel            #+#    #+#             */
/*   Updated: 2019/07/23 16:24:12 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	init_scene(t_env *env)
{
	env->win_x = WIN_X;
	env->win_y = WIN_Y;

	env->cam.pos = vec(0, 0, 1000);
	env->cam.rot = vec(0, 0, 0);
	env->cam.fov = 30;

	env->ambient = 0.06f;
	env->num_light = 2;
	if (!(env->lights = (t_light *)malloc(env->num_light * sizeof(t_light))))
		exit(-1);
	env->lights[0].pos = vec(300, 300, 350); 
	env->lights[0].col = color(1, 1, 1); 
	env->lights[1].pos = vec(-300, -300, 400); 
	env->lights[1].col = color(0, 0, 0); 

	env->num_obj = 3;
	if (!(env->objs = (t_obj *)malloc(env->num_obj * sizeof(t_obj))))
		exit(-1);
	env->objs[0].type = 1; 
	env->objs[0].pos = vec(0, -50, 0);
	env->objs[0].rad = 100;
	env->objs[0].rot = vec(0, 0, 0);
	env->objs[0].col = color(1, 0, 0); 
	env->objs[0].reflect = 0.6;

	env->objs[1].type = 1; 
	env->objs[1].pos = vec(-100, 30, -5);
	env->objs[1].rad = 50;
	env->objs[1].rot = vec(0, 0, 0);
	env->objs[1].col = color(0, 1, 1); 
	env->objs[1].reflect = 0.2;
	
	env->objs[2].type = 2; 
	env->objs[2].pos = vec(-160, -70, -60);
	env->objs[2].rad = 60;
	env->objs[2].rot = vec(0, 0, 0);
	env->objs[2].col = color(0, 1, 0); 
	env->objs[2].reflect = 0;
	
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
	mlx_hook(env.mlx.win_ptr, 2, 0, deal_key, NULL);
	mlx_loop(env.mlx.mlx_ptr);
	return (0);
}
