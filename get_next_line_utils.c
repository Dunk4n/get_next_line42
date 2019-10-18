/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niduches <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 14:08:41 by niduches          #+#    #+#             */
/*   Updated: 2019/10/18 17:53:54 by niduches         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "get_next_line.h"

void	to_nl(char *buff)
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
