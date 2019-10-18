/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niduches <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 17:55:02 by niduches          #+#    #+#             */
/*   Updated: 2019/10/18 11:35:12 by niduches         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "get_next_line_bonus.h"

static void	to_nl(char *buff)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < BUFFER_SIZE && buff[i] && buff[i] != '\n')
		i++;
	j = 0;
	while (j < BUFFER_SIZE && buff[j])
	{
		if (i + 1 < BUFFER_SIZE && buff[i + 1] != '\0')
			buff[j] = buff[1 + i++];
		else if (j < BUFFER_SIZE)
			buff[j] = '\0';
		j++;
	}
	while (j < BUFFER_SIZE)
		buff[j++] = '\0';
}

static int	put_in_buff(char *line, char *tmp, ssize_t size, char *buff)
{
	size_t	i;
	int		ret;

	i = 0;
	ret = (size > 0) ? 1 : 0;
	if (size < 0)
		ret = -1;
	while (i < BUFFER_SIZE && buff[i])
	{
		line[i] = buff[i];
		i++;
	}
	while (size-- > 0)
		buff[size] = tmp[size];
	return (ret);
}

static char	*next_line(int fd, char *buff, size_t nb, int *rt)
{
	char	tmp[BUFFER_SIZE];
	char	*line;
	ssize_t	i;
	ssize_t	size;

	tmp[0] = '\0';
	size = read(fd, tmp, BUFFER_SIZE);
	i = 0;
	while (i < size && tmp[i] && tmp[i] != '\n')
		i++;
	if (tmp[i] == '\n' || size <= 0)
	{
		if (!(line = malloc((nb + i + 1) * sizeof(char))))
			return (NULL);
		*rt = put_in_buff(line, tmp, size, buff);
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

static int	get_next_line_buff(int fd, char **line, char *buff)
{
	size_t		i;
	int			rt;

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
	if (rt == 0 && *line && (*line)[0])
		rt = 1;
	return (rt);
}

int			get_next_line(int fd, char **line)
{
	static int	fds[NB_FD] = {-1, -1, -1, -1, -1};
	static char	buff[NB_FD + 1][BUFFER_SIZE] = {"", "", "", "", "", ""};
	size_t		i;

	if (fd == -1)
		return (-1);
	*line = NULL;
	i = 0;
	while (i < NB_FD)
	{
		if (fd == fds[i])
			return (get_next_line_buff(fd, line, buff[i + 1]));
		i++;
	}
	i = 0;
	while (i < NB_FD)
	{
		if (fds[i] == -1)
		{
			fds[i] = fd;
			return (get_next_line_buff(fd, line, buff[i + 1]));
		}
		i++;
	}
	return (get_next_line_buff(fd, line, buff[0]));
}
