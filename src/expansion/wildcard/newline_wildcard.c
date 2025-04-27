/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newline_wildcard.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:06:55 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 19:11:19 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/wildcard.h"

// calculer la longueur de la ligne pour malloc
size_t	rebuild_line_len(t_wc_token *tokens, t_minishell *minishell)
{
	size_t		len;
	t_strlist	*e;

	len = 0;
	while (tokens)
	{
		if (tokens->is_wildcard)
		{
			e = expand_token(tokens->str, minishell);
			while (e)
			{
				len += strlen(e->str);
				if (e->next)
					len++;
				e = e->next;
			}
		}
		else
			len += strlen(tokens->str);
		tokens = tokens->next;
	}
	return (len);
}

// remplir la ligne avec les tokens
void	rebuild_line_fill(char *res, t_wc_token *tokens, t_minishell *minishell)
{
	t_wc_token	*current;
	t_strlist	*exp;
	t_strlist	*e;

	current = tokens;
	while (current)
	{
		if (current->is_sep)
			ft_strcat(res, current->str);
		else if (current->is_wildcard)
		{
			exp = expand_token(current->str, minishell);
			e = exp;
			while (e)
			{
				ft_strcat(res, e->str);
				if (e->next)
					ft_strcat(res, " ");
				e = e->next;
			}
		}
		else
			ft_strcat(res, current->str);
		current = current->next;
	}
}

// reconstruire la ligne
char	*rebuild_line(t_wc_token *tokens, t_minishell *minishell)
{
	char	*res;
	size_t	len;

	len = rebuild_line_len(tokens, minishell);
	res = gc_malloc(len + 1, minishell->gc);
	if (!res)
		return (NULL);
	res[0] = '\0';
	rebuild_line_fill(res, tokens, minishell);
	return (res);
}

// decoupe la ligne en tokens (mots ou separateurs)
// pour savoir ou sont les wildcards et remettre bien apres les separateurs
static int	fill_buf(const char *p, char *buf, int sep)
{
	int	len;

	len = 0;
	while ((sep && is_meta(p[len])) || (p[len] && !sep && !is_meta(p[len])))
	{
		buf[len] = p[len];
		len++;
	}
	buf[len] = '\0';
	return (len);
}

t_wc_token	*split_line(const char *input, t_minishell *minishell)
{
	t_wc_token	*lst;
	const char	*p;
	char		buf[1024];
	int			len;
	int			flag;

	p = input;
	lst = NULL;
	while (*p)
	{
		flag = 0;
		if (is_meta(*p))
			flag = 2;
		len = fill_buf(p, buf, flag == 2);
		if (flag == 0)
		{
			if (contain_wildcard(buf))
				flag = 1;
		}
		wc_add_token(&lst, buf, flag, minishell);
		p += len;
	}
	return (lst);
}
