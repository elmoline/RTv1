/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evogel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 14:03:04 by evogel            #+#    #+#             */
/*   Updated: 2019/10/22 15:54:02 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_col	color(float red, float green, float blue)
{
	t_col	res;

	res.r = red;
	res.g = green;
	res.b = blue;
	return (res);
}

void	add_color(t_col *ori, t_col *add, float amount)
{
	ori->r += add->r * amount;
	ori->g += add->g * amount;
	ori->b += add->b * amount;
}

void	errormsg(int msg)
{
	if (msg == 0)
	{
		ft_printf("Error: Wrong character in the scene file.\n");
		exit(-1);
	}
	else if (msg == 1)
	{
		ft_printf("Error: Wrong format in the scene file.\n");
		exit(-1);
	}
	else if (msg == 2)
	{
		ft_printf("Error: Failed malloc.\n");
		exit(-1);
	}
}

void	freetab(char **tab)
{
	int		c;

	c = 0;
	while (tab[c])
	{
		free(tab[c]);
		tab[c] = NULL;
		++c;
	}
	free(tab);
	tab = NULL;
}
