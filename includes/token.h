/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:16:52 by mafioron          #+#    #+#             */
/*   Updated: 2025/05/13 16:13:26 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "env_parsing.h"
# include "garbage_collector.h"
# include "minishell.h"
# include <string.h>

enum					e_state
{
	NORMAL,
	IN_SQUOTE,
	IN_DQUOTE
};

// Gerer Wildcards
enum					e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR,
	T_LOGIC,
	T_PARANTHESIS
};

typedef struct s_token
{
	char				*str;
	enum e_token_type	type;
	enum e_state		state;
	enum e_state		initial_state;
	struct s_token		*next;
}						t_token;

typedef struct s_tokenizer
{
	int					i;
	t_token				*token_list;
	enum e_state		state;
	enum e_state		token_state;
	int					nb_tok;
	char				*buff;
}						t_tokenizer;

typedef struct s_wildcard
{
	char				*file;
	int					index;
	struct s_wildcard	*next;
}						t_wildcard;

/*****		token_utils		*****/
void	add_token(t_tokenizer *tok, enum e_token_type type, enum e_state state, t_minishell *minishell);
int					quotes_verif(const char *input);
int					verif_quotes(const char *input);

/****		operator		*******/
int					verif_op(const char *input, int	i);
int					is_redir(char op, char next_char, int *i);
int					is_pipe_or_or(char op, char next_char, int *i);
int					is_paranthesis(char c);
int					is_operator(char c);
void				operator_str(const char *input, char *buff, int i);
enum e_token_type	handle_operator(const char *input, int *i);
int					process_operator(const char *input, t_tokenizer *tok, t_minishell *minishell);

/*****		token_process	*****/
t_token				*tokenize(const char *input, t_minishell *minishell);
void				init_tokenizer(t_tokenizer *tok, const char *input, t_minishell *minishell);
int					handle_normal_state(const char *input, t_tokenizer *tok, t_minishell *minishell);
void				handle_quotes(const char *input, t_tokenizer *tok, char quote);
void				finalize_token(t_tokenizer *tok, t_minishell *minishell, const char *input);
void				process_space(t_tokenizer *tok, t_minishell *minishell);

#endif
