/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evogel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 14:56:44 by evogel            #+#    #+#             */
/*   Updated: 2019/10/22 14:57:11 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int	read_type(char *word)
{
	if (!ft_strcmp(word, "Plane") || !ft_strcmp(word, "plane"))
		return (0);
	if (!ft_strcmp(word, "Sphere") || !ft_strcmp(word, "sphere"))
		return (1);
	if (!ft_strcmp(word, "Cylinder") || !ft_strcmp(word, "cylinder"))
		return (2);
	if (!ft_strcmp(word, "Cone") || !ft_strcmp(word, "cone"))
		return (3);
	return (4);
}

static int	read_objects2(char **split_line, int num_line, t_env *env)
{
	char	**word;

	if (!(word = ft_strsplit(split_line[3], ',')))
		errormsg(2);
	if (env->objs[num_line].type != 1)
		env->objs[num_line].rot = get_axe(vec(ft_atoi(word[0]),\
			ft_atoi(word[1]), ft_atoi(word[2])));
	freetab(word);
	if (env->objs[num_line].type == 3)
		env->objs[num_line].rad = deg2rad(ft_atoi(split_line[4]));
	else
		env->objs[num_line].rad = ft_atoi(split_line[4]);
	return (0);
}

int			read_objects(char *line, t_env *env, int num_line)
{
	char	**split_line;
	char	**word;

	if (!ft_isalpha(line[0]))
		return (0);
	if (num_line + 1 > env->num_obj)
		errormsg(1);
	if (!(split_line = ft_strsplit(line, '\t')))
		errormsg(2);
	env->objs[num_line].type = read_type(split_line[0]);
	if (!(word = ft_strsplit(split_line[1], ',')))
		errormsg(2);
	env->objs[num_line].pos = vec(ft_atoi(word[0]), ft_atoi(word[1]),\
		ft_atoi(word[2]));
	freetab(word);
	if (!(word = ft_strsplit(split_line[2], ',')))
		errormsg(2);
	env->objs[num_line].col = color(ft_atoi(word[0]), ft_atoi(word[1]),\
		ft_atoi(word[2]));
	env->objs[num_line].col = color(env->objs[num_line].col.r / 100,\
		env->objs[num_line].col.g / 100, env->objs[num_line].col.b / 100);
	freetab(word);
	read_objects2(split_line, num_line, env);
	freetab(split_line);
	return (1);
}
