/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newline_wildcard.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:06:55 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 15:47:06 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/wildcard.h"

//calculer la longueur de la ligne pour malloc
size_t	rebuild_line_len(t_wc_token *tokens, t_minishell *minishell)
{
	size_t	len;
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

//remplir la ligne avec les tokens
void	rebuild_line_fill(char *res, t_wc_token *tokens, t_minishell *minishell)
{
	t_wc_token	*cur;
	t_strlist	*exp;
	t_strlist	*e;

	cur = tokens;
	while (cur)
	{
		if (cur->is_sep)
			ft_strcat(res, cur->str);
		else if (cur->is_wildcard)
		{
			exp = expand_token(cur->str, minishell);
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
			ft_strcat(res, cur->str);
		cur = cur->next;
	}
}

//reconstruire la ligne
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

// decoupe la ligne en tokens (mots ou separateurs) pour savoir ou sont les wildcards et remettre bien apres les separateurs
t_wc_token	*split_line(const char *input, t_minishell *minishell)
{
	t_wc_token	*lst;
	int			i;
	int			start;
	char		buf[1024];
	int			j;

	lst = NULL;
	i = 0;
	while (input[i])
	{
		if (is_meta(input[i]))
		{
			start = i;
			while (is_meta(input[i]) && input[i])
				i++;
			ft_memcpy(buf, &input[start], i - start);
			buf[i - start] = '\0';
			wc_add_token(&lst, buf, 0, 1, minishell);
		}
		else
		{
			start = i;
			j = 0;
			while (input[i] && !is_meta(input[i]))
				buf[j++] = input[i++];
			buf[j] = '\0';
			wc_add_token(&lst, buf, contain_wildcard(buf), 0, minishell);
		}
	}
	return (lst);
}
