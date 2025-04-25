/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:23:43 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/24 21:12:45 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/wildcard.h"

t_wc_token	*wc_add_token(t_wc_token **lst, const char *str, int is_wc,
		int is_sep, t_minishell *minishell)
{
	t_wc_token	*new;
	t_wc_token	*cur;

	new = gc_malloc(sizeof(t_wc_token), minishell->gc);
	if (!new)
		return (NULL);
	new->str = gc_strdup(str, minishell->gc);
	new->is_wildcard = is_wc;
	new->is_sep = is_sep;
	new->next = NULL;
	if (!*lst)
		*lst = new;
	else
	{
		cur = *lst;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
	return (new);
}

// recursive match un pattern
int	match_pattern(const char *pattern, const char *str)
{
	if (!*pattern)
		return (!*str);
	if (*pattern == '*')
	{
		if (match_pattern(pattern + 1, str))
			return (1);
		if (*str && match_pattern(pattern, str + 1))
			return (1);
		return (0);
	}
	if (*str && *pattern == *str)
		return (match_pattern(pattern + 1, str + 1));
	return (0);
}

// expansion d'un token wildcard en liste de fichiers
t_strlist	*expand_token(const char *pattern, t_minishell *minishell)
{
	DIR				*dir;
	struct dirent	*entry;
	t_strlist		*lst;
	int				found;
	int				show_hidden;

	lst = NULL;
	found = 0;
	show_hidden = (pattern[0] == '.');
	dir = opendir(".");
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry)
	{
		if ((show_hidden || entry->d_name[0] != '.') && match_pattern(pattern,
				entry->d_name))
		{
			add_strlist(&lst, entry->d_name, minishell);
			found = 1;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (!found)
		add_strlist(&lst, pattern, minishell);
	return (lst);
}

t_strlist	*wc_dup_token(const char *str, t_minishell *minishell)
{
	t_strlist	*lst;

	lst = NULL;
	add_strlist(&lst, str, minishell);
	return (lst);
}

char	*expand_wildcards(const char *input, t_minishell *minishell)
{
	t_wc_token	*tokens;
	char		*final;

	tokens = split_line(input, minishell);
	final = rebuild_line(tokens, minishell);
	return (final);
}

// int	main(int ac, char **av)
// {
// 	char *res;
// 	if (ac < 2)
// 	{
// 		printf("Usage: %s <commande avec wildcards>\n", av[0]);
// 		return (1);
// 	}
// 	res = expand_wildcards(av[1], minishell);
// 	if (res)
// 	{
// 		printf("%s\n", res);
// 		free(res);
// 	}
// 	else
// 		printf("Erreur ou aucun résultat\n");
// 	return (0);
// }