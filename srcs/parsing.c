/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 16:25:18 by wael-mos          #+#    #+#             */
/*   Updated: 2019/10/15 17:30:56 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

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

static void		read_objects(char *line, t_env *env, int num_line)
{
	char	**split_line;
	char	**word;

	if (!ft_isalpha(line[0]))
		return ;
	split_line = ft_strsplit(line, '\t');
	env->objs[num_line].type = read_type(split_line[0]);

	word = ft_strsplit(split_line[1], ','); //pos
	env->objs[num_line].pos = vec(ft_atoi(word[0]), ft_atoi(word[1]), ft_atoi(word[2]));
	freeshit(word);

	if (env->objs[num_line].type == 3)
		env->objs[num_line].rad = deg2rad(ft_atoi(split_line[2]));
	else
		env->objs[num_line].rad = ft_atoi(split_line[2]);

	word = ft_strsplit(split_line[3], ','); //rot
	if (env->objs[num_line].type != 1)
		env->objs[num_line].rot = get_axe(vec(ft_atoi(word[0]), ft_atoi(word[1]), ft_atoi(word[2])));
	freeshit(word);

	word = ft_strsplit(split_line[4], ','); //col
	env->objs[num_line].col = color(ft_atoi(word[0]), ft_atoi(word[1]), ft_atoi(word[2]));
	env->objs[num_line].col = color(env->objs[num_line].col.r / 100, env->objs[num_line].col.g / 100, env->objs[num_line].col.b / 100);
	freeshit(word);

	env->objs[num_line].reflect = ft_atoi(split_line[5]);


	freeshit(split_line);
}

static void		read_lights(char *line, t_env *env, int num_line)
{
	char	**split_line;
	char	**word;

	if (!ft_isdigit(line[0]))
		return ;
	split_line = ft_strsplit(line, '\t');

	word = ft_strsplit(split_line[0], ',');
	env->lights[num_line].pos = vec(ft_atoi(word[0]), ft_atoi(word[1]), ft_atoi(word[2]));
	freeshit(word);

	word = ft_strsplit(split_line[1], ',');
	env->lights[num_line].col = color(ft_atoi(word[0]), ft_atoi(word[1]), ft_atoi(word[2]));
	env->lights[num_line].col = color(env->lights[num_line].col.r / 100, env->lights[num_line].col.g / 100, env->lights[num_line].col.b / 100);
	freeshit(word);

	freeshit(split_line);
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
	if (!(env->lights = (t_light *)malloc(env->num_light * sizeof(t_light))))
		exit(-1);
	if (!(env->objs = (t_obj *)malloc(env->num_obj * sizeof(t_obj))))
		exit(-1);
}

static void		check_error(char *line)
{
	int		count;

	count = 0;
	if (line[0] && line[0] != '#')
	{
		while (line[count])
		{
			if (!ft_isdigit(line[count]) && !ft_isalpha(line[count])\
				&& line[count] != ',' && line[count] != '\t' && line[count] != '-')
			{
				ft_printf("Error: Wrong character in the scene file. (%c)\n", line[count]);
				exit(-1);
			}
			++count;
		}
	}
}

int		parsing(int ac, char **av, t_env *env)
{
	int		fd;
	char	*line;
	size_t	num_lights;
	size_t	num_objs;
	size_t	num_hash;

	ac = 0;
	if ((fd = open(av[1], O_RDONLY)) == -1)
		exit(-1);
	num_lights = 0;
	num_objs = 0;
	num_hash = 0;
	while (get_next_line(fd, &line))
	{
		check_error(line);
		if (num_hash == 1)
			read_env(line, env);
		if (num_hash == 2)
		{
			read_lights(line, env, num_lights);
			++num_lights;
		}
		if (num_hash == 3)
		{
			read_objects(line, env, num_objs);
			++num_objs;
		}
		if (line[0] && line[0] == '#')
			++num_hash;
		ft_strdel(&line);
	}
	close(fd);
    return (0);
}
