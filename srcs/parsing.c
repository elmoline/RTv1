/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 16:25:18 by wael-mos          #+#    #+#             */
/*   Updated: 2019/10/17 14:12:32 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <stdio.h>

static void			errormsg(int msg)
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
}

static void			freeshit(char **tab)
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

static int		read_type(char	*word)
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

static int		read_objects(char *line, t_env *env, int num_line)
{
	char	**split_line;
	char	**word;

	if (!ft_isalpha(line[0]))
		return (0);
	if (num_line + 1 > env->num_obj)
		errormsg(1);
	split_line = ft_strsplit(line, '\t');
	env->objs[num_line].type = read_type(split_line[0]);

	word = ft_strsplit(split_line[1], ','); //pos
	env->objs[num_line].pos = vec(ft_atoi(word[0]), ft_atoi(word[1]), ft_atoi(word[2]));
	freeshit(word);

	word = ft_strsplit(split_line[2], ','); //col
	env->objs[num_line].col = color(ft_atoi(word[0]), ft_atoi(word[1]), ft_atoi(word[2]));
	env->objs[num_line].col = color(env->objs[num_line].col.r / 100, env->objs[num_line].col.g / 100, env->objs[num_line].col.b / 100);
	freeshit(word);

	word = ft_strsplit(split_line[3], ','); //rot
	if (env->objs[num_line].type != 1)
		env->objs[num_line].rot = get_axe(vec(ft_atoi(word[0]), ft_atoi(word[1]), ft_atoi(word[2])));
	freeshit(word);

	if (env->objs[num_line].type == 3)
		env->objs[num_line].rad = deg2rad(ft_atoi(split_line[4]));
	else
		env->objs[num_line].rad = ft_atoi(split_line[4]);

	freeshit(split_line);
	return (1);
}

static int		read_lights(char *line, t_env *env, int num_line)
{
	char	**split_line;
	char	**word;

	if (!ft_isdigit(line[0]) && line[0] != '-')
		return (0);
	if (num_line + 1 > env->num_light)
		errormsg(1);
	split_line = ft_strsplit(line, '\t');
	word = ft_strsplit(split_line[0], ',');
	env->lights[num_line].pos = vec(ft_atoi(word[0]), ft_atoi(word[1]), ft_atoi(word[2]));
	freeshit(word);
	word = ft_strsplit(split_line[1], ',');
	env->lights[num_line].col = color(ft_atoi(word[0]), ft_atoi(word[1]), ft_atoi(word[2]));
	env->lights[num_line].col = color(env->lights[num_line].col.r / 100, env->lights[num_line].col.g / 100, env->lights[num_line].col.b / 100);
	freeshit(word);
	freeshit(split_line);
	return (1);
}

static void		read_env(char *line, t_env *env)
{
	char	**split_line;
	char	**word;

	if (!ft_isdigit(line[0]))
		return ;
	split_line = ft_strsplit(line, '\t');
	word = ft_strsplit(split_line[0], ','); //win
	env->win_x = ft_atoi(word[0]);
	env->win_y = ft_atoi(word[1]);
	freeshit(word);
	word = ft_strsplit(split_line[1], ','); //cam
	env->cam.pos = vec(ft_atoi(word[0]), ft_atoi(word[1]) ,ft_atoi(word[2]));
	freeshit(word);
	word = ft_strsplit(split_line[2], ','); //camrot
	env->cam.rot = vec(deg2rad(ft_atoi(word[0])), deg2rad(ft_atoi(word[1])), deg2rad(ft_atoi(word[2])));
	freeshit(word);
	env->cam.fov = ft_atoi(split_line[3]);
	env->ambient = ft_atoi(split_line[4]);
	env->ambient /= 100;
	env->num_light = ft_atoi(split_line[5]);
	env->num_obj = ft_atoi(split_line[6]);
	freeshit(split_line);
	if (!(env->lights = (t_light *)ft_memalloc(env->num_light * sizeof(t_light))))
		exit(-1);
	if (!(env->objs = (t_obj *)ft_memalloc(env->num_obj * sizeof(t_obj))))
		exit(-1);
}

static void		check_comma(char **split_line, int i, int size)
{
	size_t		count;
	size_t		num_comma;
	
	while (i <= size)
	{
		num_comma = 0;
		count = 0;
		while (split_line[i][count])
		{
			if (split_line[i][count] == ',')
				++num_comma;
			++count;
		}
		if (num_comma != 2)
			errormsg(1);
		++i;
	}
}

static void		check_objs(char *line)
{
	char	**split_line;
	
	split_line = ft_strsplit(line, '\t');
	check_comma(split_line, 1, 3);
	freeshit(split_line);
}

static void		check_lights(char *line)
{
	char	**split_line;

	split_line = ft_strsplit(line, '\t');
	check_comma(split_line, 0, 1);
	freeshit(split_line);
}

static void		check_env(char *line)
{
	char	**split_line;
	int		count;
	int		num_comma;

	count = 0;
	num_comma = 0;
	split_line = ft_strsplit(line, '\t');
	while (split_line[0][count])
		if (split_line[0][count++] == ',')
			++num_comma;
	if (num_comma != 1)
		errormsg(1);
	check_comma(split_line, 1, 2);
	freeshit(split_line);
}

static void		check_error(char *line, size_t num_hash)
{
	int		count;

	count = 0;
	if (line[0] && line[0] != '#')
	{
		while (line[count])
		{
			if (!ft_isdigit(line[count]) && !ft_isalpha(line[count])\
				&& line[count] != ',' && line[count] != '\t' \
				&& line[count] != '-' && line[count] != ' ')
				errormsg(0);
			++count;
		}
	}
	if (num_hash == 1 && ft_isdigit(line[0]))
		check_env(line);
	if (num_hash == 2 && (ft_isdigit(line[0]) || line[0] == '-'))
		check_lights(line);
	if (num_hash == 3 && ft_isalpha(line[0]))
		check_objs(line);
}

int		parsing(char **av, t_env *env)
{
	int		fd;
	char	*line;
	size_t	num_lights;
	size_t	num_objs;
	size_t	num_hash;

	if ((fd = open(av[1], O_RDONLY)) == -1)
		exit(-1);
	num_lights = 0;
	num_objs = 0;
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
	env->num_light = num_lights;
	env->num_obj = num_objs;
	close(fd);
    return (0);
}
