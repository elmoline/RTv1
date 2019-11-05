/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_mlx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evogel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 14:31:37 by evogel            #+#    #+#             */
/*   Updated: 2019/11/05 15:06:20 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		deal_key(int key, void *param)
{
	t_env	*env;

	env = (t_env *)param;
	if (key == 53)
		exit(1);
	return (0);
}

int		deal_expose(t_env *env)
{
	mlx_put_image_to_window(env->mlx.mlx_ptr, env->mlx.win_ptr, \
		env->mlx.img_ptr, 0, 0);
	return (0);
}

int		deal_close(void)
{
	exit(1);
	return (0);
}

int		error_mlx(void)
{
	ft_printf("Error creating window with MLX\n");
	exit(-1);
}
