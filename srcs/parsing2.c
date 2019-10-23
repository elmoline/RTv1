/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evogel <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 14:17:59 by evogel            #+#    #+#             */
/*   Updated: 2019/10/22 14:34:50 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void	check_comma(char **split_line, int i, int size)
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

static void	check_objs(char *line)
{
	char	**split_line;
	int		count;

	if (!(split_line = ft_strsplit(line, '\t')))
		errormsg(2);
	count = 0;
	while (count <= 4)
	{
		if (!split_line[count])
			errormsg(1);
		++count;
	}
	check_comma(split_line, 1, 3);
	freetab(split_line);
}

static void	check_lights(char *line)
{
	char	**split_line;
	int		count;

	if (!(split_line = ft_strsplit(line, '\t')))
		errormsg(2);
	count = 0;
	while (count <= 1)
	{
		if (!split_line[count])
			errormsg(1);
		++count;
	}
	check_comma(split_line, 0, 1);
	freetab(split_line);
}

static void	check_env(char *line)
{
	char	**split_line;
	int		count;
	int		num_comma;

	count = 0;
	num_comma = 0;
	if (!(split_line = ft_strsplit(line, '\t')))
		errormsg(2);
	while (count <= 6)
	{
		if (!split_line[count])
			errormsg(1);
		++count;
	}
	count = 0;
	while (split_line[0][count])
		if (split_line[0][count++] == ',')
			++num_comma;
	if (num_comma != 1)
		errormsg(1);
	check_comma(split_line, 1, 2);
	freetab(split_line);
}

void		check_error(char *line, size_t num_hash)
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
