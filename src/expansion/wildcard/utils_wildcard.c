/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 20:59:26 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/24 21:04:52 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/wildcard.h"

int	is_meta(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>'
		|| c == '&' || c == ';');
}

int	is_filename_char(char c)
{
	if (c == '\0' || c == '/')
		return (0);
	if (is_meta(c))
		return (0);
	return (1);
}

int	contain_wildcard(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '*')
			return (1);
		i++;
	}
	return (0);
}
// add une strlist a une liste chainee 
t_strlist	*add_strlist(t_strlist **lst, const char *str, t_minishell *minishell)
{
	t_strlist	*new;
	t_strlist	*cur;

	new = gc_malloc(sizeof(t_strlist), minishell->gc);
	if (!new)
		return (NULL);
	new->str = gc_strdup(str, minishell->gc);
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

// add tous les elements d'une liste a une autre
void	concat_strlist(t_strlist **dest, t_strlist *src)
{
	t_strlist	*cur;

	if (!src)
		return ;
	if (!*dest)
		*dest = src;
	else
	{
		cur = *dest;
		while (cur->next)
			cur = cur->next;
		cur->next = src;
	}
}