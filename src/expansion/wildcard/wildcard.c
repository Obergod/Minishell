/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:23:43 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 16:36:47 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/wildcard.h"

// flag 0 ->mot normal
// flag 1 ->mot avec wc
// flag 2 ->separateur
// foutu norme...
t_wc_token	*wc_add_token(t_wc_token **lst, const char *str, int flags,
		t_minishell *minishell)
{
	t_wc_token	*new;
	t_wc_token	*cur;

	new = gc_malloc(sizeof(t_wc_token), minishell->gc);
	if (!new)
		return (NULL);
	new->str = gc_strdup(str, minishell->gc);
	new->is_wildcard = 0;
	new->is_sep = 0;
	if (flags == 1)
		new->is_wildcard = 1;
	if (flags == 2)
		new->is_sep = 1;
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
	int				show_hidden;

	lst = NULL;
	show_hidden = (*pattern == '.');
	dir = opendir(".");
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry)
	{
		if ((show_hidden || entry->d_name[0] != '.') && match_pattern(pattern,
				entry->d_name))
			add_strlist(&lst, entry->d_name, minishell);
		entry = readdir(dir);
	}
	closedir(dir);
	if (!lst)
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
