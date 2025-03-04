/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:48:48 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/14 17:08:25 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (fd == -1)
		return ;
	write(fd, s, ft_strlen(s));
}
/*
int	main(void)
{
	int	fd;

	fd = open("test.txt", O_WRONLY);
	ft_putstr_fd("test", fd);
	close(fd);
	return (0);
}*/
