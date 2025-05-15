/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 20:24:48 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/15 19:04:07 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARD_H
# define WILDCARD_H

# include "../full_libft/include/libft.h"
# include "garbage_collector.h"
# include "minishell.h"
# include <dirent.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

// stock un token (mot ou separateur) stock les separateurs par groupe
typedef struct s_wc_token
{
	char				*str;
	int					is_wildcard;
	int					is_sep;
	struct s_wc_token	*next;
}						t_wc_token;

// liste chainee (resultats d'expansion)
typedef struct s_strlist
{
	char				*str;
	struct s_strlist	*next;
}						t_strlist;

int						is_meta(char c);
int						is_filename_char(char c);
int						contain_wildcard(const char *s);
t_strlist				*add_strlist(t_strlist **lst, const char *str,
							t_minishell *minishell);
void					concat_strlist(t_strlist **dest, t_strlist *src);
size_t					rebuild_line_len(t_wc_token *tokens,
							t_minishell *minishell);
void					rebuild_line_fill(char *res, t_wc_token *tokens,
							t_minishell *minishell);
char					*rebuild_line(t_wc_token *tokens,
							t_minishell *minishell);
t_wc_token				*split_line(const char *input, t_minishell *minishell);
t_strlist				*wc_dup_token(const char *str, t_minishell *minishell);
char					*expand_wildcards(const char *input,
							t_minishell *minishell);
t_strlist				*expand_token(const char *pattern,
							t_minishell *minishell);
t_wc_token				*wc_add_token(t_wc_token **lst, const char *str,
							int flags, t_minishell *minishell);
int						match_pattern(const char *pattern, const char *str);
#endif