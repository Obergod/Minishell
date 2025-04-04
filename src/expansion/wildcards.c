/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:58:21 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/04 16:58:23 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/token.h"
#include "../../includes/expand.h"
#include <dirent.h>

int	count_slash(char *str)
{
	int	count;
	int	i;

	i = -1;
	count = 0;
	while (str[++i])
	{
		if (str[i] == '/')
			count++;
	}
	return (count);
}

const char *find_path(char *str, t_minishell *minishell)
{
	char	*path;
	int			i;
	int			count;

	count = count_slash(str);
	if (count == 0)
		return (gc_strdup(".", minishell->gc));
	i = -1;
	path = gc_calloc(ft_strlen(str) + 1, sizeof(char), minishell->gc);
//	if (!path)
//		exit
	while (1)
	{
		if (path[i] == '/')
		{
			count--;
			if (!count)
			{
				path[i] = '\0';
				break;
			}
		}
		path[i] = str[i];
	}
	return (path);
}

t_token	expand_wildcards(t_token *token, t_minishell *minishell)
{
	const char		*path;
	DIR				*dir;
	struct dirent	*entry;

	path = find_path(token->str, minishell);
	dir = opendir(path);
	if (!dir)
	{
		perror("opendir failed\n");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		printf("Dir/file : %s\n", entry->d_name);
	}
	closedir(dir);
}
