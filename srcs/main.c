/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 14:38:14 by evogel            #+#    #+#             */
/*   Updated: 2019/10/23 14:24:06 by wael-mos         ###   ########.fr       */
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

static int	window_init(t_mlx *mlx, int win_x, int win_y)
{
	int	tmp;

	if (!(mlx->mlx_ptr = mlx_init()))
		return (-1);
	if (!(mlx->win_ptr = mlx_new_window(mlx->mlx_ptr, win_x, win_y, "RTv1")))
		return (-1);
	if (!(mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, win_x, win_y)))
		return (-1);
	if (!(mlx->data = (int *)mlx_get_data_addr(mlx->img_ptr, &tmp, &tmp, &tmp)))
		return (-1);
	return (0);
}

int			main(int ac, char **av)
{
	t_env	env;

	if (ac != 2)
		errormsg(3);
	ft_bzero((void *)&env, sizeof(t_env));
	parsing(av, &env);
	if (window_init(&env.mlx, env.win_x, env.win_y) == -1)
		error_mlx();
	render(&env);
	mlx_expose_hook(env.mlx.win_ptr, deal_expose, &env);
	mlx_hook(env.mlx.win_ptr, 17, (1L << 17), deal_close, NULL);
	mlx_hook(env.mlx.win_ptr, 2, 0, deal_key, &env);
	mlx_loop(env.mlx.mlx_ptr);
	return (0);
}
