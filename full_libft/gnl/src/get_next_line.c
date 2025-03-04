/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <mafioron@student.42.fr>          +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:22:50 by mafioron          #+#    #+#            */
/*   Updated: 2024/12/03 18:22:53 by mafioron         ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

char	*stock_extend(char *stock, char *buffer)
{
	char	*temp;

	if (!stock)
	{
		temp = gnl_strdup(buffer);
		free(buffer);
		return (temp);
	}
	temp = gnl_strjoin(stock, buffer);
	if (stock)
		free(stock);
	if (buffer)
		free(buffer);
	return (temp);
}

char	*extract_line(char *stock, int fd)
{
	char	*buff;
	int		i;

	i = 1;
	while (!modif_strchr(stock, '\n') && i != 0)
	{
		buff = (char *)gnl_calloc(BUFFER_SIZE + 1, sizeof(char));
		if (!buff)
			return (NULL);
		i = read(fd, buff, BUFFER_SIZE);
		if (i < 0 || (i == 0 && stock == NULL))
		{
			free(stock);
			stock = NULL;
			return (free(buff), NULL);
		}
		buff[i] = '\0';
		stock = stock_extend(stock, buff);
		if (!stock)
			return (stock = NULL, NULL);
	}
	return (stock);
}

char	*precise_line(char *stock)
{
	char	*res;
	int		i;
	int		j;

	j = -1;
	i = 0;
	while (stock[i] != '\n' && stock[i])
		i++;
	res = (char *)gnl_calloc(i + 2, sizeof(char));
	if (!res)
		return (NULL);
	while (++j < i + 1)
		res[j] = stock[j];
	return (res);
}

char	*after_line(char *stock)
{
	char	*s;
	int		i;

	i = 0;
	while (stock[i] != '\n')
		i++;
	if (stock[i + 1] == '\0')
	{
		free(stock);
		return (NULL);
	}
	s = gnl_strdup(stock + (i + 1));
	if (!s)
		return (NULL);
	free(stock);
	return (s);
}

char	*get_next_line(int fd)
{
	static char	*stock[FD_MAX] = {0};
	char		*buff;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (free(stock[fd]), stock[fd] = NULL, NULL);
	if (!stock[fd])
		stock[fd] = gnl_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!stock[fd])
		return (NULL);
	stock[fd] = extract_line(stock[fd], fd);
	if (!stock[fd])
		return (NULL);
	if (stock[fd][0] == '\0')
		return (free(stock[fd]), stock[fd] = NULL, NULL);
	if (!modif_strchr(stock[fd], '\n'))
	{
		buff = gnl_strdup(stock[fd]);
		return (free(stock[fd]), stock[fd] = NULL, buff);
	}
	buff = precise_line(stock[fd]);
	stock[fd] = after_line(stock[fd]);
	return (buff);
}
/*
#include <stdio.h>
int main()
{
	int fd;
	int	fd2;
	char *res;
	int i = 0;
	int	j = 0;

	 //fd = 0;
	fd = open("test.txt", O_RDONLY);
	fd2 = open("test1.txt", O_RDONLY);
	res = NULL;
	while (i != -1 || j != -1)
	{
		res = get_next_line(fd2);
		printf("this is line : %s", res);
		if (res == NULL)
			i = -1;
		if (res)
			free(res);
		res = get_next_line(fd);
		printf("this is line : %s", res);
		if (res == NULL)
			j = -1;
		if (res)
			free(res);
	}
	printf("\n");
	return (0);
}*/
