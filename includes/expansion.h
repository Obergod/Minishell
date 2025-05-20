/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:41:19 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/20 18:54:28 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "../full_libft/include/libft.h"
# include "garbage_collector.h"
# include "wildcard.h"

struct	s_redir;
struct	s_minishell;

typedef struct s_quotes_state
{
	int				in_squotes;
	int				in_dquotes;
}					t_quotes_state;

typedef struct s_exp_data
{
	char			*new_str;
	int				i;
	int				j;
	t_quotes_state	quotes;
}					t_exp_data;

void				manage_quote_states(char *str, int *i,
						t_quotes_state *quotes);
int					should_keep_char(char c, t_quotes_state quotes);
char				*process_token(char *token, struct s_minishell *minishell);
char				*expand_str(char *str, struct s_minishell *minishell);
char				*get_vars(char *str, int *i, struct s_minishell *minishell);
int					get_full_len(char *str, struct s_minishell *minishell);
char				*remove_quotes(char *str, struct s_minishell *minishell);
int					remove_quotes_redirs(struct s_redir *redirs,
						struct s_minishell *minishell);
char				*dup_token(char *token, struct s_minishell *minishell);
void				copy_var_value(char *var, t_exp_data *data);
void				process_str_char(char *str, t_exp_data *data);
void				process_dollar(char *str, t_exp_data *data,
						struct s_minishell *minishell);
char				*get_exit_status(int *i, struct s_minishell *minishell);
char				*expand_wildcard_if_needed(char *token,
						char *expanded_token, struct s_minishell *minishell);
char				**remove_quotes_array(char **tokens,
						struct s_minishell *minishell);
int					count_cmd_tokens(char **cmd);
char				**expand_vars(char **cmd, struct s_minishell *minishell);

#endif
