/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 16:25:18 by wael-mos          #+#    #+#             */
/*   Updated: 2019/10/23 14:02:35 by wael-mos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int	read_lights(char *line, t_env *env, int num_line)
{
	char	**split_line;
	char	**word;

	if (!ft_isdigit(line[0]) && line[0] != '-')
		return (0);
	if (num_line + 1 > env->num_light)
		errormsg(1);
	if (!(split_line = ft_strsplit(line, '\t')))
		errormsg(2);
	if (!(word = ft_strsplit(split_line[0], ',')))
		errormsg(2);
	env->lights[num_line].pos = vec(ft_atoi(word[0]), ft_atoi(word[1]),\
		ft_atoi(word[2]));
	freetab(word);
	if (!(word = ft_strsplit(split_line[1], ',')))
		errormsg(2);
	env->lights[num_line].col = color(ft_atoi(word[0]), ft_atoi(word[1]),\
		ft_atoi(word[2]));
	env->lights[num_line].col = color(env->lights[num_line].col.r / 100,\
		env->lights[num_line].col.g / 100, env->lights[num_line].col.b / 100);
	freetab(word);
	freetab(split_line);
	return (1);
}

static void	read_env2(char **split_line, t_env *env)
{
	char	**word;

	if (!(word = ft_strsplit(split_line[2], ',')))
		errormsg(2);
	env->cam.rot = vec(deg2rad(ft_atoi(word[0])), deg2rad(ft_atoi(word[1])),\
		deg2rad(ft_atoi(word[2])));
	freetab(word);
	env->cam.fov = ft_atoi(split_line[3]);
	env->ambient = ft_atoi(split_line[4]);
	env->ambient /= 100;
	env->num_light = ft_atoi(split_line[5]);
	env->num_obj = ft_atoi(split_line[6]);
}

static void	read_env(char *line, t_env *env)
{
	char	**split_line;
	char	**word;

	if (!ft_isdigit(line[0]))
		return ;
	if (!(split_line = ft_strsplit(line, '\t')))
		errormsg(2);
	if (!(word = ft_strsplit(split_line[0], ',')))
		errormsg(2);
	env->win_x = ft_atoi(word[0]);
	env->win_y = ft_atoi(word[1]);
	freetab(word);
	if (!(word = ft_strsplit(split_line[1], ',')))
		errormsg(2);
	env->cam.pos = vec(ft_atoi(word[0]), ft_atoi(word[1]), ft_atoi(word[2]));
	freetab(word);
	read_env2(split_line, env);
	freetab(split_line);
	env->lights = (t_light *)ft_memalloc(env->num_light * sizeof(t_light));
	if (env->lights == NULL)
		exit(-1);
	env->objs = (t_obj *)ft_memalloc(env->num_obj * sizeof(t_obj));
	if (env->objs == NULL)
		exit(-1);
}

static void	parsing_loop(int fd, t_env *env, size_t num_lights, size_t num_objs)
{
	char	*line;
	size_t	num_hash;

	num_hash = 0;
	while (get_next_line(fd, &line))
	{
		check_error(line, num_hash);
		if (num_hash == 1)
			read_env(line, env);
		if (num_hash == 2)
			if (read_lights(line, env, num_lights))
				++num_lights;
		if (num_hash == 3)
			if (read_objects(line, env, num_objs))
				++num_objs;
		if (line[0] && line[0] == '#')
			++num_hash;
		ft_strdel(&line);
	}
	if (num_hash == 0)
		errormsg(1);
	env->num_light = num_lights;
	env->num_obj = num_objs;
}

int			parsing(char **av, t_env *env)
{
	int		fd;
	size_t	num_hash;

	if ((fd = open(av[1], O_RDONLY)) == -1)
		exit(-1);
	num_hash = 0;
	parsing_loop(fd, env, 0, 0);
	close(fd);
	return (0);
}
