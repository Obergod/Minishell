/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:16:52 by mafioron          #+#    #+#             */
/*   Updated: 2025/03/22 17:42:11 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include <string.h>
# include "minishell.h"
# include "env_parsing.h"
# include "garbage_collector.h"

extern int g_exit_status;

enum		e_state
{
	NORMAL,
	IN_SQUOTE,
	IN_DQUOTE
};

<<<<<<< HEAD
// Gerer Wildcards
=======
>>>>>>> ufalzone
enum		e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR,
	T_LOGIC,
	T_PARANTHESIS
	//T_PARANTHESIS_L
};

typedef struct	s_token
{
	char				*str;
	enum e_token_type	type;
	enum e_state		state;
	enum e_state		initial_state;
	struct s_token		*next;
}	t_token;

typedef struct s_tokenizer
{
	int				i;
	t_token			*token_list;
	enum e_state	state;
	enum e_state	token_state;
	int				nb_tok;
	char			*buff;
}	t_tokenizer;

typedef struct	s_wildcard
{
	char	*file;
	int		index;
	struct s_wildcard *next;
}	t_wildcard;

/*****		A trier			*****/
t_token	*tokenize(const char *input, t_minishell *minishell);

/*****		token_utils		*****/
int					is_operator(char c);
void				operator_str(const char *input, char *buff, int i);
enum e_token_type	handle_operator(const char *input, int *i);
void				add_token(t_token **token, char *buff, enum e_token_type type, enum e_state state, t_minishell *minishell);
int					quotes_verif(const char *input);

/*****		token_process	*****/
void				init_tokenizer(t_tokenizer *tok, const char *input, t_minishell *minishell);
void				handle_normal_state(const char *input, t_tokenizer *tok, t_minishell *minishell);
void				handle_quotes(const char *input, t_tokenizer *tok, char quote);
void				finalize_token(t_tokenizer *tok, t_minishell *minishell);
void				process_space(t_tokenizer *tok, t_minishell *minishell);
void				process_operator(const char *input, t_tokenizer *tok, t_minishell *minishell);

#endif
