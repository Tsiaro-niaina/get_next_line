/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmandimb <rmandimb@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 09:46:55 by rmandimb          #+#    #+#             */
/*   Updated: 2026/02/28 13:32:55 by rmandimb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*init_remain(char *remain)
{
	if (!remain)
	{
		remain = malloc(1);
		if (!remain)
			return (NULL);
		remain[0] = '\0';
	}
	return (remain);
}

static char	*ft_read_and_accumulate(int fd, char *remain)
{
	char	*buffer;
	char	*temp;
	int		bytes_read;

	remain = init_remain(remain);
	if (!remain)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (free(remain), NULL);
	bytes_read = 1;
	while (!ft_strchr(remain, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(buffer), free(remain), NULL);
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(remain, buffer);
		free(remain);
		remain = temp;
	}
	free(buffer);
	return (remain);
}

static char	*ft_extract_line(char *remain)
{
	int		i;
	char	*line_needed;

	i = 0;
	if (!remain[i])
		return (NULL);
	while (remain[i] && remain[i] != '\n')
		i++;
	line_needed = malloc(sizeof(char) * (i + 2));
	if (!line_needed)
		return (NULL);
	i = 0;
	while (remain[i] && remain[i] != '\n')
	{
		line_needed[i] = remain[i];
		i++;
	}
	if (remain[i] == '\n')
		line_needed[i++] = '\n';
	line_needed[i] = '\0';
	return (line_needed);
}

static char	*ft_update_line(char *remain)
{
	int		i;
	int		j;
	char	*new_remain;

	i = 0;
	while (remain[i] && remain[i] != '\n')
		i++;
	if (!remain[i])
	{
		free(remain);
		return (NULL);
	}
	i++;
	new_remain = malloc(ft_strlen(remain + i) + 1);
	if (!new_remain)
		return (NULL);
	j = 0;
	while (remain[i])
		new_remain[j++] = remain[i++];
	new_remain[j] = '\0';
	free(remain);
	return (new_remain);
}

char	*get_next_line(int fd)
{
	static char	*remain;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	remain = ft_read_and_accumulate(fd, remain);
	if (!remain)
		return (NULL);
	line = ft_extract_line(remain);
	remain = ft_update_line(remain);
	return (line);
}
