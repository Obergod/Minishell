/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <mafioron@student.42.fr>          +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:22:50 by mafioron          #+#    #+#            */
/*   Updated: 2024/12/03 18:22:53 by mafioron         ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# ifndef FD_MAX
#  define FD_MAX 1024
# endif

# include <fcntl.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>

char		*get_next_line(int fd);
size_t		gnl_strlen(const char *s);
void		*gnl_calloc(size_t nmemb, size_t size);
char		*gnl_strjoin(char const *s1, char const *s2);
int			modif_strchr(const char *s, int c);
char		*gnl_strdup(const char *s);
char		*stock_extend(char *stock, char *buffer);

#endif
