/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niduches <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 12:27:24 by niduches          #+#    #+#             */
/*   Updated: 2019/10/09 18:39:51 by niduches         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"

static void	to_nl(char *buff)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (buff[i] && buff[i] != '\n')
		i++;
	j = 0;
	while (buff[j])
	{
		if (buff[i + 1] != '\0')
			buff[j] = buff[1 + i++];
		else
			buff[j] = '\0';
		j++;
	}
}

static void	put_in_buff(char *line, char *tmp, size_t size, char *buff)
{
	size_t	i;

	i = 0;
	while (buff[i])
	{
		line[i] = buff[i];
		i++;
	}
	while (size-- > 0)
		buff[size] = tmp[size];
}

static char	*next_line(int fd, char *buff, size_t nb, int *rt)
{
	char	tmp[BUFFER_SIZE];
	char	*line;
	size_t	i;
	size_t	size;

	size = read(fd, tmp, BUFFER_SIZE);
	i = 0;
	while (i < size && tmp[i] && tmp[i] != '\n')
		i++;
	if (tmp[i] == '\n' || !size)
	{
		if (!(line = malloc((nb + i + 1) * sizeof(char))))
			return (NULL);
		*rt = (!size) ? 0 : 1;
		put_in_buff(line, tmp, size, buff);
		line[nb + i] = '\0';
		while (i-- > 0)
			line[nb + i] = tmp[i];
		return (line);
	}
	if (!(line = next_line(fd, buff, nb + size, rt)))
		return (NULL);
	while (size-- > 0)
		line[nb + size] = tmp[size];
	return (line);
}

int			get_next_line(int fd, char **line)
{
	static char	buff[BUFFER_SIZE] = "";
	size_t		i;
	int			rt;

	*line = NULL;
	if (fd == -1)
		return (-1);
	i = 0;
	rt = 1;
	while (i < BUFFER_SIZE && buff[i] && buff[i] != '\n')
		i++;
	if (buff[i] == '\n')
	{
		if (!(*line = malloc((i + 1) * sizeof(char))))
			return (-1);
		(*line)[i] = '\0';
		while (i-- > 0)
			(*line)[i] = buff[i];
	}
	else if (buff[i] != '\n' && !(*line = next_line(fd, buff, i, &rt)))
		return (-1);
	to_nl(buff);
	return (rt);
}
