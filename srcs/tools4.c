/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evogel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 14:03:04 by evogel            #+#    #+#             */
/*   Updated: 2019/10/15 14:19:03 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

float	deg2rad(int d)
{
	return (d * M_PI / 180);
}

int		deal_key(int key, void *param)
{
	t_env	*env;

	env = (t_env *)param;
	if (key == 53)
		exit(1);
	else if (key == 36)
		write_ppm(key, env);
	return (0);
}

int		deal_close(void)
{
	exit(1);
	return (0);
}

int		error2(void)
{
	ft_printf("Error creating window with MLX\n");
	exit(-1);
}
